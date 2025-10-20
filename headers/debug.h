#ifndef DEBUG_H
#define DEBUG_H

#include <fstream>
#include <iostream>

#define DEBUG_LOG(msg){ \
  std::ofstream log("gshellDebug.log", std::ios::app); \
  log << msg << std::endl; \
}

#endif
