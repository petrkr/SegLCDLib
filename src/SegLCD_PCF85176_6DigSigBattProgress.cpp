#include <Wire.h>
#include <SegLCD_PCF85176_6DigSigBattProgress.h>


SegLCD_PCF85176_6DigitSignalBatteryProgress::SegLCD_PCF85176_6DigitSignalBatteryProgress(TwoWire& i2c, uint8_t address, uint8_t subaddress) :  SegDriver_PCF85176(i2c, address, subaddress) {}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::init() {
    SegDriver_PCF85176::init();
    _setMode(MODE_STATUS_ENABLED);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::clear() {
    _buffer_sigbatt = 0x00;
    _buffer_labels = 0x00;
    memset(_buffer_top, 0x00, sizeof(_buffer_top));
    memset(_buffer_default, 0x00, sizeof(_buffer_default));
    SegDriver_PCF85176::clear();
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setBatteryLevel(uint8_t level) {
    if (level > 4)
        level = 4;

    _buffer_sigbatt &= ~(0x0f);

    if (level > 0)
        _buffer_sigbatt |= 8;
    if (level > 1)
        _buffer_sigbatt |= 1;
    if (level > 2)
        _buffer_sigbatt |= 2;
    if (level > 3)
        _buffer_sigbatt |= 4;

    _writeRam(_buffer_sigbatt, ADDR_SIGNAL_BATT);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setSignalLevel(uint8_t level) {
    if (level > 4)
        level = 4;

    _buffer_sigbatt &= ~(0xf0);

    if (level > 0)
        _buffer_sigbatt |= 0x80;
    if (level > 1)
        _buffer_sigbatt |= 0x40;
    if (level > 2)
        _buffer_sigbatt |= 0x20;
    if (level > 3)
        _buffer_sigbatt |= 0x10;

    _writeRam(_buffer_sigbatt, ADDR_SIGNAL_BATT);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setProgress(uint8_t value) {
    uint8_t buffer[2] = { 0, 0 };

    if (value > 0) {
        buffer[0] |= 0x80;
    }
    if (value >= 10) {
        buffer[0] |= 0x40;
    }
    if (value >= 20) {
        buffer[0] |= 0x20;
    }
    if (value >= 30) {
        buffer[0] |= 0x10;
    }
    if (value >= 40) {
        buffer[0] |= 0x08;
    }
    if (value >= 50) {
        buffer[0] |= 0x04;
    }
    if (value >= 60) {
        buffer[0] |= 0x02;
    }
    if (value >= 70) {
        buffer[0] |= 0x01;
    }

    if (value >= 80) {
        buffer[1] |= 0x80;
    }
    if (value >= 90) {
        buffer[1] |= 0x40;
    }
    if (value >= 100) {
        buffer[1] |= 0x20;
    }
    if (value >= 110) {
        buffer[1] |= 0x10;
    }
    if (value >= 120) {
        buffer[1] |= 0x08;
    }
    if (value >= 130) {
        buffer[1] |= 0x04;
    }
    if (value >= 140) {
        buffer[1] |= 0x02;
    }
    if (value == 150) {
        buffer[1] |= 0x01;
    }

    _writeRam(buffer, sizeof(buffer), ADDR_PROGRESS);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setWheel(uint16_t value) {
    uint8_t buffer[2] = { 0, 0 };
    
    buffer[0] = value & 0xFF;
    buffer[1] = (((value >> 8) & 0x0F) << 4);

    _writeRam(buffer, sizeof(buffer), ADDR_WHEEL);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setLabels(LabelFlags labels) {
    _buffer_labels |= labels;
    
    _writeRam(_buffer_labels, ADDR_PRES_LABELS);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::clearLabels(LabelFlags labels) {
    _buffer_labels &= ~labels;
    
    _writeRam(_buffer_labels, ADDR_PRES_LABELS);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setClockColon(uint8_t row, uint8_t col, bool state) {
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

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setDecimal(uint8_t row, uint8_t col, bool state) {
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

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setCursor(uint8_t row, uint8_t col) {
    if (row == 0 && col < 2) {
        _colon_top = false;
    }

    if (row == 1 && col < 5) {
        _colon_default = false;
    }

    SegDriver_PCF85176::setCursor(row, col);
}

size_t SegLCD_PCF85176_6DigitSignalBatteryProgress::write(uint8_t ch) {
    uint8_t segment_data = _mapSegments(_get_char_value(ch));

    switch (_cursorRow) {
        case 0:
            if (_cursorCol < 0 || _cursorCol > 3) {
                return 0; // Invalid digit
            }

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
            _writeRam(_buffer_default[_cursorCol], ADDR_BIG_SEGS + ((6 - _cursorCol - 1) * 2));
            break;
        default:
            return 0; // invalid digit
    }

    _cursorCol++;
    return 1;
}

// ABCD_EFGH to ABCH FGED
uint8_t SegLCD_PCF85176_6DigitSignalBatteryProgress::_mapSegments(uint8_t val) {
    uint8_t out = 0;
    out |= (val & 0b11100000);           // ABC: bits 7-5 → 7-5
    out |= (val & 0b00010000) >> 4;      // D: bit 4 → 0
    out |= (val & 0b00001000) >> 2;      // E: bit 3 → 1
    out |= (val & 0b00000100) << 1;      // F: bit 2 → 3
    out |= (val & 0b00000010) << 1;      // G: bit 1 → 2
    out |= (val & 0b00000001) << 4;      // H: bit 0 → 4
    return out;
}