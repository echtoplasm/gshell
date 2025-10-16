#ifndef CUSTOM_H
#define CUSTOM_H

#include <cstddef>
#include <fstream>

#include <ftxui/screen/color.hpp>
#include <map>
#include <string>
#include <unordered_map>
using std::string;

struct Customization {
  string confPath = "~/.config/gshell/config.conf";
  string tagline;

  string userUptime;
  string userProcessor;
  string userGpu;
  string userRam;
  string userCpu;

  ftxui::Color uptimeColor;
  ftxui::Color processorColor;
  ftxui::Color gpuColor;
  ftxui::Color ramGaugeColor;
  ftxui::Color cpuGaugeColor;

  string defaultTagLine = "Gshell";

  string defaultColorStr = "Magenta";
  ftxui::Color defaultColor = ftxui::Color::Magenta;

  std::map<string, string> confSettings = {
      {"tag_line", ""},  {"uptime_color", ""},   {"proc_color", ""},
      {"gpu_color", ""}, {"ramgauge_color", ""}, {"cpugauge_color", ""}};

  void getConfSettings() {
    string expandedPath = confPath;
    if (confPath[0] == '~') {
      const char *home = getenv("HOME");
      if (home) {
        expandedPath = string(home) + confPath.substr(1);
      }
    }

    std::ifstream confile(expandedPath);

    if (!confile.is_open()) {
      return;
    }

    string line;
    while (std::getline(confile, line)) {
      for (auto &[key, value] : confSettings) {
        if (line.find(key) != string::npos) {
          size_t delimiter = line.find('=');
          if (delimiter != string::npos) {
            value = line.substr(delimiter + 1);
          }
        }
      }
    }
    confile.close();
  }

  ftxui::Color ParseColors(const std::string &userColor) {
    static const std::unordered_map<std::string, ftxui::Color> colorMap = {
        {"CyanLight", ftxui::Color::CyanLight},
        {"CyanDark", ftxui::Color::DarkCyan},
        {"MagentaLight", ftxui::Color::MagentaLight},
        {"Magenta", ftxui::Color::Magenta},
        {"BlueLight", ftxui::Color::BlueLight},
        {"Blue", ftxui::Color::Blue},
        {"GreenLight", ftxui::Color::GreenLight},
        {"Green", ftxui::Color::Green},
        {"PinkLight", ftxui::Color::LightPink1},
        {"Pink", ftxui::Color::Pink1},
        {"RedLight", ftxui::Color::RedLight},
        {"Red", ftxui::Color::Red}};

    auto it = colorMap.find(userColor);
    return (it != colorMap.end()) ? it->second : ftxui::Color::Default;
  }

  void applyUserConf() {

    if (confSettings.at("tag_line").empty()) {
      tagline = defaultTagLine;
    } else {
      tagline = confSettings.at("tag_line");
    }

    if (confSettings.at("uptime_color").empty()) {
      userUptime = defaultColorStr;
    } else {
      userUptime = confSettings.at("uptime_color");
    }

    if (confSettings.at("proc_color").empty()) {
      userProcessor = defaultColorStr;
    } else {
      userProcessor = confSettings.at("proc_color");
    }

    if (confSettings.at("gpu_color").empty()) {
      userGpu = defaultColorStr;
    } else {
      userGpu = confSettings.at("gpu_color");
    }

    if (confSettings.at("ramgauge_color").empty()) {
      userRam = defaultColorStr;
    } else {
      userRam = confSettings.at("ramgauge_color");
    }
  }


};
#endif // CUSTOM_H
