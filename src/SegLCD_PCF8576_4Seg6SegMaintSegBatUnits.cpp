#include <Wire.h>
#include <SegLCD_PCF8576_4Seg6SegMaintSegBatUnits.h>


SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::SegLCD_PCF8576_4Seg6SegMaintSegBatUnits(TwoWire& i2c, uint8_t address, uint8_t subaddress) :  SegDriver_PCF8576(i2c, address, subaddress) {}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::init() {
    SegDriver_PCF85176::init();
    _setMode(MODE_STATUS_ENABLED);
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::clear() {
    memset(_buffer, 0x00, sizeof(_buffer));
    _buffer_batt = 0x00;
    _buffer_sig = 0x00;
    memset(_buffer_labels, 0x00, sizeof(_buffer_labels));
    memset(_buffer_top, 0x00, sizeof(_buffer_top));
    memset(_buffer_default, 0x00, sizeof(_buffer_default));
    SegDriver_PCF85176::clear();
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::setBatteryLevel(uint8_t level) {
    if (level > 4)
        level = 4;

    _buffer_batt &= ~(0x0f);

    if (level > 0)
        _buffer_batt |= 8;
    if (level > 1)
        _buffer_batt |= 1;
    if (level > 2)
        _buffer_batt |= 2;
    if (level > 3)
        _buffer_batt |= 4;

    _writeRam(_buffer_batt, ADDR_BATT);
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::setSignalLevel(uint8_t level) {
    if (level > 4)
        level = 4;

    _buffer_sig &= ~(0x0f);

    if (level > 0)
        _buffer_sig |= 0x08;
    if (level > 1)
        _buffer_sig |= 0x04;
    if (level > 2)
        _buffer_sig |= 0x02;
    if (level > 3)
        _buffer_sig |= 0x01;

    _writeRam(_buffer_sig, ADDR_SIGNAL);
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::setLabels(LabelFlags labels) {
    //_buffer_labels |= labels;

    //_writeRam(_buffer_labels, ADDR_LABELS);
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::clearLabels(LabelFlags labels) {
    //_buffer_labels &= ~labels;

    //_writeRam(_buffer_labels, ADDR_LABELS);
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::setClockColon(uint8_t row, uint8_t col, bool state) {
    uint8_t address = 0;
    uint8_t digit = 0;
    uint8_t* _buffer = nullptr;
    switch (row) {
        case 0:
            if (col != 2) {
                return; // Invalid digit
            }
            digit = 4;
            address = ADDR_SMALL_SEGS + 6; // Digit 4
            _buffer = _buffer_top;
            break;
        case 1:
            if (col != 4) {
                return; // Invalid digit
            }

            address = ADDR_BIG_SEGS; // Digit 6
            _buffer = _buffer_default;
            digit = 6;
            break;
        default:
            return; // Invalid section
    }

    if (state) {
        _buffer[(digit-1)] |= 0x10; // Set the decimal point bit
    } else {
        _buffer[(digit-1)] &= ~0x10; // Clear the decimal point bit
    }

    _writeRam(_buffer[(digit-1)], address);
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::setDecimal(uint8_t row, uint8_t col, bool state) {
    uint8_t address = 0;
    uint8_t* _buffer = nullptr;
    switch (row) {
        case 0:
            if (col < 0 || col > 2) {
                return; // Invalid digit
            }

            address = ADDR_SMALL_SEGS + (col * 2);
            _buffer = _buffer_top;
            break;
        case 1:
            if (col < 0 || col > 4) {
                return; // Invalid digit
            }

            address = ADDR_BIG_SEGS + ((6 - col - 1) * 2);
            _buffer = _buffer_default;
            break;
        default:
            return; // Invalid section
    }

    if (state) {
        _buffer[col] |= DECIMAL_POINT_BIT; // Set the decimal point bit
    } else {
        _buffer[col] &= ~DECIMAL_POINT_BIT; // Clear the decimal point bit
    }

    _writeRam(_buffer[col], address);
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::setCursor(uint8_t row, uint8_t col) {
    if (row == 0 && col < 2) {
        _colon_top = false;
    }

    if (row == 1 && col < 5) {
        _colon_default = false;
    }

    SegDriver_PCF85176::setCursor(row, col);
}

size_t SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::write(uint8_t ch) {
    uint8_t segment_data = 0x00;
    uint8_t addr = 0x00;
    uint8_t col = 0x00;

    Serial.println(_cursorRow);
    Serial.println(_cursorCol);
    Serial.println(ch);
    Serial.println();
    

    switch (_cursorRow) {
        case 0:
            if (_cursorCol < 0 || _cursorCol > 3) {
                return 0; // Invalid digit
            }
            segment_data = _mapSegmentsTop(_get_char_value(ch));

            if (_cursorCol == 2 && ch != ':' && !_colon_top && (_buffer_top[_cursorCol] & DECIMAL_POINT_BIT)) {
                _buffer_top[_cursorCol] &= ~DECIMAL_POINT_BIT;
                _writeRam(_buffer_top[_cursorCol], ADDR_SMALL_SEGS + ((_cursorCol) * 2));
            }

            if (ch == '.') {
                setDecimal(_cursorRow, _cursorCol - 1, true);
                return 1;
            }

            if (ch == ':' && _cursorCol == 2 && !_colon_top) {
                setClockColon(_cursorRow, _cursorCol, true);
                _colon_top = true;
                return 1;
            }

            if (_cursorCol == 3 && _colon_top) {
                _buffer_top[_cursorCol] &= DECIMAL_POINT_BIT;
                _buffer_top[_cursorCol] |= (segment_data & ~DECIMAL_POINT_BIT);
            } else {
                _buffer_top[_cursorCol] = segment_data;
            }
            _writeRam(_buffer_top[_cursorCol], ADDR_SMALL_SEGS + ((_cursorCol) * 2));
            break;
        case 1:
            if (_cursorCol < 0 || _cursorCol > 5) {
                return 0; // Invalid digit
            }
            segment_data = _mapSegments(_get_char_value(ch));

            if (_cursorCol == 4 && ch != ':' && !_colon_default && (_buffer_default[_cursorCol] & DECIMAL_POINT_BIT)) {
                _buffer_default[_cursorCol] &= ~DECIMAL_POINT_BIT;
                _writeRam(_buffer_default[_cursorCol], ADDR_BIG_SEGS + ((6 - _cursorCol - 1) * 2));
            }

            if (ch == '.') {
                setDecimal(_cursorRow, _cursorCol - 1, true);
                return 1;
            }

            if (ch == ':' && _cursorCol == 4 && !_colon_default) {
                setClockColon(_cursorRow, _cursorCol, true);
                _colon_default = true;
                return 1;
            }

            if (_cursorCol == 5 && _colon_default) {
                _buffer_default[_cursorCol] &= DECIMAL_POINT_BIT;
                _buffer_default[_cursorCol] |= (segment_data & ~DECIMAL_POINT_BIT);
            } else {
                _buffer_default[_cursorCol] = segment_data;
            }

            col = (uint8_t)(5 - _cursorCol);
            addr = (uint8_t)(ADDR_BIG_SEGS + (col * 2));
            if (addr >= 17) { addr += 2; } // Skip labels
            _writeRam(_buffer_default[_cursorCol], addr);
            break;
        default:
            return 0; // invalid digit
    }

    _cursorCol++;
    return 1;
}

// ABCD EFGH to 
uint8_t SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::_mapSegmentsTop(uint8_t val) {
    uint8_t out = 0;
    out |= (val & 0b00011000) << 3;      // DE: bits 4-3 -> 7-6
    out |= (val & 0b00000100) << 2;      // F: bit 2 -> 4
    out |= (val & 0b00000010) << 4;      // G: bit 1 -> 5
    out |= (val & 0b00100000) >> 3;      // C: bit 5 -> 2
    out |= (val & 0b01000000) >> 5;      // B: bit 6 -> 1
    out |= (val & 0b10000000) >> 7;      // A: bit 7 -> 0
    return out;
}

uint8_t SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::_mapSegments(uint8_t val)
{
    uint8_t out = 0;
    out |= (val & 0b11100000);        // A,B,C: bits 7-5 -> 7-5
    out |= (val & 0b00000100) << 1;   // F: bit2 -> bit3
    out |= (val & 0b00000010) << 1;   // G: bit1 -> bit2
    out |= (val & 0b00001000) >> 2;   // E: bit3 -> bit1
    out |= (val & 0b00010000) >> 4;   // D: bit4 -> bit0
    return out;
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::_setRamMasked(uint8_t addr, uint8_t mask, uint8_t value) {
    uint8_t byteIndex = addr >> 1;
    bool high = (addr & 0x01) != 0;
    uint8_t m = mask & 0x0F;
    uint8_t v = value & 0x0F;

    if (high) {
        uint8_t cur = (_buffer[byteIndex] >> 4) & 0x0F;
        cur = (cur & ~m) | (v & m);
        _buffer[byteIndex] = (_buffer[byteIndex] & 0x0F) | (cur << 4);
    } else {
        uint8_t cur = _buffer[byteIndex] & 0x0F;
        cur = (cur & ~m) | (v & m);
        _buffer[byteIndex] = (_buffer[byteIndex] & 0xF0) | cur;
    }
}



// For generic class allow access to low level functions
void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::writeRam(uint8_t data, uint8_t address) {
    _writeRam(data, address);
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::writeRam(uint8_t *data, size_t length, uint8_t address) {
    _writeRam(data, length, address);
}
