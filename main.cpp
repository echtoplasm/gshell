#include <atomic>
#include <chrono>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <thread>

#include "headers/SystemInfo.h"
#include "headers/customization.h"
#include "headers/kittyTerminal.h"
#include "headers/readThermal.h"
using namespace ftxui;

std::string KittyTerminalImg::gifPath = "";

int main() {
  system("clear");

  // kittyTerminal.h struct
  KittyTerminalImg gifDisplay;

  // systeminfo.h struct
  SystemInfo info;
  info.update();

  // readThermal.h struct
  Thermal thermal;
  thermal.getTemp0();

  // customization.h struct
  Customization custom;
  custom.ensureConfigExists();
  custom.getConfSettings();
  custom.applyUserConf();
  custom.applyKittyTermConf();

  // Frontend gif rendering
  // and ftxui screen render
  gifDisplay.displayGif(56, 1, 40, 20);
  auto screen = ScreenInteractive::FixedSize(56, 20);

  // program clock
  std::atomic<bool> running{true};

  // thread for calling systeminfo.h update
  // in one second intervals
  std::thread update_thread([&] {
    while (running) {
      info.update();
      screen.Post(Event::Custom);
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  });

  // thread for calling thermal.h getTemp0
  // one second intervals
  std::thread getTemp([&] {
    while (running) {
      thermal.getTemp0();
      screen.Post(Event::Custom);
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  });

  // thread for applying customization.h userConf
  // 2 second intervals
  std::thread updateCustom([&] {
    while (running) {
      custom.applyUserConf();
      screen.Post(Event::Custom);
      std::this_thread::sleep_for(std::chrono::seconds(2));
    }
  });

  // FTXUI renderer component
  auto component = Renderer([&] {
    return vbox({
               text(custom.tagline) | bold | center |
                   color(custom.taglineColor),
               separatorLight(),
               hbox({
                   text(info.distroName) | color(custom.distroColor),

               }),
               separatorLight(),
               hbox({
                   text("Uptime: "),
                   text(info.getUptimeString()) | color(custom.uptimeColor),
               }),
               separatorLight(),

               hbox({
                   text("Processor: "),
                   text(info.processorName) | color(custom.processorColor),
               }),
               separatorLight(),
               hbox({
                   text("GPU: "),
                   text(info.gpuName) | color(custom.gpuColor),
               }),
               separatorLight(),
               hbox({
                   text("RAMuse: ") | center,
                   border(gauge(info.ram_percent / 100.0f)) |
                       color(custom.ramGaugeColor) | size(WIDTH, EQUAL, 40),
                   text(" " +
                        std::to_string(static_cast<int>(info.ram_percent)) +
                        "%") |
                       center,
               }),

               hbox({
                   text("CPUtmp: ") | center,
                   border(gauge(thermal.tempC / 100.0f)) |
                       color(custom.cpuGaugeColor) | size(WIDTH, EQUAL, 40),
                   text(" " + std::to_string(static_cast<int>(thermal.tempC)) +
                        "°C") |
                       center,
               }),
           }) |
           border;
  });
  screen.Loop(component);
  running = false;
  update_thread.join();
  getTemp.join();
  updateCustom.join();

  screen.TrackMouse(false);
  return 0;
}
