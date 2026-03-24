#include <SegLCD_PCF85176_T1T2Lcd.h>


SegLCD_PCF85176_T1T2Lcd::SegLCD_PCF85176_T1T2Lcd(SegTransportI2C& transport, uint8_t address, uint8_t subaddress) :  SegDriver_PCF85176(transport, address, subaddress) {
    _allocateBuffer(RAM_SIZE);
}

void SegLCD_PCF85176_T1T2Lcd::init() {
    SegDriver_PCF85176::init();
    _setMode(MODE_STATUS_ENABLED);
}

void SegLCD_PCF85176_T1T2Lcd::setBatteryLevel(uint8_t level) {

    if (level > MAX_BATTERY_LEVEL)
        level = MAX_BATTERY_LEVEL;

    _ramBuffer[OFFSET_BATT] &= ~(BATTERY_MASK);

    if (level > 0)
        _ramBuffer[OFFSET_BATT] |= BATTERY_LEVEL_SEG[0];
    if (level > 1)
        _ramBuffer[OFFSET_BATT] |= BATTERY_LEVEL_SEG[1];
    if (level > 2)
        _ramBuffer[OFFSET_BATT] |= BATTERY_LEVEL_SEG[2];
    if (level > 3)
        _ramBuffer[OFFSET_BATT] |= BATTERY_LEVEL_SEG[3];

    _writeRam(_ramBuffer[OFFSET_BATT], ADDR_BATT);
}

void SegLCD_PCF85176_T1T2Lcd::setSignalLevel(uint8_t level) {

    if (level > MAX_SIGNAL_LEVEL)
        level = MAX_SIGNAL_LEVEL;

    _ramBuffer[OFFSET_SIGCLK] &= ~(SIGNAL_MASK);

    if (level > 0)
        _ramBuffer[OFFSET_SIGCLK] |= SIGNAL_LEVEL_BITS[0];
    if (level > 1)
        _ramBuffer[OFFSET_SIGCLK] |= SIGNAL_LEVEL_BITS[1];
    if (level > 2)
        _ramBuffer[OFFSET_SIGCLK] |= SIGNAL_LEVEL_BITS[2];
    if (level > 3)
        _ramBuffer[OFFSET_SIGCLK] |= SIGNAL_LEVEL_BITS[3];
    if (level > 4)
        _ramBuffer[OFFSET_SIGCLK] |= SIGNAL_LEVEL_BITS[4];

    _writeRam(_ramBuffer[OFFSET_SIGCLK], ADDR_SIGNAL_CLOCK);
}

void SegLCD_PCF85176_T1T2Lcd::setClockSymbol(bool status, uint8_t index) {
    if (index > 0) return;

    if (status)
        _ramBuffer[OFFSET_SIGCLK] |= SYMBOL_CLOCK_MASK;
    else
        _ramBuffer[OFFSET_SIGCLK] &= ~SYMBOL_CLOCK_MASK;

    _writeRam(_ramBuffer[OFFSET_SIGCLK], ADDR_SIGNAL_CLOCK);
}

void SegLCD_PCF85176_T1T2Lcd::setLabels(uint8_t labels) {
    _ramBuffer[OFFSET_LABELS] |= labels;
    _writeRam(_ramBuffer[OFFSET_LABELS], ADDR_LABELS);
}

void SegLCD_PCF85176_T1T2Lcd::clearLabels(uint8_t labels) {
    _ramBuffer[OFFSET_LABELS] &= ~labels;
    _writeRam(_ramBuffer[OFFSET_LABELS], ADDR_LABELS);
}

void SegLCD_PCF85176_T1T2Lcd::setT1T2Labels(uint8_t t1t2) {

    if (t1t2 & SegLCD_PCF85176_T1T2Lcd::LABEL_T1)
      _ramBuffer[OFFSET_CLOCK] |= SYMBOL_T1T2_MASK;

    if (t1t2 & SegLCD_PCF85176_T1T2Lcd::LABEL_T2)
      _ramBuffer[OFFSET_CLOCK + 1] |= SYMBOL_T1T2_MASK;

    _writeRam(&_ramBuffer[OFFSET_CLOCK], 2, ADDR_CLOCK_T1T2_LABELS_SEGS);
}

void SegLCD_PCF85176_T1T2Lcd::clearT1T2Labels(uint8_t t1t2) {

    if (t1t2 & SegLCD_PCF85176_T1T2Lcd::LABEL_T1)
      _ramBuffer[OFFSET_CLOCK] &= ~SYMBOL_T1T2_MASK;

    if (t1t2 & SegLCD_PCF85176_T1T2Lcd::LABEL_T2)
      _ramBuffer[OFFSET_CLOCK + 1] &= ~SYMBOL_T1T2_MASK;

    _writeRam(&_ramBuffer[OFFSET_CLOCK], 2, ADDR_CLOCK_T1T2_LABELS_SEGS);
}

void SegLCD_PCF85176_T1T2Lcd::_setColon(uint8_t row, uint8_t col, bool state) {
    if (row != ROW_CLOCK || col != 2) {
        return;
    }

    if (state)
        _ramBuffer[OFFSET_SIGCLK] |= 0x02;
    else
        _ramBuffer[OFFSET_SIGCLK] &= ~0x02;

    _writeRam(_ramBuffer[OFFSET_SIGCLK], ADDR_SIGNAL_CLOCK);
}

void SegLCD_PCF85176_T1T2Lcd::_setDecimal(uint8_t row, uint8_t col, bool state) {

    if (row < DECIMAL_MIN_ROW || row > DECIMAL_MAX_ROW) {
        return; // Invalid digit
    }

    if (col > DECIMAL_MAX_COL) {
        return; // Invalid digit
    }

    uint8_t address = 0;
    uint8_t offset = 0;
    switch (row) {
        case 1:
            address = ADDR_T1_SEGS + (col * 2);
            offset = OFFSET_T1;
            break;
        case 2:
            address = ADDR_T2_SEGS + (col * 2);
            offset = OFFSET_T2;
            break;
        default:
            return; // Invalid section
    }

    if (state) {
        _ramBuffer[offset + col] |= DECIMAL_POINT_BIT; // Set the decimal point bit
    } else {
        _ramBuffer[offset + col] &= ~DECIMAL_POINT_BIT; // Clear the decimal point bit
    }

    _writeRam(_ramBuffer[offset + col], address);
}

void SegLCD_PCF85176_T1T2Lcd::setCursor(uint8_t row, uint8_t col) {
    if (row == ROW_CLOCK && col < 2) {
        _clearFlag(FLAG_COLON_CLOCK);
    }

    SegDriver_PCF85176::setCursor(row, col);
}

size_t SegLCD_PCF85176_T1T2Lcd::write(uint8_t ch) {
    if (_cursorRow > ROW_T2) {
        return 0;
    }

    if (_cursorRow == ROW_CLOCK && _cursorCol > 4) {
        _cursorRow = ROW_T1;
        _cursorCol = 0;
    } else if (_cursorRow == ROW_T1 && _cursorCol > 3) {
        _cursorRow = ROW_T2;
        _cursorCol = 0;
    } else if (_cursorRow == ROW_T2 && _cursorCol > 3) {
        return 0;
    }

    uint8_t segment_data = _mapSegments(_get_char_value(ch));

    switch (_cursorRow) {
        case ROW_CLOCK:
            // Handle colon
            if (ch == ':' && _cursorCol == 2) {
                if (!_isFlagSet(FLAG_COLON_CLOCK)) {
                    _setColon(_cursorRow, 2, true);
                    _setFlag(FLAG_COLON_CLOCK);
                }
                return 1;
            }

            // Clear colon only when cursor reaches the digit just before it.
            if (ch != ':' && _cursorCol == 1) {
                _setColon(_cursorRow, 2, false);
                _clearFlag(FLAG_COLON_CLOCK);
            }

            if (_cursorCol < 4) {
                _ramBuffer[OFFSET_CLOCK + _cursorCol] &= ~0b11111110;
                _ramBuffer[OFFSET_CLOCK + _cursorCol] |= segment_data & 0b11111110;
                _writeRam(_ramBuffer[OFFSET_CLOCK + _cursorCol], ADDR_CLOCK_T1T2_LABELS_SEGS + (_cursorCol * 2));
            }

            if (_cursorCol == 4) {
                _ramBuffer[OFFSET_DAY] = segment_data;
                _writeRam(segment_data, ADDR_DAY_SEG);
            }
            break;
        case ROW_T1:
            // Handle decimal point
            if (_dotWrite(ch, DECIMAL_MIN_COL, DECIMAL_MAX_COL, DECIMAL_COL_OFFSET)) {
                return 1;
            }
            if (_isFlagSet(FLAG_PENDING_DOT)) {
                _clearFlag(FLAG_PENDING_DOT);
            }
            if (_cursorCol <= DECIMAL_MAX_COL) {
                _setDecimal(_cursorRow, _cursorCol, false);
            }

            _ramBuffer[OFFSET_T1 + _cursorCol] = segment_data;
            _writeRam(_ramBuffer[OFFSET_T1 + _cursorCol], ADDR_T1_SEGS + (_cursorCol * 2));
            break;
        case ROW_T2:
            // Handle decimal point
            if (_dotWrite(ch, DECIMAL_MIN_COL, DECIMAL_MAX_COL, DECIMAL_COL_OFFSET)) {
                return 1;
            }
            if (_isFlagSet(FLAG_PENDING_DOT)) {
                _clearFlag(FLAG_PENDING_DOT);
            }
            if (_cursorCol <= DECIMAL_MAX_COL) {
                _setDecimal(_cursorRow, _cursorCol, false);
            }

            _ramBuffer[OFFSET_T2 + _cursorCol] = segment_data;
            _writeRam(_ramBuffer[OFFSET_T2 + _cursorCol], ADDR_T2_SEGS + (_cursorCol * 2));
            break;
    }
    _cursorCol++;
    return 1;
}

// ABCD_EFGH to FGED ABCH..
uint8_t SegLCD_PCF85176_T1T2Lcd::_mapSegments(uint8_t val) {
    uint8_t out = 0;
    out |= (val & 0b11100000) >> 4;   // ABC
    out |= (val & 0b00010000);        // D
    out |= (val & 0b00001000) << 2;   // E
    out |= (val & 0b00000110) << 5;   // FG
    out |= (val & 0b00000001);        // H
    return out;
}
