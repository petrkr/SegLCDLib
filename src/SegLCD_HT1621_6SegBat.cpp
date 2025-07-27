#include <SegLCD_HT1621_6SegBat.h>


SegLCD_HT1621_6SegBat::SegLCD_HT1621_6SegBat(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read) : SegDriver_HT1621(chipselect, data, write, read) {}

void SegLCD_HT1621_6SegBat::init() {
    SegDriver_HT1621::init();
    _setMode(MODE_DRIVE_14, MODE_BIAS_13);

    command(CMD_SYS_EN);
    command(CMD_LCD_ON);
    command(CMD_NORMAL);
}

void SegLCD_HT1621_6SegBat::setBatteryLevel(uint8_t level) {
    if (level > 3)
        level = 3;

    _buffer_default[2] &= ~(0x80);
    _buffer_default[1] &= ~(0x80);
    _buffer_default[0] &= ~(0x80);

    if (level > 0)
        _buffer_default[2] |= 0x80;
    if (level > 1)
        _buffer_default[1] |= 0x80;
    if (level > 2)
        _buffer_default[0] |= 0x80;

    _writeRam(_buffer_default[2], 6);
    _writeRam(_buffer_default[1], 8);
    _writeRam(_buffer_default[0], 10);
}

void SegLCD_HT1621_6SegBat::setDecimal(uint8_t digit, bool state, LCDSections section) {
    if (digit < 3 || digit > 5) {
        return; // Invalid digit
    }

    if (state) {
        _buffer_default[(digit)] |= 0x80; // Set the decimal point bit
    } else {
        _buffer_default[(digit)] &= ~0x80; // Clear the decimal point bit
    }

    _writeRam(_buffer_default[(digit)], ((6 - digit - 1) * 2));
}

void SegLCD_HT1621_6SegBat::setCursor(uint8_t row, uint8_t col) {
    _cursorCol = col;
    _cursorRow = row;
}

size_t SegLCD_HT1621_6SegBat::write(uint8_t ch) {
    if (ch == '.') {
        if (_cursorCol > 0 && _cursorCol <= 6) {
            uint8_t prev_digit_idx = _cursorCol - 1;
            _buffer_default[prev_digit_idx] |= 0x80;
            _writeRam(_buffer_default[prev_digit_idx], (6 - (prev_digit_idx + 1)) * 2);
        }
        return 1;
    }

    if (_cursorCol >= 0 && _cursorCol < 6) {
        uint8_t current_digit_idx = _cursorCol;
        uint8_t segment_data = _mapSegments(_get_char_value(ch));

        uint8_t decimal_point = _buffer_default[current_digit_idx] & 0x80;

        _buffer_default[current_digit_idx] = (segment_data & 0b01111111) | decimal_point;
        _writeRam(_buffer_default[current_digit_idx], (6 - (current_digit_idx + 1)) * 2);

        _cursorCol++;
        return 1;
    }

    return 0;
}

// ABCD_EFGH to HCBA_DEGF
uint8_t SegLCD_HT1621_6SegBat::_mapSegments(uint8_t val) {
    uint8_t out = 0;

    out |= (val & 0b01011000) >> 1;      // BDE: bits 6,4,3 → 5,3,2
    out |= (val & 0b10000000) >> 3;      // A: bit 7 → 4
    out |= (val & 0b00100000) << 1;      // C: bit 5 → 6
    out |= (val & 0b00000100) >> 2;      // F: bit 2 → 0
    out |= (val & 0b00000010);           // G: bit 1 → 1
    out |= (val & 0b00000001) << 7;      // H: bit 0 → 7
    return out;
}
