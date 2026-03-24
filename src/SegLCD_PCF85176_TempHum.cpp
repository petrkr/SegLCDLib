#include <SegLCD_PCF85176_TempHum.h>


SegLCD_PCF85176_TempHumidity::SegLCD_PCF85176_TempHumidity(SegTransportI2C& transport, uint8_t address, uint8_t subaddress) :  SegDriver_PCF85176(transport, address, subaddress) {
    _allocateBuffer(RAM_SIZE);
}

void SegLCD_PCF85176_TempHumidity::init() {
    SegDriver_PCF85176::init();
    _setMode(MODE_STATUS_ENABLED);
}

void SegLCD_PCF85176_TempHumidity::clear() {
    _clearFlag(FLAG_MINUS_DISPLAYED);
    SegDriver_PCF85176::clear();
}

void SegLCD_PCF85176_TempHumidity::setBatteryLevel(uint8_t level) {

    if (level > MAX_SIGNAL_BATTERY_LEVEL)
        level = MAX_SIGNAL_BATTERY_LEVEL;

    _ramBuffer[OFFSET_SIGBATT] &= ~SIGNAL_BATTERY_MASK_LOW;

    if (level > 0)
        _ramBuffer[OFFSET_SIGBATT] |= BATTERY_LEVEL_BITS[0];
    if (level > 1)
        _ramBuffer[OFFSET_SIGBATT] |= BATTERY_LEVEL_BITS[1];
    if (level > 2)
        _ramBuffer[OFFSET_SIGBATT] |= BATTERY_LEVEL_BITS[2];
    if (level > 3)
        _ramBuffer[OFFSET_SIGBATT] |= BATTERY_LEVEL_BITS[3];

    _writeRam(_ramBuffer[OFFSET_SIGBATT], ADDR_SIGNAL_BATT);
}

void SegLCD_PCF85176_TempHumidity::setSignalLevel(uint8_t level) {

    if (level > MAX_SIGNAL_BATTERY_LEVEL)
        level = MAX_SIGNAL_BATTERY_LEVEL;

    _ramBuffer[OFFSET_SIGBATT] &= ~SIGNAL_BATTERY_MASK_HIGH;

    if (level > 0)
        _ramBuffer[OFFSET_SIGBATT] |= SIGNAL_LEVEL_BITS[0];
    if (level > 1)
        _ramBuffer[OFFSET_SIGBATT] |= SIGNAL_LEVEL_BITS[1];
    if (level > 2)
        _ramBuffer[OFFSET_SIGBATT] |= SIGNAL_LEVEL_BITS[2];
    if (level > 3)
        _ramBuffer[OFFSET_SIGBATT] |= SIGNAL_LEVEL_BITS[3];

    _writeRam(_ramBuffer[OFFSET_SIGBATT], ADDR_SIGNAL_BATT);
}

void SegLCD_PCF85176_TempHumidity::setLabels(uint8_t labels) {

    if (labels & LABEL_DEGREE_C) {
        _ramBuffer[OFFSET_TEMP + LABEL_TEMP_COL] |= LABEL_BIT; // Set the C label
        _writeRam(_ramBuffer[OFFSET_TEMP + LABEL_TEMP_COL], ADDR_TEMP_SEGS + TEMP_LABEL_OFFSET);
    }

    if (labels & LABEL_PROC) {
        _ramBuffer[OFFSET_HUM + LABEL_HUM_COL] |= LABEL_BIT; // Set the proc symbol
        _writeRam(_ramBuffer[OFFSET_HUM + LABEL_HUM_COL], ADDR_HUM_SEGS + HUM_LABEL_OFFSET);
    }
}

void SegLCD_PCF85176_TempHumidity::clearLabels(uint8_t labels) {

    if (labels & LABEL_DEGREE_C) {
        _ramBuffer[OFFSET_TEMP + LABEL_TEMP_COL] &= ~LABEL_BIT; // Clear the C label
        _writeRam(_ramBuffer[OFFSET_TEMP + LABEL_TEMP_COL], ADDR_TEMP_SEGS + TEMP_LABEL_OFFSET);
    }

    if (labels & LABEL_PROC) {
        _ramBuffer[OFFSET_HUM + LABEL_HUM_COL] &= ~LABEL_BIT; // Clear the proc symbol
        _writeRam(_ramBuffer[OFFSET_HUM + LABEL_HUM_COL], ADDR_HUM_SEGS + HUM_LABEL_OFFSET);
    }
}

void SegLCD_PCF85176_TempHumidity::_setDecimal(uint8_t row, uint8_t col, bool state) {

    uint8_t address = 0;
    uint8_t offset = 0;

    switch (row) {
    case TEMP_ROW: // Temp segments
        if (col >= TEMP_DIGITS) {
            return; // Invalid digit
        }

        address = ADDR_TEMP_SEGS + (col * 2);
        offset = OFFSET_TEMP;
        break;


    case HUM_ROW: // Hum segments
        if (col >= HUM_DIGITS) {
            return; // Invalid digit
        }

        address = ADDR_HUM_SEGS + (col * 2);
        offset = OFFSET_HUM;
        break;

    default:
        return; // Invalid selection
    }

    if (state) {
        _ramBuffer[offset + col] |= DECIMAL_POINT_BIT; // Set the decimal point bit
    } else {
        _ramBuffer[offset + col] &= ~DECIMAL_POINT_BIT; // Clear the decimal point bit
    }

    _writeRam(_ramBuffer[offset + col], address);
}

void SegLCD_PCF85176_TempHumidity::setCursor(uint8_t row, uint8_t col) {
    if (row == TEMP_ROW && col == 0) {
        _clearFlag(FLAG_MINUS_DISPLAYED);
    }

    SegDriver_PCF85176::setCursor(row, col);
}

size_t SegLCD_PCF85176_TempHumidity::write(uint8_t ch) {

    // Wrap to next row if cursor exceeds current row capacity
    if (_cursorRow == TEMP_ROW && _cursorCol >= TEMP_DIGITS) {
        _cursorRow = HUM_ROW;
        _cursorCol = 0;
    } else if (_cursorRow == HUM_ROW && _cursorCol >= HUM_DIGITS) {
        return 0;  // End of display
    }

    if (_cursorRow == TEMP_ROW) {
        // Temp segments (row 0)
        if (_cursorCol >= TEMP_DIGITS) {
            return 0;  // Invalid digit
        }

        // Clear minus if writing non-minus at col 0
        if (_cursorCol == 0 && ch != '-' && !_isFlagSet(FLAG_MINUS_DISPLAYED) && (_ramBuffer[OFFSET_HUM] & MINUS_SIGN_BIT)) {
            _ramBuffer[OFFSET_HUM] &= ~MINUS_SIGN_BIT;
            _writeRam(_ramBuffer[OFFSET_HUM], ADDR_HUM_SEGS);
        }

        // Handle decimal point - only set, don't clear previous
        if (_dotWrite(ch, DECIMAL_TOP_MIN_COL, DECIMAL_TOP_MAX_COL, DECIMAL_COL_OFFSET)) {
            return 1;
        }

        // Minus sign at position 0 (special handling)
        if (ch == '-' && _cursorCol == 0 && !_isFlagSet(FLAG_MINUS_DISPLAYED)) {
            _ramBuffer[OFFSET_HUM] |= MINUS_SIGN_BIT;
            _writeRam(_ramBuffer[OFFSET_HUM], ADDR_HUM_SEGS);
            _setFlag(FLAG_MINUS_DISPLAYED);
            return 1;  // Minus doesn't move cursor
        }

        // Clear decimal on current column when writing regular character
        if (_cursorCol != LABEL_TEMP_COL) {  // Label column has C label, not decimal
            _setDecimal(TEMP_ROW, _cursorCol, false);
        }

        // Regular character
        uint8_t c = _mapSegments(_get_char_value(ch));
        if (_cursorCol == LABEL_TEMP_COL) {
            // Preserve C label bit - keep bit 3, write other bits from c
            _ramBuffer[OFFSET_TEMP + _cursorCol] = (_ramBuffer[OFFSET_TEMP + _cursorCol] & 0x08) | (c & ~0x08);
        } else {
            _ramBuffer[OFFSET_TEMP + _cursorCol] = c;
        }
        _writeRam(_ramBuffer[OFFSET_TEMP + _cursorCol], ADDR_TEMP_SEGS + _cursorCol * 2);
        _cursorCol++;
        return 1;

    } else if (_cursorRow == HUM_ROW) {
        // Humidity segments (row 1)
        if (_cursorCol >= HUM_DIGITS) {
            return 0;  // Invalid digit
        }

        // Handle decimal point - only set, don't clear previous
        if (_dotWrite(ch, DECIMAL_BOTTOM_MIN_COL, DECIMAL_BOTTOM_MAX_COL, DECIMAL_COL_OFFSET)) {
            return 1;
        }

        // Clear decimal on current column when writing regular character (except at col 0 where minus is, and label column where % is)
        if (_cursorCol >= DECIMAL_BOTTOM_MIN_COL && _cursorCol != LABEL_HUM_COL) {
            _setDecimal(HUM_ROW, _cursorCol, false);
        }

        // Regular character
        uint8_t c = _mapSegments(_get_char_value(ch));
        if (_cursorCol == 0) {
            // Preserve minus sign bit at position 0
            _ramBuffer[OFFSET_HUM + _cursorCol] = (_ramBuffer[OFFSET_HUM + _cursorCol] & MINUS_SIGN_BIT) | (c & ~MINUS_SIGN_BIT);
        } else if (_cursorCol == LABEL_HUM_COL) {
            // Preserve % label bit - keep bit 3, write other bits from c
            _ramBuffer[OFFSET_HUM + _cursorCol] = (_ramBuffer[OFFSET_HUM + _cursorCol] & 0x08) | (c & ~0x08);
        } else {
            _ramBuffer[OFFSET_HUM + _cursorCol] = c;
        }
        _writeRam(_ramBuffer[OFFSET_HUM + _cursorCol], ADDR_HUM_SEGS + _cursorCol * 2);
        _cursorCol++;
        return 1;
    }

    return 0;
}

// ABCD_EFGH to DEGF_PCBA
uint8_t SegLCD_PCF85176_TempHumidity::_mapSegments(uint8_t val) {
    uint8_t out = 0;
    out |= (val & 0b10000000) >> 7;      // A: bit 7 → 0
    out |= (val & 0b01000000) >> 5;      // B: bit 6 → 1
    out |= (val & 0b00100000) >> 3;      // C: bit 5 → 2
    out |= (val & 0b00010000) << 3;      // D: bit 4 → 7
    out |= (val & 0b00001000) << 3;      // E: bit 3 → 6
    out |= (val & 0b00000100) << 2;      // F: bit 2 → 4
    out |= (val & 0b00000010) << 4;      // G: bit 1 → 5
    out |= (val & 0b00000001) << 3;      // H: bit 0 → 3
    return out;
}
