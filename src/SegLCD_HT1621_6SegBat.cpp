#include <SegLCD_HT1621_6SegBat.h>


SegLCD_HT1621_6SegBat::SegLCD_HT1621_6SegBat(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read) : SegDriver_HT1621(chipselect, data, write, read) {}

void SegLCD_HT1621_6SegBat::init() {
    SegDriver_HT1621::init();
    _setMode(MODE_DRIVE_14, MODE_BIAS_13);

    command(CMD_SYS_EN);
    command(CMD_LCD_ON);
    command(CMD_NORMAL);
}

void SegLCD_HT1621_6SegBat::clear() {
    memset(_buffer_default, 0x00, sizeof(_buffer_default));
    SegDriver_HT1621::clear();
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

void SegLCD_HT1621_6SegBat::setDecimal(uint8_t row, uint8_t col, bool state) {
    if (row != 0) {
        return; // invalid digit
    }

    if (col < 3 || col > 5) {
        return; // Invalid digit
    }

    if (state) {
        _buffer_default[col] |= 0x80; // Set the decimal point bit
    } else {
        _buffer_default[col] &= ~0x80; // Clear the decimal point bit
    }

    _writeRam(_buffer_default[col], ((6 - col - 1) * 2));
}

size_t SegLCD_HT1621_6SegBat::write(uint8_t ch) {
    if (_cursorCol < 0 || _cursorCol > 6) {
        return 0; //Invalid digit
    }

    if (ch == '.') {
        setDecimal(_cursorRow, _cursorCol, true);
        _previousDot = true;
        return 1;
    }

    uint8_t current_digit_idx = _cursorCol;
    uint8_t segment_data = _mapSegments(_get_char_value(ch));

    if ((_cursorCol >= 0 && _cursorCol <= 2) || _previousDot) {
        segment_data |= _buffer_default[current_digit_idx] & 0x80;
        _previousDot = false;
    }

    _buffer_default[current_digit_idx] = segment_data;
    _writeRam(_buffer_default[current_digit_idx], (6 - (current_digit_idx + 1)) * 2);

    _cursorCol++;
    return 1;
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
