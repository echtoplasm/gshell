#ifndef CUSTOM_H
#define CUSTOM_H

#include <cstddef>
#include <fstream>
#include <map>
#include <string>

using std::string;

struct Customization {
  string confPath = "~/.config/gshell/config.conf";
  string tagline;
  string uptimeColor;
  string processorColor;
  string gpuColor;
  string ramGaugeColor;

  string defaultTagLine = "Gshell";
  string defaultColor = "Magenta";

  std::map<string, string> confSettings = {{"tag_line", ""},
                                           {"uptime_color", ""},
                                           {"proc_color", ""},
                                           {"gpu_color", ""},
                                           {"ramgauge_color", ""}};

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
      for (auto& [key, value] : confSettings) {
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

  void applySettings() {

    if (confSettings.at("tag_line").empty()) {
      tagline = defaultTagLine;
    } else {
      tagline = confSettings.at("tag_line");
    }

    if (confSettings.at("uptime_color").empty()) {
      uptimeColor = confSettings.at("uptime_color");
    } else {
      uptimeColor = defaultColor;
    }

    if (confSettings.at("proc_color").empty()) {
      processorColor = confSettings.at("proc_color");
    } else {
      processorColor = defaultColor;
    }

    if (confSettings.at("gpu_color").empty()) {
      gpuColor = confSettings.at("gpu_color");
    } else {
      gpuColor = defaultColor;
    }

    if (confSettings.at("ramgauge_color").empty()) {
      ramGaugeColor = confSettings.at("ramgauge_color");
    } else {
      ramGaugeColor = defaultColor;
    }
  }
};

#endif // CUSTOM_H
