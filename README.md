# gshell - System Monitor TUI

A lightweight terminal-based system monitor with anime flair. Because why should system monitoring be boring?

## About

A practice project for learning multi-threaded C++ systems programming while building something actually fun to look at. Combines real-time system stats with Kitty terminal graphics for that classic r/unixporn aesthetic.

## Features

- Real-time system statistics (RAM usage, uptime)
- Kitty terminal graphics integration - display GIFs alongside your stats
- Multi-threaded architecture for smooth updates
- Clean TUI built with FTXUI
- Reads directly from `/proc` filesystem

## Prerequisites

- **Kitty terminal** (for image rendering)
- **FTXUI library**
- **CMake** (3.10+)
- **C++17 compiler**

## Building

```bash
# Configure
cmake -B build

# Build
cmake --build build -j

# Run
./build/gshell
```

## What I Learned

- Multi-threaded programming with atomics and synchronization
- Parsing Linux `/proc` filesystem data
- Working with terminal graphics protocols
- Building responsive TUIs
- Real-time data updates without blocking

## Customization

Drop your own GIF in and change the path in `main.cpp`:

```cpp
gifDisplay.gifPath = "your-waifu-here.gif";
```

---

*A learning project for systems programming fundamentals (and showing off your terminal setup)*
