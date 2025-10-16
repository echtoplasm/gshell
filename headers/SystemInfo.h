#ifndef SYS_INFO_H
#define SYS_INFO_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

struct SystemInfo {
  double uptime_seconds;
  double idle_seconds;
  float ram_percent;
  float cpu_percent;
  std::string processorName;
  std::string gpuName;
  std::string distroName;

  std::string getDistroName() {
    std::ifstream distroinfo("/etc/os-release");
    std::string line;
    std::string distroName;

    while (std::getline(distroinfo, line)) {
      if (line.find("PRETTY_NAME") != std::string::npos) {
        size_t delimiter = line.find('=');
        if (delimiter != std::string::npos) {
          distroName = line.substr(delimiter + 1);
          distroName.erase(
              std::remove(distroName.begin(), distroName.end(), '"'),
              distroName.end());
        } else {
          distroName = "Unknown Distro";
        }
      }
    }
    distroinfo.close();
    return distroName;
  }

  std::string getProcessorInfo() {
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    std::string processorName;

    while (std::getline(cpuinfo, line)) {
      if (line.find("model name") != std::string::npos) {
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
          processorName = line.substr(colonPos + 2);
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

    if (distroName.empty()) {
      distroName = getDistroName();
    }

    ram_percent = 100.0f * (1.0f - (float)mem_available / mem_total);

    std::ifstream cpuVals; // start here to calc cpuvals

    uptime.close();
    meminfo.close();
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
#endif // SYS_INFO_H
