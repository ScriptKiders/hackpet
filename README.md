# HackPet 🐾

A hardware hacking tool and educational platform for cybersecurity and embedded systems enthusiasts. HackPet combines powerful hardware with accessible examples to make learning security research and embedded development approachable and fun.

## Mission

HackPet is more than just another hacking tool - it's a learning platform with a mission to make cybersecurity and embedded systems education accessible to everyone. We believe in:

- **Open Learning**: Comprehensive, well-documented examples that teach by doing
- **Community Growth**: Building a welcoming space for beginners and experts alike
- **Hands-on Experience**: Real hardware projects that bridge theory and practice
- **Open Source**: Everything is transparent, modifiable, and free to learn from

## Inspiration

We stand on the shoulders of giants and are deeply grateful to the projects that inspired us:

- **[HackBat](https://github.com/controlpaths/hackbat)** - For showing what's possible with RP2040-based security tools
- **[Flipper Zero](https://github.com/flipperdevices/flipperzero-firmware)** - For proving that hacking tools can be accessible and educational

Thank you for paving the way and inspiring the next generation of hardware hackers! 🙏

## Hardware

**⚠️ Note: HackPet is currently in the conceptual/development phase. Hardware specifications may change.**

### Core Components

#### Microcontroller
- **RP2040** (Raspberry Pi's dual-core ARM Cortex-M0+ microcontroller)
- Future consideration: RP2350 for enhanced capabilities
- Clock speed: Up to 133 MHz
- 264KB SRAM
- Flexible GPIO with PIO (Programmable I/O) support

#### Display
- **ST7789** TFT LCD (240×320 pixels)
- RGB565 color format (65,536 colors)
- SPI interface for fast communication
- Custom lightweight graphics library
- Low power consumption

### Why These Choices?

- **RP2040**: Affordable, powerful, excellent community support, perfect for learning
- **ST7789**: Common, well-documented, great balance of features and simplicity

## Project Structure
```
hackpet/
├── docs/                    # Documentation and datasheets
│   └── ST7789 Datasheet.pdf
├── examples/                # Educational examples
│   └── rp2040-st7789/      # ST7789 display driver example
├── libs/                    # Third-party libraries
│   └── pico-sdk/           # Raspberry Pi Pico SDK (auto-downloaded)
├── setup.sh                # Automated setup script
├── .devcontainer/          # Docker dev environment
└── LICENSE
```

## Examples - Learn by Doing! 🚀

The `examples/` directory is the heart of HackPet's educational mission. Each example is:

- **Thoroughly documented**: Every line of code explained
- **Self-contained**: Easy to understand and modify
- **Practical**: Real-world applications, not just theory
- **Expandable**: Starting points for your own projects

### Current Examples

#### [rp2040-st7789](examples/rp2040-st7789/)
Learn how to interface with TFT LCD displays using SPI communication. This example includes:
- Complete ST7789 driver implementation
- Hardware SPI configuration
- RGB565 color handling
- Basic graphics primitives

**Perfect for**: Understanding display drivers, SPI communication, embedded graphics

### Coming Soon

We're actively developing new examples covering:
- USB HID device clone & emulation
- ESP32 WiFi packet sniffer

**Have an idea?** Open an issue, talk with us!

## Getting Started

### Prerequisites

- CMake (3.22 or higher)
- ARM GCC toolchain
- Git
- Make or Ninja

### Quick Setup

1. **Clone the repository**:
```bash
git clone https://github.com/ScriptKiders/hackpet
cd hackpet
```

2. **Run the setup script**:
```bash
bash setup.sh
```

This automatically downloads and configures the Pico SDK.

3. **Choose an example and build**:
```bash
cd examples/rp2040-st7789
mkdir build && cd build
cmake ..
make
```

4. **Upload to your Pico**:
- Hold BOOTSEL button on Pico
- Connect via USB
- Copy the `.uf2` file to the Pico drive

### Using Dev Containers (VSCode)

For a consistent development environment:

1. Open the project in VSCode
2. Click "Reopen in Container" when prompted
3. Everything is automatically configured!

## Contributing

At this time, HackPet is **not accepting external pull requests** as the project is in its early development phase. However, we greatly value community feedback!

**How you can help:**

- **Report bugs**: Found something broken? [Open an issue](https://github.com/ScriptKiders/hackpet/issues)
- **Share your projects**: Built something cool with HackPet? Let us know in an issue - we'd love to see it!

We may open contributions in the future as the project matures. For now, please use the issue tracker for all feedback and suggestions.

## Educational Use

HackPet is designed for educational purposes. We encourage its use in:

- University courses on embedded systems and security
- Workshops and hackathons
- Self-directed learning
- Security research labs

**Note**: Always use HackPet responsibly and ethically. Only test on systems you own or have explicit permission to test.

## License

This project is open source under the [LICENSE](LICENSE) file terms. Feel free to learn from it, modify it, and share your improvements!

## Authors

**dielburg** and **bluerabbit**!

---

**Remember**: With great power comes great responsibility. Use your skills to learn, build, and make the world more secure - never to cause harm.

**Happy hacking! 🚀🔐**