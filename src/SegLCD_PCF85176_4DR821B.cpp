#include <Wire.h>
#include <SegLCD_PCF85176_4DR821B.h>

SegLCD_PCF85176_4DR821B::SegLCD_PCF85176_4DR821B(TwoWire& i2c, uint8_t address, uint8_t subaddress) :  SegDriver_PCF85176(i2c, address, subaddress) {}

void SegLCD_PCF85176_4DR821B::init() {
    SegDriver_PCF85176::init();
    _setMode(MODE_STATUS_ENABLED, MODE_DRIVE_STATIC);
}

void SegLCD_PCF85176_4DR821B::clear() {
    memset(_buffer, 0x00, sizeof(_buffer));
    SegDriver_PCF85176::clear();
}

void SegLCD_PCF85176_4DR821B::setClockColon(uint8_t row, uint8_t col, bool state) {
    if (state)
        _buffer[ADDR_SYMBOLS] |= DECIMAL_POINT_BIT;
    else
        _buffer[ADDR_SYMBOLS] &= ~DECIMAL_POINT_BIT;

    _writeRam(_buffer[ADDR_SYMBOLS], ADDR_SYMBOLS);
}


void SegLCD_PCF85176_4DR821B::setDecimal(uint8_t row, uint8_t col, bool state) {
    if (row != 0) {
        return; // invalid digit
    }

    if (col < DECIMAL_MIN_COL || col > DECIMAL_MAX_COL) {
        return; // Invalid digit
    }

    if (state) {
        _buffer[ADDR_SEGS + col] |= DECIMAL_POINT_BIT; // Set the decimal point bit
    } else {
        _buffer[ADDR_SEGS + col] &= ~DECIMAL_POINT_BIT; // Clear the decimal point bit
    }

    _writeRam(_buffer[ADDR_SEGS + col], (ADDR_SEGS + col) * 8);
    
}

size_t SegLCD_PCF85176_4DR821B::write(uint8_t ch) {
    if (_cursorCol < 0 || _cursorCol > DIGITS) {
        return 0; //Invalid digit
    }

    if (ch == '.') {
        setDecimal(_cursorRow, _cursorCol - 1, true);
        return 1;
    }

    if (ch == ':') {
        setClockColon(_cursorRow, _cursorCol - 1, true);
        return 1;
    }

    uint8_t segment_data = _get_char_value(ch);

    _buffer[ADDR_SEGS + _cursorCol] = segment_data;

    
    _writeRam(_buffer[ADDR_SEGS + _cursorCol], (ADDR_SEGS + _cursorCol) * 8);
    

    _cursorCol++;
    return 1;
}
