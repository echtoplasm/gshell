#include <atomic>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include "headers/SystemInfo.h"

using namespace ftxui;

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

  /**
   * @brief 
   */
  auto component = Renderer([&] {
    return vbox({
               text("See you space cowboy") | bold | center | color(Color::MagentaLight),
               separatorLight(),
               hbox({
                   text(info.distroName) | color(Color::CyanLight),

               }),
               separatorLight(),
               hbox({
                   text("Uptime: "),
                   text(info.getUptimeString()) | color(Color::Cornsilk1),
               }),
               separatorLight(),

               hbox({
                   text("Processor: "),
                   text(info.processorName) | color(Color::MagentaLight),
               }),
               separatorLight(),
               hbox({
                   text("GPU: "),
                   text(info.gpuName) | color(Color::CyanLight),
               }),
                separatorLight(),
                hbox({
                   text("RAM: ") | center,
                   border(gauge(info.ram_percent / 100.0f)) |
                       color(Color::Cornsilk1) | size(WIDTH, EQUAL, 40),
                   text(" " +
                        std::to_string(static_cast<int>(info.ram_percent)) +
                        "%") |
                       center,
               }),
               separatorLight(),
           }) |
           border;
  });
  screen.Loop(component);
  running = false;
  update_thread.join();
  screen.TrackMouse(false);
  return 0;
}
