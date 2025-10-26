/**
 * st7789.h
 * ST7789 display driver library for Raspberry Pi Pico
 * dielburg
 * 01/10/2025
 * 
 * 
 * This file contains the ST7789 class definition which enables communication
 * with ST7789 LCD displays via SPI interface. The ST7789 is a popular
 * TFT LCD controller with 240x320 pixel resolution, using 16-bit RGB565
 * color format.
 * 
 *
 * The display requires the following connections:
 * - CS (Chip Select): selects the device on SPI bus
 * - DC (Data/Command): switches between command and data mode
 * - RST (Reset): hardware reset for the display
 * - SCK (Serial Clock): SPI clock signal
 * - MOSI (Master Out Slave In): data from Pico to display
 * 
 * example:
 * 
 * ST7789 display(spi0, 17, 16, 20, 18, 19);
 * display.init(32000000);
 * display.fillScreen(COLOR_RED);
 * 
 */

#ifndef ST7789_H
#define ST7789_H

#include <stdint.h>
#include "hardware/spi.h"

/**
 * ST7789Commands ST7789 Command Definitions
 * 
 * Command bytes for ST7789 display controller
 * 
 * 
 * These are the fundamental commands defined in the ST7789 datasheet.
 * Each command is sent via SPI with DC pin LOW, followed by parameters
 * (if any) with DC pin HIGH.
 * 
 */
#define ST7789_SWRESET   0x01  // < Software Reset - resets display internal state
#define ST7789_SLPOUT    0x11  // < Sleep Out - exits sleep mode
#define ST7789_COLMOD    0x3A  // < Color Mode - sets pixel format (RGB565, RGB666, etc.)
#define ST7789_MADCTL    0x36  // < Memory Access Control - rotation and mirroring
#define ST7789_CASET     0x2A  // < Column Address Set - defines X range
#define ST7789_RASET     0x2B  // < Row Address Set - defines Y range
#define ST7789_RAMWR     0x2C  // < RAM Write - starts pixel data transfer
#define ST7789_DISPON    0x29  // < Display On - turns on the display
#define ST7789_INVON     0x21  // < Inversion On - inverts display colors for better quality
#define ST7789_INVOFF    0x20  // < Inversion Off - disables color inversion

/**
 * Constants defining the physical display resolution
 */
#define SCREEN_WIDTH  240  // < Screen width in pixels
#define SCREEN_HEIGHT 320  // < Screen height in pixels

/**
 * 16-bit RGB565 format colors
 * 
 * RGB565 format consists of:
 * - 5 bits for Red (R) - bits 15-11
 * - 6 bits for Green (G) - bits 10-5
 * - 5 bits for Blue (B) - bits 4-0
 * 
 */
#define COLOR_BLACK   0x0000  ///< Black (R=0, G=0, B=0)
#define COLOR_WHITE   0xFFFF  ///< White (R=31, G=63, B=31)
#define COLOR_RED     0xF800  ///< Red (R=31, G=0, B=0)
#define COLOR_GREEN   0x07E0  ///< Green (R=0, G=63, B=0)
#define COLOR_BLUE    0x001F  ///< Blue (R=0, G=0, B=31)
#define COLOR_YELLOW  0xFFE0  ///< Yellow (R=31, G=63, B=0)
#define COLOR_MAGENTA 0xF81F  ///< Magenta (R=31, G=0, B=31)
#define COLOR_CYAN    0x07FF  ///< Cyan (R=0, G=63, B=31)
#define COLOR_ORANGE  0xFD20  ///< Orange (R=31, G=40, B=0)

/**
 * Driver class for ST7789 TFT LCD display
 * 
 * 
 * This class provides methods to initialize and control an ST7789 display
 * via SPI interface. It supports basic drawing operations like filling
 * the screen, drawing rectangles, and individual pixels.
 * 
 * The class uses hardware SPI for fast communication and supports
 * configurable SPI ports (spi0 or spi1) and baud rates.
 */
class ST7789 {
public:
    /**
     * Constructor - creates ST7789 display object
     * 
     * spi Pointer to SPI instance (spi0 or spi1)
     * cs Chip Select pin number
     * dc Data/Command pin number
     * rst Reset pin number
     * sck SPI Clock pin number
     * mosi SPI MOSI (Master Out Slave In) pin number
     * 
     * This only stores the pin configuration. Call init() to
     *       actually initialize the hardware and display.
     * 
     */
    ST7789(spi_inst_t* spi, uint8_t cs, uint8_t dc, uint8_t rst, 
           uint8_t sck, uint8_t mosi);
    
    /**
     * Initialize the display hardware and SPI interface
     * 
     * baudrate SPI baud rate in Hz (default: 32 MHz)
     * 
     * 
     * This function performs the following initialization sequence:
     * 1. Configures SPI interface with specified baud rate
     * 2. Sets up GPIO pins for CS, DC, and RST
     * 3. Performs hardware reset by toggling RST pin
     * 4. Sends initialization commands to ST7789:
     *    - Software reset (SWRESET)
     *    - Exit sleep mode (SLPOUT)
     *    - Set color mode to RGB565 (COLMOD)
     *    - Configure memory access (MADCTL)
     *    - Enable color inversion (INVON)
     *    - Turn on display (DISPON)
     * 
     * Must be called before any drawing operations
     * 
     * Delays are included for display stability. Do not remove them.
     * 
     */
    void init(uint32_t baudrate = 32000000);
    
    /**
     * Fill entire screen with specified color
     * 
     * color RGB565 color value (use COLOR_* macros)
     * 
     * 
     * This is a convenience function that fills the entire screen
     * by calling fillRect() with full screen dimensions.
     * 
     */
    void fillScreen(uint16_t color);
    
    /**
     * Fill rectangular area with specified color
     * 
     * x X coordinate of top-left corner (0-239)
     * y Y coordinate of top-left corner (0-319)
     * w Width of rectangle in pixels
     * h Height of rectangle in pixels
     * color RGB565 color value
     * 
     * 
     * Draws a filled rectangle at specified position. Automatically clips
     * the rectangle if it extends beyond screen boundaries. Uses SPI
     * to send color data pixel by pixel.
     * 
     * Coordinates outside screen bounds are ignored
     * 
     * Large rectangles may take significant time to draw.
     *          For full screen, use fillScreen() instead.
     * 
     */
    void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
    
    /**
     * Draw single pixel at specified position
     * 
     * x X coordinate (0-239)
     * y Y coordinate (0-319)
     * color RGB565 color value
     * 
     * 
     * Sets a single pixel to the specified color. This is the slowest
     * drawing method as it requires full SPI transaction overhead for
     * just one pixel.
     * 
     * For drawing multiple pixels, use fillRect() instead
     * 
     */
    void drawPixel(uint16_t x, uint16_t y, uint16_t color);
    
private:
    spi_inst_t* _spi;  // < Pointer to SPI instance (spi0 or spi1)
    uint8_t _cs;       // < Chip Select pin number
    uint8_t _dc;       // < Data/Command pin number
    uint8_t _rst;      // < Reset pin number
    uint8_t _sck;      // < SPI Clock pin number
    uint8_t _mosi;     // < SPI MOSI pin number
    
    /**
     * Send command byte to display
     * 
     * cmd Command byte to send
     * 
     * 
     * Sets DC pin LOW (command mode), pulls CS LOW, sends byte via SPI,
     * then pulls CS HIGH. This is the fundamental method for sending
     * ST7789 commands.
     * 
     * This is a private method used internally by public functions
     */
    void writeCommand(uint8_t cmd);
    
    /**
     * Send single data byte to display
     * 
     * data Data byte to send
     * 
     * 
     * Sets DC pin HIGH (data mode), pulls CS LOW, sends byte via SPI,
     * then pulls CS HIGH. Used for sending parameters to commands.
     * 
     * This is a private method used internally by public functions
     */
    void writeData(uint8_t data);
    
    /**
     * Send multiple data bytes to display
     * 
     * buf Pointer to data buffer
     * len Number of bytes to send
     * 
     * 
     * Similar to writeData() but optimized for sending multiple bytes
     * in a single CS cycle. More efficient than calling writeData()
     * repeatedly.
     * 
     * This is a private method used internally by public functions
     */
    void writeDataBuf(const uint8_t* buf, size_t len);
    
    /**
     * Set drawing window (region of interest)
     * 
     * x0 Start X coordinate
     * y0 Start Y coordinate
     * x1 End X coordinate (inclusive)
     * y1 End Y coordinate (inclusive)
     * 
     * 
     * Configures the ST7789's internal address counter to define
     * a rectangular region. All subsequent pixel data will be written
     * to this region, wrapping automatically.
     * 
     * Uses CASET (Column Address Set) and RASET (Row Address Set)
     * commands followed by RAMWR (RAM Write) to prepare for data.
     * 
     * Coordinates are inclusive: (0,0)-(239,319) covers entire screen
     * This is a private method used internally by public functions
     */
    void setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
};

#endif // ST7789_H