#include <SegLCD_HT1621_4SegDegree.h>


SegLCD_HT1621_4SegDegree::SegLCD_HT1621_4SegDegree(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read) : SegDriver_HT1621(chipselect, data, write, read) {
    _allocateBuffer(RAM_SIZE);
}

void SegLCD_HT1621_4SegDegree::init() {
    SegDriver_HT1621::init();
    _setMode(MODE_DRIVE_14, MODE_BIAS_13);

    command(CMD_SYS_EN);
    command(CMD_LCD_ON);
    command(CMD_NORMAL);
}

void SegLCD_HT1621_4SegDegree::setDegree(bool state) {
    _writeSymbols(0, state); // Bit 0 is degree
}

void SegLCD_HT1621_4SegDegree::setMiddleDot(bool state) {
    _writeSymbols(1, state); // Bit 1 is middle dot
}

void SegLCD_HT1621_4SegDegree::setColon(uint8_t row, uint8_t col, bool state) {
    setMiddleDot(state);
    setDecimal(0, 1, state); // Decimal at 2nd digit is used as part of clock colon
}

void SegLCD_HT1621_4SegDegree::setDecimal(uint8_t row, uint8_t col, bool state) {
    if (row != 0) {
        return; // Invalid digit
    }

    if (col > 1 || col < 0) {
        return; // Invalid digit
    }

    uint8_t bitnum = 0;

    switch (col) {
        case 0:
            bitnum = 3; // Bit 3 is dot at 1st digit
            break;
        case 1:
            bitnum = 2; // Bit 2 is dot at 2nd digit
            break;
        default:
            return;
    }

    _writeSymbols(bitnum, state);
}

size_t SegLCD_HT1621_4SegDegree::write(uint8_t ch) {

    if (_cursorRow != 0) {
        return 0;
    }

    if (_cursorCol < 0 || _cursorCol >= DIGITS) {
        return 0;  // Invalid digit
    }

    // Decimal point - does NOT move cursor (RAM offset -1: previous position)
    if (ch == '.') {
        if (_cursorCol > DECIMAL_MIN_COL && _cursorCol <= DECIMAL_MAX_COL + 1) {
            setDecimal(_cursorRow, _cursorCol - 1, true);
        }
        return 1;  // Never move cursor for dot
    }

    // Colon - does NOT move cursor
    if (ch == ':') {
        if (_cursorCol > COLON_COL) {
            setColon(_cursorRow, _cursorCol - 1, true);
        }
        return 1;  // Never move cursor for colon
    }

    uint8_t segment_data = _mapSegments(_get_char_value(ch));
    uint8_t common = DIGITS - _cursorCol - 1;  // C3 for position 1, C2 for position 2, etc.

    // Clear the bits for segments 0-5
    for (int i = 0; i < DIGITS - 1; i++) {
        _ramBuffer[i] &= ~(1 << common);            // Clear bits in each byte for this position
        _ramBuffer[i] &= ~(1 << (common + 4));      // Clear bits for higher segment
    }

    // do not clear degree and middle dot at segment 6
    if (_cursorCol != 2 && _cursorCol != 3) {
        _ramBuffer[3] &= ~(1 << common);
    }
    // Clear segment 7
    _ramBuffer[3] &= ~(1 << (common + 4));      // Clear bits for higher segment

    // Map each segment to the correct byte and bit within _ramBuffer based on segmentPatterns
    if (segment_data & 0b00000001) _ramBuffer[0] |= (1 << (common + 4)); // A -> S1
    if (segment_data & 0b00000010) _ramBuffer[0] |= (1 << common);       // B -> S0
    if (segment_data & 0b00000100) _ramBuffer[1] |= (1 << (common + 4)); // C -> S3
    if (segment_data & 0b00001000) _ramBuffer[1] |= (1 << common);       // D -> S2
    if (segment_data & 0b00010000) _ramBuffer[2] |= (1 << (common + 4)); // E -> S5
    if (segment_data & 0b00100000) _ramBuffer[2] |= (1 << common);       // F -> S4
    if (segment_data & 0b01000000) _ramBuffer[3] |= (1 << (common + 4)); // G -> S7

    _writeRam(_ramBuffer, RAM_SIZE, 0);

    _cursorCol++;
    return 1;
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
      _ramBuffer[3] |= (1 << bitnum);
    } else {
      _ramBuffer[3] &= ~(1 << bitnum);
    }

    _writeRam(_ramBuffer[3], ADDR_SYMBOLS);
}
