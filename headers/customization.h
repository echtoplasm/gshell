#ifndef CUSTOM_H
#define CUSTOM_H
#include "debug.h"
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <ftxui/screen/color.hpp>
#include <functional>
#include <map>
#include <string>
#include <unordered_map>

#include "kittyTerminal.h"

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
      {"tag_line", ""},       {"distro_color", ""},   {"color_tl", ""},
      {"uptime_color", ""},   {"proc_color", ""},     {"gpu_color", ""},
      {"ramgauge_color", ""}, {"cpugauge_color", ""}, {"gif_path", ""}};

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

  void createDefaultConfig(const std::string &path) {
    std::ofstream file(path);
    file << getDefaultConfigContent();
    file.close();
  }

    std::string getDefaultConfigContent() {
    return
        R"(# gshell configuration file
# Documentation: https://github.com/echtoplasm/gshell

# Available colors:
# Cyan: CyanLight, Cyan, CyanDark
# Magenta: MagentaLight, Magenta, MagentaDark
# Blue: BlueLight, Blue, BlueDark
# Green: GreenLight, Green, GreenDark
# Pink: PinkLight, Pink
# Red: RedLight, Red, RedDark
# Yellow: YellowLight, Yellow, YellowDark
# Orange: Orange, OrangeRed
# Purple: PurpleLight, Purple, PurpleDark
# Grayscale: White, GrayLight, Gray, GrayDark, Black
# Special: Gold, Coral, Turquoise, Salmon, Aquamarine

# Tagline text at the top (default: gshell)
tag_line = gshell 
# Tagline color
color_tl = CyanLight
# Uptime display color
uptime_color = Green
# Processor info color
proc_color = Blue
# GPU info color
gpu_color = Magenta
# RAM gauge color
ramgauge_color = BlueLight
# CPU gauge color
cpugauge_color = GreenLight
# Path to GIF file for display (optional but recommended, use absolute path)
gif_path = 
)";
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

  void applyKittyTermConf() {
    KittyTerminalImg::gifPath = confSettings["gif_path"];
  }

  void ensureConfigExists() {
    std::string homeDir = getenv("HOME");
    std::string configDir = homeDir + "/.config/gshell";
    std::string configPath = configDir + "/config.conf";
    if (std::filesystem::exists(configPath)) {
      return;
    }
    std::filesystem::create_directories(configDir);
    createDefaultConfig(configPath);
  }
};

// color map for property comparisons, this can be expanded for more colors
inline const std::unordered_map<std::string, ftxui::Color>
    Customization::colorMap = {
        // Cyan variants
        {"CyanLight", ftxui::Color::CyanLight},
        {"Cyan", ftxui::Color::Cyan},
        {"CyanDark", ftxui::Color::DarkCyan},

        // Magenta variants
        {"MagentaLight", ftxui::Color::MagentaLight},
        {"Magenta", ftxui::Color::Magenta},
        {"MagentaDark", ftxui::Color::DarkMagenta},

        // Blue variants
        {"BlueLight", ftxui::Color::BlueLight},
        {"Blue", ftxui::Color::Blue},
        {"BlueDark", ftxui::Color::DarkBlue},

        // Green variants
        {"GreenLight", ftxui::Color::GreenLight},
        {"Green", ftxui::Color::Green},
        {"GreenDark", ftxui::Color::DarkGreen},

        // Pink variants
        {"PinkLight", ftxui::Color::LightPink1},
        {"Pink", ftxui::Color::Pink1},

        // Red variants
        {"RedLight", ftxui::Color::RedLight},
        {"Red", ftxui::Color::Red},
        {"RedDark", ftxui::Color::DarkRed},

        // Yellow variants
        {"YellowLight", ftxui::Color::YellowLight},
        {"Yellow", ftxui::Color::Yellow},
        {"YellowDark", ftxui::Color::DarkOrange},

        // Orange variants
        {"Orange", ftxui::Color::Orange1},
        {"OrangeRed", ftxui::Color::OrangeRed1},

        // Purple variants
        {"Purple", ftxui::Color::Purple},
        {"PurpleLight", ftxui::Color::Violet},
        {"PurpleDark", ftxui::Color::DarkViolet},

        // Grayscale
        {"White", ftxui::Color::White},
        {"GrayLight", ftxui::Color::GrayLight},
        {"Gray", ftxui::Color::GrayDark},
        {"GrayDark", ftxui::Color::GrayDark},
        {"Black", ftxui::Color::Black},

        // Special colors
        {"Gold", ftxui::Color::Gold1},
        {"Coral", ftxui::Color::LightCoral},
        {"Turquoise", ftxui::Color::Turquoise2},
        {"Salmon", ftxui::Color::Salmon1},
        {"Aquamarine", ftxui::Color::Aquamarine1}};

#endif // CUSTOM_H
