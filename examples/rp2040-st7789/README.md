# ST7789 Display Example for RP2040

A simple, educational example demonstrating how to use the ST7789 TFT LCD display with Raspberry Pi Pico (RP2040). This project provides a clean, well-documented driver implementation and a basic color cycling demo.

## Features

- Clean C++ driver implementation for ST7789 displays
- Hardware SPI communication (32 MHz)
- RGB565 color format support
- Basic drawing primitives (pixels, rectangles, screen fills)
- Extensively commented code for educational purposes
- Simple color cycling demonstration
- Self-contained build system with automatic Pico SDK setup

## Hardware Requirements

- **Raspberry Pi Pico** (or any RP2040-based board)
- **ST7789 TFT LCD Display** (240×320 pixels)
- Breadboard and jumper wires
- USB cable for programming and power

## Wiring Diagram

Connect the ST7789 display to your Raspberry Pi Pico as follows:

| ST7789 Pin | Pico GPIO | Description |
|------------|-----------|-------------|
| VCC | 3.3V (OUT) | Power supply |
| GND | GND | Ground |
| CS | GPIO 17 | Chip Select (SPI) |
| DC | GPIO 16 | Data/Command select |
| RST | GPIO 20 | Reset (active low) |
| SCK | GPIO 18 | SPI Clock |
| MOSI | GPIO 19 | SPI Data (Master Out) |

**Note:** Make sure to use the 3.3V pin, not VBUS (5V), as the ST7789 operates at 3.3V logic levels.

## Building the Project

### Prerequisites

- CMake (version 3.22 or higher)
- ARM GCC toolchain
- Git
- Make or Ninja build system

**Note:** You do NOT need to manually install the Pico SDK - it's automatically set up by the project!

### Quick Start

#### Option 1: Using the setup script (Recommended)

From the project root directory:
```bash
# Run the setup script to download Pico SDK
bash setup.sh

# Navigate to this example
cd examples/rp2040-st7789

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
make
```

#### Option 2: Using Dev Container (VSCode)

If you're using Visual Studio Code with Docker:

1. Open the project root in VSCode
2. When prompted, click "Reopen in Container"
3. The Dev Container will automatically run `setup.sh` and set up everything
4. Navigate to the example and build:
```bash
cd examples/rp2040-st7789
mkdir build
cd build
cmake ..
make
```

### Build Instructions (Detailed)

1. **Setup Pico SDK** (first time only):
```bash
# From project root
bash setup.sh
```

This will:
- Create a `libs` directory
- Clone Pico SDK v2.2.0
- Initialize all required submodules

2. **Navigate to the example**:
```bash
cd examples/rp2040-st7789
```

3. **Create a build directory**:
```bash
mkdir build
cd build
```

4. **Configure the project with CMake**:
```bash
cmake ..
```

The CMake configuration will automatically find the Pico SDK at `../../libs/pico-sdk`.

5. **Build the project**:
```bash
make
```

6. The compiled `.uf2` file will be located at `build/st7789_example.uf2`.

## Project Structure

This example is part of the larger hackpet project:
```
hackpet/
├── docs/
│   └── ST7789 Datasheet.pdf    # Official ST7789 datasheet
├── examples/
│   └── rp2040-st7789/          # This example
│       ├── main.cpp             # Main program with color cycling demo
│       ├── st7789.h             # ST7789 driver header file
│       ├── st7789.cpp           # ST7789 driver implementation
│       ├── CMakeLists.txt       # Build configuration
│       └── build/               # Build output directory
├── libs/
│   └── pico-sdk/                # Pico SDK (auto-downloaded by setup.sh)
├── setup.sh                     # Pico SDK setup script
├── .devcontainer/               # Dev Container configuration
│   ├── devcontainer.json
│   └── Dockerfile
└── LICENSE
```

## Uploading to Pico

1. Hold down the **BOOTSEL** button on your Pico
2. Connect it to your computer via USB (while holding BOOTSEL)
3. Release the BOOTSEL button - the Pico will appear as a USB drive
4. Copy `build/st7789_example.uf2` to the Pico drive
5. The Pico will automatically reboot and start running the program

## What the Example Does

The example program cycles through eight different colors on the display, changing every second:

1. Red
2. Green
3. Blue
4. Yellow
5. Magenta
6. Cyan
7. White
8. Orange

Each color change is also logged via USB serial output for debugging purposes.

## Code Structure

### Key Components

- **`ST7789` class**: Complete driver for the display with initialization and drawing methods
- **`fillScreen()`**: Fill entire display with a single color
- **`fillRect()`**: Draw filled rectangles
- **`drawPixel()`**: Draw individual pixels
- **RGB565 color definitions**: Pre-defined color constants

## Customization

### Changing Pin Connections

Edit the pin definitions in `main.cpp`:
```cpp
#define PIN_CS   17  // Chip Select
#define PIN_DC   16  // Data/Command
#define PIN_RST  20  // Reset
#define PIN_SCK  18  // SPI Clock
#define PIN_MOSI 19  // SPI MOSI
```

### Adjusting SPI Speed

Modify the baudrate in `main.cpp`:
```cpp
#define SPI_BAUDRATE (32 * 1000 * 1000)  // 32 MHz (default)
```

Lower speeds (e.g., 16 MHz) may improve stability with longer wires.

### Creating Custom Colors

Use the RGB565 format to create custom colors:
```cpp
// RGB565: 5 bits Red, 6 bits Green, 5 bits Blue
uint16_t myColor = (R << 11) | (G << 5) | B;

// Example: Purple (R=20, G=10, B=25)
uint16_t purple = (20 << 11) | (10 << 5) | 25;
```

Or use the provided color constants from `st7789.h`:
- `COLOR_BLACK`, `COLOR_WHITE`
- `COLOR_RED`, `COLOR_GREEN`, `COLOR_BLUE`
- `COLOR_YELLOW`, `COLOR_MAGENTA`, `COLOR_CYAN`
- `COLOR_ORANGE`

## Serial Debug Output

Connect to the Pico's USB serial port to see debug messages:
```
ST7789 Display Example Starting...
Hardware: Raspberry Pi Pico + ST7789 LCD
Display initialized! (240x320 pixels)
SPI baudrate: 32000000 Hz
Displaying color: 0xF800
Displaying color: 0x07E0
...
```

On Linux/macOS:
```bash
screen /dev/ttyACM0 115200
```

On Windows, use PuTTY or the Arduino Serial Monitor.

## Troubleshooting

### "Pico SDK NOT FOUND!" error

Make sure you've run the setup script from the project root:
```bash
cd ../../  # Go to project root
bash setup.sh
```

### Display shows nothing or wrong colors

- Double-check all wiring connections
- Ensure 3.3V power supply is stable
- Try reducing SPI baudrate to 16 MHz
- Verify your display uses ST7789 controller (not ST7735 or ILI9341)

### Display shows inverted/incorrect colors

Some ST7789 displays have different color inversion settings. Try commenting out this line or change in `st7789.cpp`:
```cpp
writeCommand(ST7789_INVON);  // Try commenting this out or change to INVOFF
```

### Build errors

- Ensure you've run `setup.sh` to download the Pico SDK
- Verify CMake version is 3.22 or higher: `cmake --version`
- Try deleting the `build` directory and rebuilding from scratch

### Git submodule errors

If you get submodule errors, manually initialize them:
```bash
cd ../../libs/pico-sdk
git submodule update --init
```

## API Reference

### Initialization
```cpp
ST7789 display(spi0, PIN_CS, PIN_DC, PIN_RST, PIN_SCK, PIN_MOSI);
display.init(32000000);  // Initialize with 32 MHz SPI
```

### Drawing Functions
```cpp
// Fill entire screen
display.fillScreen(COLOR_RED);

// Draw filled rectangle
display.fillRect(x, y, width, height, COLOR_BLUE);

// Draw single pixel
display.drawPixel(x, y, COLOR_GREEN);
```

## Performance Notes

- **`fillScreen()`**: ~40ms at 32 MHz (entire 240×320 screen)
- **`drawPixel()`**: Very slow for multiple pixels - use `fillRect()` instead
- **SPI overhead**: Each transaction has setup overhead; batch operations when possible

## Development Environment

This project supports two development approaches:

### 1. Local Development
- Run `setup.sh` to download dependencies
- Use your preferred editor/IDE
- Build from command line with CMake

### 2. Dev Container (VSCode)
- Automatically sets up complete development environment
- Includes all necessary tools and extensions
- Consistent environment across different machines
- Simply open in VSCode and select "Reopen in Container"

## License

This example is part of the hackpet project. See the main repository LICENSE file for details.

## Author

**dielburg**  
Created: October 26, 2025

## Further Development

This is a basic example to get started. Consider adding:

- Text rendering capabilities
- Image/sprite support
- Buffered drawing for smoother animations
- Touch screen support (if your display has it)
- More complex graphics primitives (lines, circles, etc.)

## Resources

- [ST7789 Datasheet](../../docs/ST7789-Datasheet.pdf) - Official datasheet included in this repository
- [Pico SDK Documentation](https://www.raspberrypi.com/documentation/pico-sdk/)
- [RGB565 Color Picker](http://www.barth-dev.de/online/rgb565-color-picker/)

---

**Happy hacking! 🚀**