/**
 * st7789.cpp
 * Implementation of ST7789 display driver
 * dielburg
 * 01/10/2025
 */

#include "st7789.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

/**
 * Constructor implementation
 * 
 * 
 * Initializes member variables with pin numbers. Does not configure
 * any hardware - that happens in init().
 */
ST7789::ST7789(spi_inst_t* spi, uint8_t cs, uint8_t dc, uint8_t rst, 
               uint8_t sck, uint8_t mosi) 
    : _spi(spi), _cs(cs), _dc(dc), _rst(rst), _sck(sck), _mosi(mosi) {
    // Member initializer list handles all assignments
}

/**
 * @Send command to display
 * 
 * 
 * The ST7789 uses the DC (Data/Command) pin to distinguish between
 * commands and data:
 * - DC LOW = Command byte
 * - DC HIGH = Data byte
 * 
 * The CS (Chip Select) pin must be toggled for each transaction.
 */
void ST7789::writeCommand(uint8_t cmd) {
    gpio_put(_dc, 0);  // DC LOW = Command mode
    gpio_put(_cs, 0);  // CS LOW = Start transaction
    spi_write_blocking(_spi, &cmd, 1);  // Send command byte
    gpio_put(_cs, 1);  // CS HIGH = End transaction
}

/**
 * Send single data byte to display
 * 
 * 
 * Same as writeCommand() but with DC HIGH to indicate data.
 * Used for command parameters and single-byte data.
 */
void ST7789::writeData(uint8_t data) {
    gpio_put(_dc, 1);  // DC HIGH = Data mode
    gpio_put(_cs, 0);  // CS LOW = Start transaction
    spi_write_blocking(_spi, &data, 1);  // Send data byte
    gpio_put(_cs, 1);  // CS HIGH = End transaction
}

/**
 * Send multiple data bytes to display
 * 
 * 
 * More efficient than calling writeData() multiple times because
 * CS is only toggled once for the entire buffer. This reduces
 * overhead significantly for large transfers.
 */
void ST7789::writeDataBuf(const uint8_t* buf, size_t len) {
    gpio_put(_dc, 1);  // DC HIGH = Data mode
    gpio_put(_cs, 0);  // CS LOW = Start transaction
    spi_write_blocking(_spi, buf, len);  // Send all bytes
    gpio_put(_cs, 1);  // CS HIGH = End transaction
}

/**
 * Configure drawing window on display
 * 
 * 
 * The ST7789 has an internal address counter that automatically
 * increments after each pixel write. By setting CASET and RASET,
 * we define the boundaries where pixels will be drawn.
 * 
 * After calling this function, any data sent via RAMWR will be
 * written sequentially within the defined window, wrapping to
 * the next row when reaching the right edge.
 * 
 * Each coordinate is sent as 2 bytes (16-bit big-endian)
 */
void ST7789::setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    // Column Address Set (X coordinates)
    writeCommand(ST7789_CASET);
    writeData(x0 >> 8);        // X start high byte
    writeData(x0 & 0xFF);      // X start low byte
    writeData(x1 >> 8);        // X end high byte
    writeData(x1 & 0xFF);      // X end low byte
    
    // Row Address Set (Y coordinates)
    writeCommand(ST7789_RASET);
    writeData(y0 >> 8);        // Y start high byte
    writeData(y0 & 0xFF);      // Y start low byte
    writeData(y1 >> 8);        // Y end high byte
    writeData(y1 & 0xFF);      // Y end low byte
    
    // Prepare for pixel data
    writeCommand(ST7789_RAMWR);
}

/**
 * Initialize display hardware
 * 
 * 
 * Initialization sequence follows ST7789 datasheet recommendations:
 * 
 * 1. SPI Configuration:
 *    - Initializes SPI at specified baud rate (typically 32 MHz)
 *    - Configures SCK and MOSI pins as SPI functions
 * 
 * 2. GPIO Setup:
 *    - CS, DC, RST as outputs
 * 
 * 3. Hardware Reset:
 *    - RST HIGH → LOW → HIGH sequence
 *    - Clears display internal state
 * 
 * 4. Software Reset:
 *    - SWRESET command
 *    - Additional safety measure
 * 
 * 5. Display Configuration:
 *    - SLPOUT: Exit sleep mode (required for operation)
 *    - COLMOD: Set to 16-bit RGB565 format (0x05)
 *    - MADCTL: Set rotation and mirroring (0x00 = no rotation)
 *    - INVON: Enable color inversion (improves color accuracy)
 *    - DISPON: Turn on display output
 * 
 */
void ST7789::init(uint32_t baudrate) {
    // ========== SPI INITIALIZATION ==========
    spi_init(_spi, baudrate);  // Configure SPI peripheral
    gpio_set_function(_sck, GPIO_FUNC_SPI);   // SCK as SPI clock
    gpio_set_function(_mosi, GPIO_FUNC_SPI);  // MOSI as SPI data out
    
    // ========== GPIO INITIALIZATION ==========
    gpio_init(_cs);   // Initialize CS pin
    gpio_init(_dc);   // Initialize DC pin
    gpio_init(_rst);  // Initialize RST pin
    gpio_set_dir(_cs, GPIO_OUT);   // CS as output
    gpio_set_dir(_dc, GPIO_OUT);   // DC as output
    gpio_set_dir(_rst, GPIO_OUT);  // RST as output
    
    // ========== HARDWARE RESET ==========
    gpio_put(_rst, 1);   // RST HIGH
    sleep_ms(100);       // Wait for stable power
    gpio_put(_rst, 0);   // RST LOW - trigger reset
    sleep_ms(100);       // Hold reset for 100ms
    gpio_put(_rst, 1);   // RST HIGH - release reset
    sleep_ms(100);       // Wait for display to initialize
    
    // ========== SOFTWARE RESET ==========
    writeCommand(ST7789_SWRESET);
    sleep_ms(150);  // SWRESET requires 120ms minimum
    
    // ========== EXIT SLEEP MODE ==========
    writeCommand(ST7789_SLPOUT);
    sleep_ms(120);  // SLPOUT requires 120ms minimum
    
    // ========== COLOR MODE CONFIGURATION ==========
    // Set to 16-bit RGB565 format
    // 0x55 = 16-bit/pixel (5-6-5 bit RGB)
    writeCommand(ST7789_COLMOD);
    writeData(0x55);
    
    // ========== MEMORY ACCESS CONTROL ==========
    // 0x00 = No rotation, no mirroring
    // Other values allow 90°/180°/270° rotation
    writeCommand(ST7789_MADCTL);
    writeData(0x00);
    
    // ========== COLOR INVERSION ==========
    // Some ST7789 displays require color inversion, others don't.
    // If colors appear inverted (white shows as black, red as cyan):
    // - Try ST7789_INVOFF (0x20) instead of ST7789_INVON (0x21)
    // - Or comment out this line entirely
    writeCommand(ST7789_INVOFF);
    
    // ========== DISPLAY ON ==========
    writeCommand(ST7789_DISPON);
    sleep_ms(100);  // Allow display to stabilize
}

/**
 * Fill entire screen with color
 * 
 * 
 * Simple wrapper that calls fillRect() with full screen dimensions.
 * Provided for convenience and code readability.
 */
void ST7789::fillScreen(uint16_t color) {
    fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, color);
}

/**
 * Fill rectangular area with color
 * 
 * 
 * Drawing process:
 * 1. Validate and clip coordinates to screen bounds
 * 2. Convert RGB565 color to 2-byte array (big-endian)
 * 3. Set drawing window to rectangle bounds
 * 4. Send color data for each pixel in the rectangle
 * 
 * RGB565 format: each pixel requires 2 bytes
 *       Byte 0: RRRRR GGG (red + green high bits)
 *       Byte 1: GGG BBBBB (green low bits + blue)
 * 
 * This function sends pixels one at a time via SPI.
 *          For large rectangles, this can be slow. A buffered
 *          version would be faster but requires more RAM.
 */
void ST7789::fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    // ========== BOUNDARY CHECKING ==========
    // Prevent drawing outside screen bounds
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return;
    
    // Clip rectangle if it extends beyond screen
    if (x + w > SCREEN_WIDTH) w = SCREEN_WIDTH - x;
    if (y + h > SCREEN_HEIGHT) h = SCREEN_HEIGHT - y;
    
    // ========== COLOR PREPARATION ==========
    // Convert 16-bit color to 2-byte array (big-endian format)
    uint8_t colorBuf[2];
    colorBuf[0] = color >> 8;      // High byte (RRRRR GGG)
    colorBuf[1] = color & 0xFF;    // Low byte (GGG BBBBB)
    
    // ========== SET DRAWING WINDOW ==========
    // Configure ST7789 to accept pixel data for this rectangle
    setWindow(x, y, x + w - 1, y + h - 1);
    
    // ========== PIXEL DATA TRANSMISSION ==========
    gpio_put(_dc, 1);  // DC HIGH = Data mode
    gpio_put(_cs, 0);  // CS LOW = Start transaction
    
    // Send color data for each pixel
    // Total pixels = width × height
    for (uint32_t i = 0; i < (uint32_t)w * h; i++) {
        spi_write_blocking(_spi, colorBuf, 2);
    }
    
    gpio_put(_cs, 1);  // CS HIGH = End transaction
}

/**
 * Draw single pixel
 * 
 * 
 * Sets a single pixel to specified color. This is the least efficient
 * drawing method because it requires full SPI transaction overhead
 * (set window + send 2 bytes) for just one pixel.
 * 
 * For multiple adjacent pixels, use fillRect() instead
 * 
 * Very slow for drawing many pixels. Consider buffering
 *          pixel data if performance is critical.
 */
void ST7789::drawPixel(uint16_t x, uint16_t y, uint16_t color) {
    // Boundary check - ignore out of bounds pixels
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return;
    
    // Convert color to 2-byte array
    uint8_t colorBuf[2];
    colorBuf[0] = color >> 8;      // High byte
    colorBuf[1] = color & 0xFF;    // Low byte
    
    // Set 1×1 pixel window and send color
    setWindow(x, y, x, y);
    writeDataBuf(colorBuf, 2);
}