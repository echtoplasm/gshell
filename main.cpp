#include <atomic>
#include <chrono>
#include <fstream>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <iostream>
#include <sstream>
#include <thread>

using namespace ftxui;

struct SystemInfo {
  double uptime_seconds;
  double idle_seconds;
  float ram_percent;
  float cpu_percent;
  std::string processorName;
  std::string gpuName;

  std::string getProcessorInfo() {
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    std::string processorName;

    while (std::getline(cpuinfo, line)) {
      if (line.find("model name") != std::string::npos) {
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
          processorName = line.substr(colonPos + 2); // +2 to skip ": "
        } else {
          processorName = "Unknown CPU";
        }
      }
    }
    cpuinfo.close();
    return processorName;
  }

  std::string getGpuName() {
    std::string gpuPath =
        R"(/proc/driver/nvidia/gpus/0000:01:00.0/information)";
    std::ifstream gpuinfo(gpuPath);
    std::string line;

    while (std::getline(gpuinfo, line)) {
      if (line.find("Model") != std::string::npos) {
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
          gpuName = line.substr(colonPos + 1);
          size_t start = gpuName.find_first_not_of('\t');
          if (start != std::string::npos) {
            gpuName = gpuName.substr(start);
            break;
          } else {
            gpuName = "Unknown GPU";
          }
        }
      }
    }
    gpuinfo.close();
    return gpuName;
  }

  void update() {
    std::ifstream uptime("/proc/uptime");
    uptime >> uptime_seconds >> idle_seconds;

    std::ifstream meminfo("/proc/meminfo");
    unsigned long mem_total = 0, mem_available = 0;
    std::string line, key;
    unsigned long value;

    while (std::getline(meminfo, line)) {
      std::istringstream iss(line);
      if (iss >> key >> value) {
        if (key == "MemTotal:")
          mem_total = value;
        else if (key == "MemAvailable:")
          mem_available = value;
      }
    }
    if (processorName.empty()) {
      processorName = getProcessorInfo();
    }

    if (gpuName.empty()) {
      gpuName = getGpuName();
    }

    ram_percent = 100.0f * (1.0f - (float)mem_available / mem_total);
    uptime.close();
  }

  std::string getUptimeString() const {
    unsigned long total = static_cast<unsigned long>(uptime_seconds);
    unsigned long days = total / 86400;
    unsigned long hours = (total % 86400) / 3600;
    unsigned long minutes = (total % 3600) / 60;
    unsigned long seconds = (total % 60);

    std::ostringstream oss;
    if (days > 0)
      oss << days << "d ";
    oss << hours << "h " << minutes << "m " << seconds << "s";
    return oss.str();
  }
};

struct KittyTerminalImg {
  std::string gifPath;

  void displayGif(int x, int y, int width, int height) {
    std::cout << "\033[" << y << ";" << x << "H";
    std::string cmd = "kitty +kitten icat --place " + std::to_string(width) +
                      "x" + std::to_string(height) + "@" + std::to_string(x) +
                      "x" + std::to_string(y) + " " + gifPath;
    system(cmd.c_str());
  }
};

int main() {
  system("clear");

  KittyTerminalImg gifDisplay;
  gifDisplay.gifPath = "~/dev/gshell/giphy.gif";
  gifDisplay.displayGif(53, 1, 40, 20);

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  auto screen = ScreenInteractive::FixedSize(52, 20);
  SystemInfo info;

  info.update();

  std::atomic<bool> running{true};

  std::thread update_thread([&] {
    while (running) {
      info.update();
      screen.Post(Event::Custom);
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  });

  auto component = Renderer([&] {
    return vbox({text("System Monitor") | bold | color(Color::CyanLight),
                 separatorLight(),
                 hbox({
                     text("Uptime: "),
                     text(info.getUptimeString()) | color(Color::GreenLight),
                 }),
                 separatorLight(),
                 hbox({
                     text("RAM: ") | center,
                     border(gauge(info.ram_percent / 100.0f)) |
                         color(Color::BlueLight) | size(WIDTH, EQUAL, 40),
                     text(" " +
                          std::to_string(static_cast<int>(info.ram_percent)) +
                          "%") |
                         center,
                 }),
                 separatorLight(),
                 hbox({
                     text("Processor: "),
                     text(info.processorName) | color(Color::MagentaLight),
                 }),
                 separatorLight(),
                 hbox({
                     text("GPU: "),
                     text(info.gpuName) | color(Color::YellowLight),
                 })}) |
           border;
  });
  screen.Loop(component);
  running = false;
  update_thread.join();
  screen.TrackMouse(false);
  return 0;
}
