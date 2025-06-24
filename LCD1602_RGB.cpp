#include <Wire.h>
#include <Arduino.h>
#include <LCD1602_RGB.h>

LCD1602::LCD1602(uint8_t lcd_cols, uint8_t lcd_rows)
    : _cols(lcd_cols), _rows(lcd_rows) {}

void LCD1602::init() {
    if (!Wire.begin()) {
        // Error handling: Wire.begin() failed
        while (true) {
            // Optionally blink LED or halt
        }
    }
    _showfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    begin(_cols, _rows);
}

void LCD1602::begin(uint8_t cols, uint8_t lines) {
    if (lines > 1) {
        _showfunction |= LCD_2LINE;
    }
    _numlines = lines;
    _currline = 0;
    delay(50);  // Required delay during initialization

    for (uint8_t i = 0; i < 2; ++i) {
        command(LCD_FUNCTIONSET | _showfunction);
        delay(5);  // Required by LCD spec
    }
    _showcontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    display();
    clear();
    _showmode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    command(LCD_ENTRYMODESET | _showmode);

    Wire.beginTransmission(_RGBAddr);
    Wire.write(REG_MODE1);
    Wire.write(0x00);  // Normal mode
    Wire.endTransmission();

    Wire.beginTransmission(_RGBAddr);
    Wire.write(REG_OUTPUT);
    Wire.write(0xFF);  // Turn on PWM for all LEDs
    Wire.endTransmission();

    Wire.beginTransmission(_RGBAddr);
    Wire.write(REG_MODE2);
    Wire.write(0x20);  // Auto increment enabled
    Wire.endTransmission();

    setColorWhite();
}

void LCD1602::command(uint8_t value) {
    uint8_t data[2] = {0x80, value};
    send(data, 2);
}

void LCD1602::send(uint8_t *data, uint8_t len) {
    Wire.beginTransmission(_lcdAddr);
    for (uint8_t i = 0; i < len; ++i) {
        if (Wire.write(data[i]) != 1) {
            break;
        }
        delayMicroseconds(50);  // Small delay required for LCD timing
    }
    Wire.endTransmission();
}

void LCD1602::display() {
    _showcontrol |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _showcontrol);
}

void LCD1602::clear() {
    command(LCD_CLEARDISPLAY);
    delayMicroseconds(2000);  // This delay is required by LCD spec
    // Clear buffer
    for (uint8_t r = 0; r < 2; ++r)
        for (uint8_t c = 0; c < 16; ++c)
            _displayBuffer[r][c] = ' ';
    _cursorCol = 0;
    _cursorRow = 0;
}

void LCD1602::setReg(uint8_t addr, uint8_t data) const {
    Wire.beginTransmission(_RGBAddr);
    Wire.write(addr);
    Wire.write(data);
    Wire.endTransmission();
}

void LCD1602::setRGB(uint8_t r, uint8_t g, uint8_t b) {
    // Only update if color actually changes
    if (r == _lastR && g == _lastG && b == _lastB) return;
    _lastR = r; _lastG = g; _lastB = b;
    Wire.beginTransmission(_RGBAddr);
    Wire.write(REG_RED);
    Wire.write(r);
    Wire.write(g);
    Wire.write(b);
    Wire.endTransmission();
}

void LCD1602::setCursor(uint8_t col, uint8_t row) {
    if (row >= _numlines) row = _numlines - 1;
    _cursorCol = col;
    _cursorRow = row;
    uint8_t addr = (row == 0 ? col | 0x80 : col | 0xc0);
    uint8_t data[2] = {0x80, addr};
    send(data, 2);
}

void LCD1602::write_char(uint8_t value) {
    if (_cursorRow < 2 && _cursorCol < 16 && _displayBuffer[_cursorRow][_cursorCol] == value) {
        _cursorCol++;
        return; // No change, skip
    }
    _displayBuffer[_cursorRow][_cursorCol] = value;
    uint8_t data[2] = {0x40, value};
    send(data, 2);
    _cursorCol++;
}

void LCD1602::send_string(const char *str) {
    if (str == nullptr) return;
    uint8_t col = _cursorCol;
    uint8_t row = _cursorRow;
    if (row >= 2) return;
    setCursor(col, row);
    for (uint8_t i = 0; str[i] && col < 16; ++i, ++col) {
        if (_displayBuffer[row][col] == str[i]) continue;
        _displayBuffer[row][col] = str[i];
        uint8_t data[2] = {0x40, static_cast<uint8_t>(str[i])};
        send(data, 2);
    }
    _cursorCol = col;
}

void LCD1602::BlinkLED() {
    setReg(REG_BLINK, 0x17);
    setReg(REG_BREATH, 0x7f);
}

void LCD1602::noBlinkLED() {
    setReg(REG_BLINK, 0x00);
    setReg(REG_BREATH, 0xff);
}

void LCD1602::customSymbol(uint8_t location, const uint8_t charmap[8]) {
    if (charmap == nullptr) return;
    location &= 0x7;
    command(LCD_SETCGRAMADDR | (location << 3));

    uint8_t data[9];
    data[0] = 0x40;
    memcpy(data + 1, charmap, 8);
    send(data, 9);
}

void LCD1602::home() {
    command(LCD_RETURNHOME);
    delayMicroseconds(2000);  // This delay is required by LCD spec
}

void LCD1602::stopBlink() {
    _showcontrol &= ~LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _showcontrol);
}

void LCD1602::blink() {
    _showcontrol |= LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _showcontrol);
}

void LCD1602::noCursor() {
    _showcontrol &= ~LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _showcontrol);
}

void LCD1602::cursor() {
    _showcontrol |= LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _showcontrol);
}

void LCD1602::scrollDisplayLeft() {
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void LCD1602::scrollDisplayRight() {
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void LCD1602::leftToRight() {
    _showmode |= LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _showmode);
}

void LCD1602::rightToLeft() {
    _showmode &= ~LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _showmode);
}

void LCD1602::noAutoscroll() {
    _showmode &= ~LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _showmode);
}

void LCD1602::autoscroll() {
    _showmode |= LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _showmode);
}