# gshell

A lightweight, customizable terminal system monitor for Kitty with GIF support.

## Features

- **Real-time system monitoring** - CPU, RAM, GPU temperature, and uptime tracking
- **Kitty graphics integration** - Display custom GIFs directly in your terminal
- **Highly customizable** - Color schemes, display options, and personalization via config file
- **Lightweight & efficient** - Multi-threaded C++ implementation with minimal resource usage
- **Direct `/proc` filesystem access** - No external dependencies for system stats

## Installation

### From AUR (Arch Linux)

```bash
yay -S gshell
# or
paru -S gshell
```

### Building from Source

**Prerequisites:**
- Kitty terminal
- FTXUI library
- CMake 3.10+
- C++17 compatible compiler

```bash
git clone https://github.com/echtoplasm/gshell.git
cd gshell
mkdir build && cd build
cmake ..
make
sudo make install
```

## Usage

Simply run:
```bash
gshell
```

On first run, a default configuration file will be created at `~/.config/gshell/config.conf`.

## Configuration

Edit `~/.config/gshell/config.conf` to customize your setup:

```ini
# gshell configuration file
tag_line = gshell
color_tl = CyanLight
uptime_color = Green
proc_color = Blue
gpu_color = Magenta
ramgauge_color = BlueLight
cpugauge_color = GreenLight
```

### Available Colors

Cyan, Blue, Green, Red, Yellow, Magenta, Purple, Orange, Pink, Gold, Coral, Turquoise, Salmon, Aquamarine, White, Gray, Black (each with Light/Dark variants where applicable)

### Custom GIFs

Place your custom GIF at `~/.config/gshell/custom.gif` or specify a custom path in the config file.

## Documentation

For more information, see:
```bash
man gshell
```

Or visit the [GitHub repository](https://github.com/echtoplasm/gshell).

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## License

MIT License - see LICENSE file for details.

## Requirements

- **Kitty terminal** - Required for GIF rendering support
- Linux system with `/proc` filesystem

---

*Perfect for ricing your setup*
