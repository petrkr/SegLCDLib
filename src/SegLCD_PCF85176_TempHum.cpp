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
    SegDriver_PCF85176::clear();
}

void SegLCD_PCF85176_TempHumidity::setBatteryLevel(uint8_t level) {
    if (level > 4)
        level = 4;

    _buffer_sigbatt &= ~(0x0f);

    if (level > 0)
        _buffer_sigbatt |= 8;
    if (level > 1)
        _buffer_sigbatt |= 4;
    if (level > 2)
        _buffer_sigbatt |= 2;
    if (level > 3)
        _buffer_sigbatt |= 1;

    _writeRam(_buffer_sigbatt, ADDR_SIGNAL_BATT);
}

void SegLCD_PCF85176_TempHumidity::setSignalLevel(uint8_t level) {
    if (level > 4)
        level = 4;

    _buffer_sigbatt &= ~(0xf0);

    if (level > 0)
        _buffer_sigbatt |= 0x10;
    if (level > 1)
        _buffer_sigbatt |= 0x20;
    if (level > 2)
        _buffer_sigbatt |= 0x40;
    if (level > 3)
        _buffer_sigbatt |= 0x80;

    _writeRam(_buffer_sigbatt, ADDR_SIGNAL_BATT);
}

void SegLCD_PCF85176_TempHumidity::setLabels(uint8_t labels) {
    if (labels & LABEL_C) {
        _buffer_temp[3] |= 0x08; // Set the C label
        _writeRam(_buffer_temp[3], ADDR_TEMP_SEGS + 6); // C label is at position 4 (3 in zero-based index)
    }

    if (labels & LABEL_PROC) {
        _buffer_hum[2] |= 0x08; // Set the proc symbol
        _writeRam(_buffer_hum[2], ADDR_HUM_SEGS + 4); // proc symbol is at position 3 (2 in zero-based index)
    }
}

void SegLCD_PCF85176_TempHumidity::clearLabels(uint8_t labels) {
    if (labels & LABEL_C) {
        _buffer_temp[3] &= ~0x08; // Clear the C label
        _writeRam(_buffer_temp[3], ADDR_TEMP_SEGS + 6); // C label is at position 4 (3 in zero-based index)
    }

    if (labels & LABEL_PROC) {
        _buffer_hum[2] &= ~0x08; // Clear the proc symbol
        _writeRam(_buffer_hum[2], ADDR_HUM_SEGS + 4); // proc symbol is at position 3 (2 in zero-based index)
    }
}

void SegLCD_PCF85176_TempHumidity::setDecimal(uint8_t row, uint8_t col, bool state) {
    uint8_t address = 0;
    uint8_t* _buffer = nullptr;

    switch (row) {
    case 0: // Temp segments
        if (col < 1 || col > 3) {
            return; // Invalid digit
        }

        address = ADDR_TEMP_SEGS + ((col - 1) * 2);
        _buffer = _buffer_temp;
        break;


    case 1: // Hum segments
        if (col !=2 ) {
            return; // Invalid digit
        }

        address = ADDR_HUM_SEGS + ((col - 1) * 2);
        _buffer = _buffer_hum;
        break;

    default:
        return; // Invalid selection
    }

    if (state) {
        _buffer[(col-1)] |= 0x08; // Set the decimal point bit
    } else {
        _buffer[(col-1)] &= ~0x08; // Clear the decimal point bit
    }

    _writeRam(_buffer[(col-1)], address);
}

void SegLCD_PCF85176_TempHumidity::setCursor(uint8_t row, uint8_t col) {
    _cursorRow = row;
    _cursorCol = col;
}

size_t SegLCD_PCF85176_TempHumidity::write(uint8_t ch) {
    uint8_t c = _mapSegments(_get_char_value(ch));

    if (_cursorRow == 0) { // Temp segments
        if (ch == '.') {
            if (_cursorCol > 0 && _cursorCol <= 4) {
                _buffer_temp[_cursorCol - 1] |= 0x08;
                _writeRam(_buffer_temp[_cursorCol - 1], ADDR_TEMP_SEGS + ((_cursorCol - 1) * 2));
                return 1;
            }
        } else if (ch == '-' && _cursorRow == 0 && _cursorCol == 0) {
            _buffer_hum[0] |= 0x08;
            _writeRam(_buffer_hum[0], ADDR_HUM_SEGS);
            return 1;
        } else if (_cursorCol >= 0 && _cursorCol < 4) {
            _buffer_temp[_cursorCol] = c;
            _writeRam(_buffer_temp[_cursorCol], ADDR_TEMP_SEGS + _cursorCol * 2);
            _cursorCol++;
            return 1;
        }
    } else if (_cursorRow == 1) { // Humidity segments
        if (ch == '.') {
            if (_cursorCol == 2) {
                _buffer_hum[1] |= 0x08;
                _writeRam(_buffer_hum[1], ADDR_HUM_SEGS + 2);
            }
            return 1;
        } else if (_cursorRow == 1 && _cursorCol >= 0 && _cursorCol < 3) {
            if (_cursorCol == 0) { // This segment has "minus" sign, so we should not remove it by new char
                _buffer_hum[_cursorCol] &= 0x08; // Clear all bits expect minus char
                _buffer_hum[_cursorCol] |= (c & ~0x08); // Set char bits
            }
            else {
                _buffer_hum[_cursorCol] = c;
            }
            _writeRam(_buffer_hum[_cursorCol], ADDR_HUM_SEGS + _cursorCol * 2);
            _cursorCol++;
            return 1;
        }
    }

    return 0; // invalid position
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
