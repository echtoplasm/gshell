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

    ram_percent = 100.0f * (1.0f - (float)mem_available / mem_total);
  }

  std::string getUptimeString() const {
    unsigned long total = static_cast<unsigned long>(uptime_seconds);
    unsigned long days = total / 86400;
    unsigned long hours = (total % 86400) / 3600;
    unsigned long minutes = (total % 3600) / 60;

    std::ostringstream oss;
    if (days > 0)
      oss << days << "d ";
    oss << hours << "h " << minutes << "m";
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
  gifDisplay.displayGif(50, 2, 40, 20);

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  auto screen = ScreenInteractive::FixedSize(40, 25);
  SystemInfo info;
  std::atomic<bool> running{true};

  std::thread update_thread([&] {
    while (running) {
      info.update();
      screen.Post(Event::Custom);
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  });

  auto component = Renderer([&] {
    return

        hbox({
            text("") | size(WIDTH, EQUAL, 100),
        }),

        vbox({
            text("System Monitor") | bold | color(Color::Cyan),
            separatorLight(),
            hbox({
                text("Uptime: "),
                text(info.getUptimeString()) | color(Color::Green),
            }),
            separatorLight(),
            hbox({
                text("RAM: ") | center,
                border(gauge(info.ram_percent / 100.0f)) |
                    color(Color::BlueLight) | size(WIDTH, EQUAL, 40),
                text(" " + std::to_string(static_cast<int>(info.ram_percent)) +
                     "%") |
                    center,
            }),
            separatorLight(),
        }) | border;
  });
  screen.Loop(component);
  running = false;
  update_thread.join();
  screen.TrackMouse(false);
  return 0;
}
