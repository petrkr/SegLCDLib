#include <Wire.h>
#include <SegLCD_PCF85176_TempHum.h>


SegLCD_PCF85176_TempHumidity::SegLCD_PCF85176_TempHumidity(TwoWire& i2c, uint8_t address, uint8_t subaddress) :  SegDriver_PCF85176(i2c, address, subaddress) {
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

    if (labels & LABEL_C) {
        _ramBuffer[OFFSET_TEMP + 3] |= LABEL_BIT; // Set the C label
        _writeRam(_ramBuffer[OFFSET_TEMP + 3], ADDR_TEMP_SEGS + TEMP_LABEL_OFFSET); // C label is at position 4 (3 in zero-based index)
    }

    if (labels & LABEL_PROC) {
        _ramBuffer[OFFSET_HUM + 2] |= LABEL_BIT; // Set the proc symbol
        _writeRam(_ramBuffer[OFFSET_HUM + 2], ADDR_HUM_SEGS + HUM_LABEL_OFFSET); // proc symbol is at position 3 (2 in zero-based index)
    }
}

void SegLCD_PCF85176_TempHumidity::clearLabels(uint8_t labels) {

    if (labels & LABEL_C) {
        _ramBuffer[OFFSET_TEMP + 3] &= ~LABEL_BIT; // Clear the C label
        _writeRam(_ramBuffer[OFFSET_TEMP + 3], ADDR_TEMP_SEGS + TEMP_LABEL_OFFSET); // C label is at position 4 (3 in zero-based index)
    }

    if (labels & LABEL_PROC) {
        _ramBuffer[OFFSET_HUM + 2] &= ~LABEL_BIT; // Clear the proc symbol
        _writeRam(_ramBuffer[OFFSET_HUM + 2], ADDR_HUM_SEGS + HUM_LABEL_OFFSET); // proc symbol is at position 3 (2 in zero-based index)
    }
}

void SegLCD_PCF85176_TempHumidity::setDecimal(uint8_t row, uint8_t col, bool state) {

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
        if (_dotWrite(ch, 0, TEMP_DIGITS - 1, -1)) {
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
        setDecimal(TEMP_ROW, _cursorCol, false);

        // Regular character
        uint8_t c = _mapSegments(_get_char_value(ch));
        _ramBuffer[OFFSET_TEMP + _cursorCol] = c;
        _writeRam(_ramBuffer[OFFSET_TEMP + _cursorCol], ADDR_TEMP_SEGS + _cursorCol * 2);
        _cursorCol++;
        return 1;

    } else if (_cursorRow == HUM_ROW) {
        // Humidity segments (row 1)
        if (_cursorCol >= HUM_DIGITS) {
            return 0;  // Invalid digit
        }

        // Handle decimal point - only set, don't clear previous
        if (_dotWrite(ch, 1, HUM_DIGITS - 1, -1)) {
            return 1;
        }

        // Clear decimal on current column when writing regular character (except at col 0 where minus is)
        if (_cursorCol > 0) {
            setDecimal(HUM_ROW, _cursorCol, false);
        }

        // Regular character
        uint8_t c = _mapSegments(_get_char_value(ch));
        if (_cursorCol == 0) {
            // Preserve minus sign bit at position 0
            _ramBuffer[OFFSET_HUM + _cursorCol] &= MINUS_SIGN_BIT;
            _ramBuffer[OFFSET_HUM + _cursorCol] |= (c & ~MINUS_SIGN_BIT);
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
