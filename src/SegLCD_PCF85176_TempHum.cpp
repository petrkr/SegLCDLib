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

void SegLCD_PCF85176_TempHumidity::_updateUnits(uint8_t units, bool set) {
    if (units & UNIT_DEGREE_C) {
        _writeRamMasked(set ? LABEL_BIT : 0x00, ADDR_TEMP_SEGS + TEMP_LABEL_OFFSET, LABEL_BIT);
    }

    if (units & UNIT_PERCENT) {
        _writeRamMasked(set ? LABEL_BIT : 0x00, ADDR_HUM_SEGS + HUM_LABEL_OFFSET, LABEL_BIT);
    }
}

void SegLCD_PCF85176_TempHumidity::setUnits(uint8_t units) {
    _updateUnits(units, true);
}

void SegLCD_PCF85176_TempHumidity::clearUnits(uint8_t units) {
    _updateUnits(units, false);
}

void SegLCD_PCF85176_TempHumidity::_setDecimal(uint8_t row, uint8_t col, bool state) {

    uint8_t address = 0;

    switch (row) {
    case TEMP_ROW: // Temp segments
        if (col >= TEMP_DIGITS) {
            return; // Invalid digit
        }

        address = ADDR_TEMP_SEGS + (col * 2);
        break;


    case HUM_ROW: // Hum segments
        if (col >= HUM_DIGITS) {
            return; // Invalid digit
        }

        address = ADDR_HUM_SEGS + (col * 2);
        break;

    default:
        return; // Invalid selection
    }

    _writeRamMasked(state ? DECIMAL_POINT_BIT : 0x00, address, DECIMAL_POINT_BIT);
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
        uint8_t temp_mask = (_cursorCol == LABEL_TEMP_COL) ? static_cast<uint8_t>(~LABEL_BIT) : 0xFF;
        _writeRamMasked(c, ADDR_TEMP_SEGS + _cursorCol * 2, temp_mask);
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
        uint8_t hum_mask = 0xFF;
        if (_cursorCol == 0 || _cursorCol == LABEL_HUM_COL) {
            hum_mask = static_cast<uint8_t>(~LABEL_BIT);
        }
        _writeRamMasked(c, ADDR_HUM_SEGS + _cursorCol * 2, hum_mask);
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
