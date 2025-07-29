#include <Wire.h>
#include <SegLCD_PCF85176_OneDigit.h>

SegLCD_PCF85176_OneDigit::SegLCD_PCF85176_OneDigit(TwoWire& i2c, uint8_t address, uint8_t subaddress) :  SegDriver_PCF85176(i2c, address, subaddress) {}

void SegLCD_PCF85176_OneDigit::init() {
    init(false);
}

void SegLCD_PCF85176_OneDigit::init(bool reverse) {
    init(reverse, false);
}

void SegLCD_PCF85176_OneDigit::init(bool reverse, bool v1fix) {
    SegDriver_PCF85176::init();
    _setMode(MODE_STATUS_ENABLED, MODE_DRIVE_STATIC);
    _reverse = reverse;
    _v1fix = v1fix;
}

void SegLCD_PCF85176_OneDigit::clear() {
    memset(_buffer, 0x00, sizeof(_buffer));
    SegDriver_PCF85176::clear();
}

void SegLCD_PCF85176_OneDigit::setDecimal(uint8_t row, uint8_t col, bool state) {
    if (row != 0) {
        return; // invalid digit
    }

    if (col < DECIMAL_MIN_COL || col > DECIMAL_MAX_COL) {
        return; // Invalid digit
    }

    if (state) {
        _buffer[col] |= DECIMAL_POINT_BIT; // Set the decimal point bit
    } else {
        _buffer[col] &= ~DECIMAL_POINT_BIT; // Clear the decimal point bit
    }

    if (_reverse) {
        _writeRam(_buffer[col], (DIGITS - (col + 1)) * 8);
    } else {
        _writeRam(_buffer[col], col * 8);
    }
}

size_t SegLCD_PCF85176_OneDigit::write(uint8_t ch) {
    if (_cursorCol < 0 || _cursorCol > DIGITS) {
        return 0; //Invalid digit
    }

    if (ch == '.') {
        setDecimal(_cursorRow, _cursorCol - 1, true);
        return 1;
    }

    uint8_t segment_data = _get_char_value(ch);

    // Board version v1 have swapped two segments
    if (_v1fix) {
        bool bit_2 = (segment_data >> 2) & 1;
        bool bit_3 = (segment_data >> 3) & 1;

        if (bit_2 != bit_3) {
            segment_data ^= (1 << 2) | (1 << 3);
        }
    }

    _buffer[_cursorCol] = segment_data;

    if (_reverse) {
        _writeRam(_buffer[_cursorCol], (DIGITS - (_cursorCol + 1)) * 8);
    } else {
        _writeRam(_buffer[_cursorCol], _cursorCol * 8);
    }

    _cursorCol++;
    return 1;
}
