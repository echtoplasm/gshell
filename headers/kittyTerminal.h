#ifndef KITTY_H
#define KITTY_H
#include <string>
#include <iostream>

struct KittyTerminalImg {
  static std::string gifPath;


  void displayGif(int x, int y, int width, int height) {
    std::cout << "\033[" << y << ";" << x << "H";
    std::string cmd = "kitty +kitten icat --place " + std::to_string(width) +
                      "x" + std::to_string(height) + "@" + std::to_string(x) +
                      "x" + std::to_string(y) + " " + gifPath;
    system(cmd.c_str());
  }
};
#endif
