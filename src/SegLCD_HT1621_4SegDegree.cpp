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

void SegLCD_HT1621_4SegDegree::clear() {
    SegDriver_HT1621::clear();
    _clearAllFlags();
}

void SegLCD_HT1621_4SegDegree::setCursor(uint8_t row, uint8_t col) {
    _clearFlag(FLAG_COLON_SESSION);
    SegLCDLib::setCursor(row, col);
}

void SegLCD_HT1621_4SegDegree::setDegree(bool state) {
    _writeSymbols(0, state); // Bit 0 is degree
}

void SegLCD_HT1621_4SegDegree::setMiddleDot(bool state) {
    _writeSymbols(1, state); // Bit 1 is middle dot
}

void SegLCD_HT1621_4SegDegree::_setColon(uint8_t row, uint8_t col, bool state) {
    (void)row;
    (void)col;

    // Colon = middle dot + decimal at 2nd digit
    if (state) {
        _ramBuffer[3] |= (1 << 1); // middle dot
        _ramBuffer[3] |= (1 << 2); // dot at 2nd digit
    } else {
        _ramBuffer[3] &= ~(1 << 1);
        _ramBuffer[3] &= ~(1 << 2);
    }

    _writeRam(_ramBuffer[3], ADDR_SYMBOLS);
}

void SegLCD_HT1621_4SegDegree::_setDecimal(uint8_t row, uint8_t col, bool state) {
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

    // Handle decimal point and colon (note: cursor at col 2 when writing ':')
    if (_dotWrite(ch, DECIMAL_MIN_COL, DECIMAL_MAX_COL, -1)) {
        return 1;
    }
    if (_colonWrite(ch, COLON_COL + 1, FLAG_COLON_DISPLAYED)) {
        _setFlag(FLAG_COLON_SESSION);
        return 1;
    }

    // Clear colon when writing digit after colon without ':' in this session
    if (ch != ':' && _cursorCol == COLON_COL + 1 &&
        _isFlagSet(FLAG_COLON_DISPLAYED) && !_isFlagSet(FLAG_COLON_SESSION)) {
        _setColon(0, COLON_COL, false);
        _clearFlag(FLAG_COLON_DISPLAYED);
    }

    // Clear pending dot flag or clear decimal on current column
    if (_isFlagSet(FLAG_PENDING_DOT)) {
        _clearFlag(FLAG_PENDING_DOT);
    } else if (_cursorCol >= DECIMAL_MIN_COL && _cursorCol <= DECIMAL_MAX_COL) {
        if (!(_cursorCol == 1 && _isFlagSet(FLAG_COLON_DISPLAYED))) {
            _setDecimal(0, _cursorCol, false);
        }
    }

    uint8_t segment_data = _mapSegments(_get_char_value(ch));
    uint8_t common = DIGITS - _cursorCol - 1;  // C3 for position 1, C2 for position 2, etc.

    // Clear the bits for segments 0-5
    for (int i = 0; i < DIGITS - 1; i++) {
        _ramBuffer[i] &= ~(1 << common);            // Clear bits in each byte for this position
        _ramBuffer[i] &= ~(1 << (common + 4));      // Clear bits for higher segment
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
