#ifndef THERMAL_H
#define THERMAL_H

#include <fstream>

using std::string;

struct Thermal {
  string thermalPath = "/sys/class/thermal/thermal_zone1/temp";
  string rawTemp;
  int rawTempDouble;
  int tempC;
  double tempF;
  
  double rawTempConv(string rawT){
    return std::stoi(rawT);
  }

  void getTemp0(){
    std::ifstream temp0(thermalPath);
    string line;
    string rawTemp;

    while(std::getline(temp0, line)){
      rawTemp = line;
   }
    temp0.close();
    
    rawTempDouble = rawTempConv(rawTemp);
    tempC = rawTempDouble / 1000;
  }
};
#endif
