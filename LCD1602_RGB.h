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

class LCD1602 {
public:
    LCD1602(uint8_t lcd_cols, uint8_t lcd_rows);

    void init();
    void home();
    void display();
    void command(uint8_t value);
    void send(uint8_t *data, uint8_t len);
    void setReg(uint8_t addr, uint8_t data) const;
    void setRGB(uint8_t r, uint8_t g, uint8_t b) const;
    void setCursor(uint8_t col, uint8_t row);
    void clear();
    void BlinkLED();
    void noBlinkLED();
    void write_char(uint8_t value);
    void send_string(const char *str);
    void customSymbol(uint8_t location, const uint8_t charmap[8]);
    void stopBlink();
    void blink();
    void noCursor();
    void cursor();
    void scrollDisplayLeft();
    void scrollDisplayRight();
    void leftToRight();
    void rightToLeft();
    void noAutoscroll();
    void autoscroll();
    void setColorWhite() const { setRGB(255, 255, 255); }

private:
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
};

#endif // __LCD1602_H__
