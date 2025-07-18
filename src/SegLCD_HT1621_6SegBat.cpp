#include <SegLCD_HT1621_6SegBat.h>


SegLCD_HT1621_6SegBat::SegLCD_HT1621_6SegBat(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read) : SegDriver_HT1621(chipselect, data, write, read) {}

void SegLCD_HT1621_6SegBat::begin() {
    SegDriver_HT1621::begin();
    _setMode(MODE_DRIVE_14, MODE_BIAS_13);

    _sendCommand(CMD_SYS_EN);
    _sendCommand(CMD_LCD_ON);
    _sendCommand(CMD_NORMAL);
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

    _write(_buffer_default[2], 6);
    _write(_buffer_default[1], 8);
    _write(_buffer_default[0], 10);
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

    _write(_buffer_default[(digit)], ((6 - digit - 1) * 2));
}

void SegLCD_HT1621_6SegBat::writeChar(uint8_t digit, char c, LCDSections section) {
    uint8_t ch = _mapSegments(_get_char_value(c));

    switch (section) {
        case LCDSections::SECTION_TOP:
        case LCDSections::SECTION_DEFAULT:
            if (digit < 1 || digit > 6) {
                return; // Invalid digit
            }
            _buffer_default[digit - 1] &= ~0b01111111;
            _buffer_default[digit - 1] |= ch & 0b01111111;
            _write(_buffer_default[digit-1], ((6 - digit) * 2));
            break;
    }
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
