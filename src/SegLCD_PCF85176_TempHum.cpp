#include <Wire.h>
#include <SegLCD_PCF85176_TempHum.h>


SegLCD_PCF85176_TempHumidity::SegLCD_PCF85176_TempHumidity(TwoWire& i2c, uint8_t address, uint8_t subaddress) :  SegDriver_PCF85176(i2c, address, subaddress) {}

void SegLCD_PCF85176_TempHumidity::init() {
    SegDriver_PCF85176::init();
    _setMode(MODE_STATUS_ENABLED);
}

void SegLCD_PCF85176_TempHumidity::clear() {
    _buffer_sigbatt = 0x00;
    memset(_buffer_temp, 0x00, sizeof(_buffer_temp));
    memset(_buffer_hum, 0x00, sizeof(_buffer_hum));
    _specialMinusDisplayed = false;
    SegDriver_PCF85176::clear();
}

void SegLCD_PCF85176_TempHumidity::setBatteryLevel(uint8_t level) {
    if (level > MAX_SIGNAL_BATTERY_LEVEL)
        level = MAX_SIGNAL_BATTERY_LEVEL;

    _buffer_sigbatt &= ~SIGNAL_BATTERY_MASK_LOW;

    if (level > 0)
        _buffer_sigbatt |= BATTERY_LEVEL_BITS[0];
    if (level > 1)
        _buffer_sigbatt |= BATTERY_LEVEL_BITS[1];
    if (level > 2)
        _buffer_sigbatt |= BATTERY_LEVEL_BITS[2];
    if (level > 3)
        _buffer_sigbatt |= BATTERY_LEVEL_BITS[3];

    _writeRam(_buffer_sigbatt, ADDR_SIGNAL_BATT);
}

void SegLCD_PCF85176_TempHumidity::setSignalLevel(uint8_t level) {
    if (level > MAX_SIGNAL_BATTERY_LEVEL)
        level = MAX_SIGNAL_BATTERY_LEVEL;

    _buffer_sigbatt &= ~SIGNAL_BATTERY_MASK_HIGH;

    if (level > 0)
        _buffer_sigbatt |= SIGNAL_LEVEL_BITS[0];
    if (level > 1)
        _buffer_sigbatt |= SIGNAL_LEVEL_BITS[1];
    if (level > 2)
        _buffer_sigbatt |= SIGNAL_LEVEL_BITS[2];
    if (level > 3)
        _buffer_sigbatt |= SIGNAL_LEVEL_BITS[3];

    _writeRam(_buffer_sigbatt, ADDR_SIGNAL_BATT);
}

void SegLCD_PCF85176_TempHumidity::setLabels(uint8_t labels) {
    if (labels & LABEL_C) {
        _buffer_temp[3] |= LABEL_BIT; // Set the C label
        _writeRam(_buffer_temp[3], ADDR_TEMP_SEGS + TEMP_LABEL_OFFSET); // C label is at position 4 (3 in zero-based index)
    }

    if (labels & LABEL_PROC) {
        _buffer_hum[2] |= LABEL_BIT; // Set the proc symbol
        _writeRam(_buffer_hum[2], ADDR_HUM_SEGS + HUM_LABEL_OFFSET); // proc symbol is at position 3 (2 in zero-based index)
    }
}

void SegLCD_PCF85176_TempHumidity::clearLabels(uint8_t labels) {
    if (labels & LABEL_C) {
        _buffer_temp[3] &= ~LABEL_BIT; // Clear the C label
        _writeRam(_buffer_temp[3], ADDR_TEMP_SEGS + TEMP_LABEL_OFFSET); // C label is at position 4 (3 in zero-based index)
    }

    if (labels & LABEL_PROC) {
        _buffer_hum[2] &= ~LABEL_BIT; // Clear the proc symbol
        _writeRam(_buffer_hum[2], ADDR_HUM_SEGS + HUM_LABEL_OFFSET); // proc symbol is at position 3 (2 in zero-based index)
    }
}

void SegLCD_PCF85176_TempHumidity::setDecimal(uint8_t row, uint8_t col, bool state) {
    uint8_t address = 0;
    uint8_t* _buffer = nullptr;

    switch (row) {
    case TEMP_ROW: // Temp segments
        if (col < TEMP_DECIMAL_MIN_COL || col > TEMP_DECIMAL_MAX_COL) {
            return; // Invalid digit
        }

        address = ADDR_TEMP_SEGS + ((col - 1) * 2);
        _buffer = _buffer_temp;
        break;


    case HUM_ROW: // Hum segments
        if (col != HUM_DECIMAL_COL) {
            return; // Invalid digit
        }

        address = ADDR_HUM_SEGS + ((col - 1) * 2);
        _buffer = _buffer_hum;
        break;

    default:
        return; // Invalid selection
    }

    if (state) {
        _buffer[(col-1)] |= DECIMAL_POINT_BIT; // Set the decimal point bit
    } else {
        _buffer[(col-1)] &= ~DECIMAL_POINT_BIT; // Clear the decimal point bit
    }

    _writeRam(_buffer[(col-1)], address);
}

void SegLCD_PCF85176_TempHumidity::setCursor(uint8_t row, uint8_t col) {
    if (row == TEMP_ROW && col == 0) {
        _specialMinusDisplayed = false;
    }

    SegDriver_PCF85176::setCursor(row, col);
}

size_t SegLCD_PCF85176_TempHumidity::write(uint8_t ch) {
    uint8_t c = _mapSegments(_get_char_value(ch));

    if (_cursorRow == TEMP_ROW) { // Temp segments
        if (_cursorCol == 0 && ch != '-' && !_specialMinusDisplayed && (_buffer_hum[0] & MINUS_SIGN_BIT)) {
            _buffer_hum[0] &= ~MINUS_SIGN_BIT;
            _writeRam(_buffer_hum[0], ADDR_HUM_SEGS);
        }

        if (ch == '.') {
            setDecimal(_cursorRow, _cursorCol, true);
            return 1;
        } else if (ch == '-' && _cursorCol == 0 && !_specialMinusDisplayed) {
            _buffer_hum[0] |= MINUS_SIGN_BIT;
            _writeRam(_buffer_hum[0], ADDR_HUM_SEGS);
            _specialMinusDisplayed = true;
            return 1;
        } else if (_cursorCol >= 0 && _cursorCol < TEMP_DIGITS) {
            _buffer_temp[_cursorCol] = c;
            _writeRam(_buffer_temp[_cursorCol], ADDR_TEMP_SEGS + _cursorCol * 2);
            _cursorCol++;
            return 1;
        }
    } else if (_cursorRow == HUM_ROW) { // Humidity segments
        if (ch == '.') {
            setDecimal(_cursorRow, _cursorCol, true);
            return 1;
        } else if (_cursorRow == HUM_ROW && _cursorCol >= 0 && _cursorCol < HUM_DIGITS) {
            if (_cursorCol == 0) {
                _buffer_hum[_cursorCol] &= MINUS_SIGN_BIT;
                _buffer_hum[_cursorCol] |= (c & ~MINUS_SIGN_BIT);
            }
            else {
                _buffer_hum[_cursorCol] = c;
            }
            _writeRam(_buffer_hum[_cursorCol], ADDR_HUM_SEGS + _cursorCol * 2);
            _cursorCol++;
            return 1;
        }
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
