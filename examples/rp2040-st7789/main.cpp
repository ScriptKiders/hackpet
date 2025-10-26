/**
 * main.cpp
 * Simple example demonstrating ST7789 display usage
 * dielburg
 * 01/10/2025
 * 
 * 
 * This example initializes an ST7789 display and cycles through
 * different colors every second. It demonstrates:
 * - Display initialization
 * - Full screen color filling
 * - Basic color cycling animation
 * 
 * 
 * Hardware connections required:
 * - GPIO 17 → CS (Chip Select)
 * - GPIO 16 → DC (Data/Command)
 * - GPIO 20 → RST (Reset)
 * - GPIO 18 → SCK (SPI Clock)
 * - GPIO 19 → MOSI (SPI Data)
 * - 3.3V → VCC
 * - GND → GND
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "st7789.h"

/**
 * 
 * Pin numbers for ST7789 connections
 */
#define PIN_CS   17  // < Chip Select - active low
#define PIN_DC   16  // < Data/Command - LOW=command, HIGH=data
#define PIN_RST  20  // < Reset - active low
#define PIN_SCK  18  // < SPI Clock
#define PIN_MOSI 19  // < SPI Master Out Slave In

/**
 * 
 * SPI interface settings
 */
#define SPI_PORT spi0                    // < SPI peripheral instance (spi0 or spi1)
#define SPI_BAUDRATE (32 * 1000 * 1000)  // < SPI speed: 32 MHz

/**
 * Program flow:
 * 1. Initialize USB serial output
 * 2. Create ST7789 display object
 * 3. Initialize display hardware
 * 4. Enter infinite loop cycling through colors
 */
int main() {
    // ========== SERIAL INITIALIZATION ==========
    stdio_init_all();  // Initialize USB serial for debugging
    
    printf("ST7789 Display Example Starting...\n");
    printf("Hardware: Raspberry Pi Pico + ST7789 LCD\n");
    
    // ========== DISPLAY INITIALIZATION ==========
    // Create display object with pin configuration
    ST7789 display(SPI_PORT, PIN_CS, PIN_DC, PIN_RST, PIN_SCK, PIN_MOSI);
    
    // Initialize hardware (SPI + display controller)
    display.init(SPI_BAUDRATE);
    
    printf("Display initialized! (%dx%d pixels)\n", SCREEN_WIDTH, SCREEN_HEIGHT);
    printf("SPI baudrate: %d Hz\n", SPI_BAUDRATE);
    
    // ========== COLOR ARRAY ==========
    /**
     * Array of colors for cycling animation
     * 
     * Each color is in RGB565 format (16-bit):
     * - 5 bits Red (0-31)
     * - 6 bits Green (0-63)
     * - 5 bits Blue (0-31)
     */
    const uint16_t colors[] = {
        COLOR_RED,      // Pure red
        COLOR_GREEN,    // Pure green
        COLOR_BLUE,     // Pure blue
        COLOR_YELLOW,   // Red + Green
        COLOR_MAGENTA,  // Red + Blue
        COLOR_CYAN,     // Green + Blue
        COLOR_WHITE,    // All colors
        COLOR_ORANGE    // Red + partial Green
    };
    const int numColors = sizeof(colors) / sizeof(colors[0]);
    
    // ========== MAIN LOOP ==========
    /**
     * Each iteration:
     * 1. Fill screen with current color
     * 2. Print color value to serial
     * 3. Advance to next color (wraps around)
     * 4. Wait 1 second
     */
    int colorIndex = 0;
    while (true) {
        // Fill entire screen with current color
        display.fillScreen(colors[colorIndex]);
        
        // Log current color to serial (hex format)
        printf("Displaying color: 0x%04X\n", colors[colorIndex]);
        
        // Move to next color (wrap around using modulo)
        colorIndex = (colorIndex + 1) % numColors;
        
        // Wait 1 second before next color
        sleep_ms(1000);
    }
    
    return 0;
}