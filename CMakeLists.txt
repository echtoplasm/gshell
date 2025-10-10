cmake_minimum_required(VERSION 3.10)
project(gshell)

set(CMAKE_CXX_STANDARD 17)

find_package(ftxui REQUIRED)

add_executable(gshell main.cpp)
target_link_libraries(gshell 
    ftxui::screen
    ftxui::dom
    ftxui::component
)
