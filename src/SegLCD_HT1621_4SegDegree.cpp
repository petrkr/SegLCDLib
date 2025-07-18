#include <SegLCD_HT1621_4SegDegree.h>


SegLCD_HT1621_4SegDegree::SegLCD_HT1621_4SegDegree(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read) : SegDriver_HT1621(chipselect, data, write, read) {}

void SegLCD_HT1621_4SegDegree::begin() {
    SegDriver_HT1621::begin();
    _setMode(MODE_DRIVE_14, MODE_BIAS_13);

    _sendCommand(CMD_SYS_EN);
    _sendCommand(CMD_LCD_ON);
    _sendCommand(CMD_NORMAL);
}

void SegLCD_HT1621_4SegDegree::setDegree(bool state, LCDSections section) {
    _writeSymbols(0, state); // Bit 0 is degree
}

void SegLCD_HT1621_4SegDegree::setMiddleDot(bool state, LCDSections section) {
    _writeSymbols(1, state); // Bit 1 is middle dot
}

void SegLCD_HT1621_4SegDegree::setClockColon(bool state, LCDSections section) {
    setMiddleDot(state);
    setDecimal(2, state); // Decimal at 2nd digit is used as part of clock colon
}

void SegLCD_HT1621_4SegDegree::setDecimal(uint8_t digit, bool state, LCDSections section) {
    uint8_t bitnum = 0;

    switch (digit) {
        case 1:
            bitnum = 3; // Bit 3 is dot at 1st digit
            break;
        case 2:
            bitnum = 2; // Bit 2 is dot at 2nd digit
            break;
        default:
            return;
    }

    _writeSymbols(bitnum, state);
}

void SegLCD_HT1621_4SegDegree::writeChar(uint8_t digit, char c, LCDSections section) {
    if (digit > 4) {
        return;
    }

    if (section != LCDSections::SECTION_TOP && section != LCDSections::SECTION_DEFAULT) {
        return;
    }

    uint8_t segments = _mapSegments(_get_char_value(c));
    uint8_t common = 4 - digit;  // C3 for position 1, C2 for position 2, etc.

    // Clear the bits for segments 0-5
    for (int i = 0; i < 3; i++) {
        _buffer[i] &= ~(1 << common);            // Clear bits in each byte for this position
        _buffer[i] &= ~(1 << (common + 4));      // Clear bits for higher segment
    }

    // Skip clearing segment 6, dots, degree
    // Clear segment 7
    _buffer[3] &= ~(1 << (common + 4));      // Clear bits for higher segment

    // Map each segment to the correct byte and bit within _buffer based on segmentPatterns
    if (segments & 0b00000001) _buffer[0] |= (1 << (common + 4)); // A -> S1
    if (segments & 0b00000010) _buffer[0] |= (1 << common);       // B -> S0
    if (segments & 0b00000100) _buffer[1] |= (1 << (common + 4)); // C -> S3
    if (segments & 0b00001000) _buffer[1] |= (1 << common);       // D -> S2
    if (segments & 0b00010000) _buffer[2] |= (1 << (common + 4)); // E -> S5
    if (segments & 0b00100000) _buffer[2] |= (1 << common);       // F -> S4
    if (segments & 0b01000000) _buffer[3] |= (1 << (common + 4)); // G -> S7

    _write(_buffer, sizeof(_buffer), 0);
}

// ABCD_EFGH to HGFE_DCBA
uint8_t SegLCD_HT1621_4SegDegree::_mapSegments(uint8_t val) {
    uint8_t out = 0;

    // Reverse bits
    for (int i = 0; i < 8; i++) {
        out <<= 1;
        out |= (val & 1);
        val >>= 1;
    }

    return out;
}

void SegLCD_HT1621_4SegDegree::_writeSymbols(uint8_t bitnum, bool state) {
    if (state) {
      _buffer[3] |= (1 << bitnum);
    } else {
      _buffer[3] &= ~(1 << bitnum);
    }
    
    _write(_buffer[3], ADDR_SYMBOLS);
}
