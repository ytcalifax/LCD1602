/**
 * @file LCD1602_RGB.h
 * @brief LCD1602 RGB I2C display driver with buffer and optimized writes.
 *
 * This library provides an interface for controlling an LCD1602 display with RGB backlight over I2C.
 * It features a display buffer, dirty tracking for optimized updates, and various display utilities.
 *
 * @author
 * @date 2025-06-24
 */
#ifndef __LCD1602_H__
#define __LCD1602_H__

#include "Print.h"
#include <inttypes.h>

// Device I2C Address
#define LCD_ADDRESS     (0x7c >> 1)
#define RGB_ADDRESS     (0xc0 >> 1)

// Color definitions
#define WHITE           0
#define RED             1
#define GREEN           2
#define BLUE            3

#define REG_RED         0x04
#define REG_GREEN       0x03
#define REG_BLUE        0x02

#define REG_MODE1       0x00
#define REG_MODE2       0x01
#define REG_OUTPUT      0x08
#define REG_BLINK       0x07
#define REG_BREATH      0x06

// LCD commands
#define LCD_CLEARDISPLAY   0x01
#define LCD_RETURNHOME     0x02
#define LCD_ENTRYMODESET   0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT    0x10
#define LCD_FUNCTIONSET    0x20
#define LCD_SETCGRAMADDR   0x40
#define LCD_SETDDRAMADDR   0x80

// Flags for display entry mode
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// Flags for display on/off control
#define LCD_DISPLAYON  0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON   0x02
#define LCD_CURSOROFF  0x00
#define LCD_BLINKON    0x01
#define LCD_BLINKOFF   0x00

// Flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE  0x00
#define LCD_MOVERIGHT   0x04
#define LCD_MOVELEFT    0x00

// Flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE    0x08
#define LCD_1LINE    0x00
#define LCD_5x8DOTS  0x00

/**
 * @class LCD1602
 * @brief Class for controlling an LCD1602 RGB I2C display.
 */
class LCD1602 {
public:
    /**
     * @brief Construct a new LCD1602 object
     * @param lcd_cols Number of columns of the LCD
     * @param lcd_rows Number of rows of the LCD
     */
    LCD1602(uint8_t lcd_cols, uint8_t lcd_rows);

    /** @brief Initialize the display */
    void init();
    /** @brief Return cursor to home position */
    void home();
    /** @brief Turn on the display */
    void display();
    /** @brief Send a command to the LCD */
    void command(uint8_t value);
    /** @brief Send raw data to the LCD */
    void send(uint8_t *data, uint8_t len);
    /** @brief Set a register on the RGB controller */
    void setReg(uint8_t addr, uint8_t data) const;
    /** @brief Set the RGB backlight color */
    void setRGB(uint8_t r, uint8_t g, uint8_t b);
    /** @brief Set the cursor position */
    void setCursor(uint8_t col, uint8_t row);
    /** @brief Clear the display */
    void clear();
    /** @brief Enable LED blinking */
    void BlinkLED();
    /** @brief Disable LED blinking */
    void noBlinkLED();
    /** @brief Write a character at the current cursor */
    void write_char(uint8_t value);
    /** @brief Write a string starting at the current cursor */
    void send_string(const char *str);
    /** @brief Define a custom symbol */
    void customSymbol(uint8_t location, const uint8_t charmap[8]);
    /** @brief Stop display blinking */
    void stopBlink();
    /** @brief Start display blinking */
    void blink();
    /** @brief Hide the cursor */
    void noCursor();
    /** @brief Show the cursor */
    void cursor();
    /** @brief Scroll display left */
    void scrollDisplayLeft();
    /** @brief Scroll display right */
    void scrollDisplayRight();
    /** @brief Set text direction left-to-right */
    void leftToRight();
    /** @brief Set text direction right-to-left */
    void rightToLeft();
    /** @brief Disable autoscroll */
    void noAutoscroll();
    /** @brief Enable autoscroll */
    void autoscroll();
    /** @brief Set backlight to white */
    void setColorWhite() { setRGB(255, 255, 255); }
    /**
     * @brief Update only changed characters on the LCD
     *
     * Call this after making changes to the buffer to update the physical display efficiently.
     */
    void refresh();

private:
    /** @brief Initialize the display hardware */
    void begin(uint8_t cols, uint8_t rows);
    uint8_t _showfunction = 0;
    uint8_t _showcontrol = 0;
    uint8_t _showmode = 0;
    uint8_t _initialized = 0;
    uint8_t _numlines = 0, _currline = 0;
    uint8_t _lcdAddr = LCD_ADDRESS;
    uint8_t _RGBAddr = RGB_ADDRESS;
    uint8_t _cols = 0;
    uint8_t _rows = 0;
    uint8_t _backlightval = 0;
    /** @brief Display buffer for characters */
    char _displayBuffer[2][16] = {{0}};
    /** @brief Dirty buffer for tracking changed characters */
    bool _dirtyBuffer[2][16] = {{false}};
    uint8_t _cursorCol = 0;
    uint8_t _cursorRow = 0;
    uint8_t _lastR = 255, _lastG = 255, _lastB = 255;
};

#endif // __LCD1602_H__
