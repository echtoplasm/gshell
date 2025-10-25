#ifndef CUSTOM_H
#define CUSTOM_H
#include "debug.h"
#include <cstddef>
#include <fstream>
#include <ftxui/screen/color.hpp>
#include <functional>
#include <map>
#include <string>
#include <unordered_map>

using std::string;

struct Customization {

  // Handles user configuration from ~/.config/gshell/config.conf
  // Flow: getConfSettings() -> reads file into confSettings map
  //       applyUserConf() -> parses colors and assigns to struct properties
  //       parseColor() -> helper to convert color strings to ftxui::Color

  string confPath = "~/.config/gshell/config.conf";
  string tagline;
  ftxui::Color taglineColor, distroColor, uptimeColor, processorColor, gpuColor,
      ramGaugeColor, cpuGaugeColor;

  string defaultTagLine = "Gshell";
  ftxui::Color defaultColor = ftxui::Color::Magenta;

  // map has been init'd with empty strings to allow for user defined
  // colors in the values
  std::map<string, string> confSettings = {
      {"tag_line", ""},       {"distro_color", ""},  {"color_tl", ""},
      {"uptime_color", ""},   {"proc_color", ""},    {"gpu_color", ""},
      {"ramgauge_color", ""}, {"cpugauge_color", ""}};

  // color lookup map, the map is defined outside the struct
  static const std::unordered_map<std::string, ftxui::Color> colorMap;

  // iterating through the conf file and setting confsettings values to
  // user defined colors
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
            value.erase(0, value.find_first_not_of(" \t\n\r"));
            value.erase(value.find_last_not_of(" \t\n\r") + 1);
          }
        }
      }
    }
    confile.close();

    DEBUG_LOG("Config loaded successfully");
    for (const auto &pair : confSettings) {
      DEBUG_LOG(" " << pair.first << " = " << pair.second);
    }
  }

  // helper function to parse color string and assign it to ftxui color
  ftxui::Color parseColor(const string &colorStr) {
    auto it = colorMap.find(colorStr);
    return (it != colorMap.end()) ? it->second : defaultColor;
  }

  void applyUserConf() {
    // setting the tagline color
    tagline = confSettings["tag_line"].empty() ? defaultTagLine
                                               : confSettings["tag_line"];

    // mapping config keys to color properties using lambdas
    std::map<string, std::function<void(ftxui::Color)>> colorSetters = {
        {"color_tl", [this](ftxui::Color c) { taglineColor = c; }},
        {"distro_color", [this](ftxui::Color c) { distroColor = c; }},
        {"uptime_color", [this](ftxui::Color c) { uptimeColor = c; }},
        {"proc_color", [this](ftxui::Color c) { processorColor = c; }},
        {"gpu_color", [this](ftxui::Color c) { gpuColor = c; }},
        {"ramgauge_color", [this](ftxui::Color c) { ramGaugeColor = c; }},
        {"cpugauge_color", [this](ftxui::Color c) { cpuGaugeColor = c; }}};

    // iterating through and setting the colors.
    for (const auto &[key, setter] : colorSetters) {
      string colorStr = confSettings[key];
      ftxui::Color color =
          colorStr.empty() ? defaultColor : parseColor(colorStr);
      setter(color);
    }
  }
};

// color map for property comparisons, this can be expanded for more colors
inline const std::unordered_map<std::string, ftxui::Color>
    Customization::colorMap = {{"CyanLight", ftxui::Color::CyanLight},
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

#endif // CUSTOM_H
