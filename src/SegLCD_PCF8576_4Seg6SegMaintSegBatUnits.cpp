#include <SegLCD_PCF8576_4Seg6SegMaintSegBatUnits.h>


SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::SegLCD_PCF8576_4Seg6SegMaintSegBatUnits(TwoWire& i2c, uint8_t address, uint8_t subaddress) :  SegDriver_PCF8576(i2c, address, subaddress) {
    _allocateBuffer(RAM_BYTE_COUNT);
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::init() {
    SegDriver_PCF8576::init();
    _setMode(MODE_STATUS_ENABLED);
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::clear() {
    memset(_buffer_default, 0x00, sizeof(_buffer_default));
    SegDriver_PCF8576::clear();
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::setBatteryLevel(uint8_t level) {
    uint8_t data = 0x00;
    if (level > 5)
        level = 5;

    if (level > 0)
        data |= 0x10;  // W1 (0x08 COM4)
    if (level > 1)
        data |= 0x08;  // W5 (0x09 COM1)
    if (level > 2)
        data |= 0x04;  // W4 (0x09 COM2)
    if (level > 3)
        data |= 0x02;  // W3 (0x09 COM3)
    if (level > 4)
        data |= 0x01;  // W2 (0x09 COM4)

    _writeRamMasked(data, ADDR_BATT, 0x1F);
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::setSignalLevel(uint8_t level) {
    uint8_t data = 0x00;
    if (level > 4)
        level = 4;

    if (level > 0)
        data |= 0x80;
    if (level > 1)
        data |= 0x40;
    if (level > 2)
        data |= 0x20;
    if (level > 3)
        data |= 0x10;

    _writeRamMasked(data, ADDR_SIGNAL, 0xF0);
}

uint8_t SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::_convertLabelFlags(uint8_t labels) {
    uint8_t label_bits = 0;
    if (labels & LABEL_MPA)  label_bits |= 0x01;
    if (labels & LABEL_KPA)  label_bits |= 0x02;
    if (labels & LABEL_UEPS) label_bits |= 0x04;
    if (labels & LABEL_MM)   label_bits |= 0x08;
    if (labels & LABEL_MV)   label_bits |= 0x10;
    if (labels & LABEL_NM)   label_bits |= 0x20;
    if (labels & LABEL_KG)   label_bits |= 0x40;
    if (labels & LABEL_G)    label_bits |= 0x80;
    return label_bits;
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::setLabels(uint8_t labels) {
    uint8_t label_bits = _convertLabelFlags(labels);
    _writeRamMasked(label_bits, ADDR_LABELS, label_bits);
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::clearLabels(uint8_t labels) {
    uint8_t label_bits = _convertLabelFlags(labels);
    _writeRamMasked(0x00, ADDR_LABELS, label_bits);
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::setDegree(bool state) {
    _writeRamMasked(state ? 0x10 : 0x00, 0x0B, 0x10);
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::setMaintenance(bool state) {
    _writeRamMasked(state ? 0x20 : 0x00, 0x08, 0x20);
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::_setColon(uint8_t row, uint8_t col, bool state) {
    switch (row) {
        case 0:
            if (col != 2) {
                return; // Invalid digit
            }
            _writeRamMasked(state ? 0x80 : 0x00, 0x07, 0x80);
            if (state) _setFlag(FLAG_COLON_TOP); else _clearFlag(FLAG_COLON_TOP);
            break;
        case 1:
            if (col != 2 && col != 4) {
                return; // Invalid digit
            }

            if (col == 2) {
                _writeRamMasked(state ? 0x80 : 0x00, 0x08, 0x80);
                if (state) _setFlag(FLAG_COLON_DEFAULT_LEFT); else _clearFlag(FLAG_COLON_DEFAULT_LEFT);
            } else {
                _writeRamMasked(state ? 0x40 : 0x00, 0x08, 0x40);
                if (state) _setFlag(FLAG_COLON_DEFAULT_RIGHT); else _clearFlag(FLAG_COLON_DEFAULT_RIGHT);
            }
            break;
        default:
            return; // Invalid section
    }
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::_setDecimal(uint8_t row, uint8_t col, bool state) {
    uint8_t address = 0;
    uint8_t mask = 0xFF;
    uint8_t dp_mask = 0;
    uint8_t current = 0;
    switch (row) {
        case 0:
            if (col > 2) {
                return; // Invalid digit
            }

            address = ADDR_SMALL_SEGS + (col * 2);
            dp_mask = 0x08;
            current = _ramBuffer[address >> 1];
            break;
        case 1:
            if (col > 4) {
                return; // Invalid digit
            }

            address = ADDR_BIG_SEGS + ((6 - col - 1) * 2);
            if (address >= 0x11) {
                address += 2; // Skip labels at 0x11/0x12
            }
            if (address == 0x0B) {
                return; // No decimal point on this digit
            }
            dp_mask = 0x10;
            current = _buffer_default[col];
            break;
        default:
            return; // Invalid section
    }

    if (state) {
        current |= dp_mask; // Set the decimal point bit
    } else {
        current &= ~dp_mask; // Clear the decimal point bit
    }

    if (address == 0x06) {
        mask &= 0xF7;
    } else if (address == 0x0B) {
        mask &= 0xEF;
    }

    _writeRamMasked(current, address, mask);
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::setCursor(uint8_t row, uint8_t col) {
    if (row == 0 && col <= 2) {
        _clearFlag(FLAG_COLON_TOP);
    }

    if (row == 1) {
        if (col <= 2) {
            _clearFlag(FLAG_COLON_DEFAULT_LEFT);
        }
        if (col <= 4) {
            _clearFlag(FLAG_COLON_DEFAULT_RIGHT);
        }
    }

    SegDriver_PCF8576::setCursor(row, col);
}

size_t SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::_writeRow0(uint8_t ch) {
    if (_cursorCol >= 4) {
        return 0;  // Invalid digit
    }

    // Handle decimal point - only set, don't clear previous
    if (_dotWrite(ch, DECIMAL_TOP_MIN_COL, DECIMAL_TOP_MAX_COL, DECIMAL_COL_OFFSET)) {
        if (_cursorCol == COLON_TOP_COL) {
            _setColon(_cursorRow, COLON_TOP_COL, false);
        }
        return 1;
    }

    // Handle colon only at dedicated HW position
    if (ch == ':' && _cursorCol == COLON_TOP_COL) {
        if (!_isFlagSet(FLAG_COLON_TOP)) {
            _setColon(_cursorRow, COLON_TOP_COL, true);
        }
        _setDecimal(_cursorRow, _cursorCol - 1, false);
        return 1;
    }

    // Clear colon when rewriting the digit just before it.
    // This includes ':' glyph written as a regular character at col 1.
    if (_cursorCol == (COLON_TOP_COL - 1)) {
        _setColon(_cursorRow, COLON_TOP_COL, false);
        _clearFlag(FLAG_COLON_TOP);
    }

    // Clear decimal on current column when writing regular character
    if (_cursorCol >= DECIMAL_TOP_MIN_COL && _cursorCol <= DECIMAL_TOP_MAX_COL) {
        _setDecimal(_cursorRow, _cursorCol, false);
    }

    // Regular character
    uint8_t segment_data = _mapSegmentsTop(_get_char_value(ch));
    uint8_t addr = ADDR_SMALL_SEGS + (_cursorCol * 2);

    _writeRamMasked(segment_data, addr, addr == 0x06 ? 0xF7 : 0xFF);
    _cursorCol++;
    return 1;
}

size_t SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::_writeRow1(uint8_t ch) {
    if (_cursorCol >= 6) {
        return 0;  // Invalid digit
    }

    // Handle decimal point - only set, don't clear previous
    if (_dotWrite(ch, DECIMAL_BOTTOM_MIN_COL, DECIMAL_BOTTOM_MAX_COL, DECIMAL_COL_OFFSET)) {
        if (_cursorCol == COLON_BOTTOM_LEFT_COL) {
            _setColon(_cursorRow, COLON_BOTTOM_LEFT_COL, false);
        }
        if (_cursorCol == COLON_BOTTOM_RIGHT_COL) {
            _setColon(_cursorRow, COLON_BOTTOM_RIGHT_COL, false);
        }
        return 1;
    }

    // Handle colon at col 2
    if (ch == ':' && _cursorCol == COLON_BOTTOM_LEFT_COL) {
        if (!_isFlagSet(FLAG_COLON_DEFAULT_LEFT)) {
            _setColon(_cursorRow, COLON_BOTTOM_LEFT_COL, true);
        }
        _setDecimal(_cursorRow, _cursorCol - 1, false);
        return 1;
    }

    // Handle colon at col 4
    if (ch == ':' && _cursorCol == COLON_BOTTOM_RIGHT_COL) {
        if (!_isFlagSet(FLAG_COLON_DEFAULT_RIGHT)) {
            _setColon(_cursorRow, COLON_BOTTOM_RIGHT_COL, true);
        }
        _setDecimal(_cursorRow, _cursorCol - 1, false);
        return 1;
    }

    // Clear colons when rewriting the digit just before each separator colon.
    if (_cursorCol == 1) {
        _setColon(1, 2, false);
        _clearFlag(FLAG_COLON_DEFAULT_LEFT);
    }
    if (_cursorCol == 3) {
        _setColon(1, 4, false);
        _clearFlag(FLAG_COLON_DEFAULT_RIGHT);
    }

    // Clear decimal on current column when writing regular character
    if (_cursorCol >= DECIMAL_BOTTOM_MIN_COL && _cursorCol <= DECIMAL_BOTTOM_MAX_COL) {
        _setDecimal(_cursorRow, _cursorCol, false);
    }

    // Regular character
    uint8_t segment_data = _mapSegments(_get_char_value(ch));
    _buffer_default[_cursorCol] = segment_data;

    uint8_t col = 5 - _cursorCol;
    uint8_t addr = ADDR_BIG_SEGS + (col * 2);
    if (addr >= 0x11) addr += 2;

    _writeRamMasked(_buffer_default[_cursorCol], addr,
                   addr == 0x0B ? 0xEF : 0xFF);

    _cursorCol++;
    return 1;
}

size_t SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::write(uint8_t ch) {
    switch (_cursorRow) {
        case 0:
            return _writeRow0(ch);
        case 1:
            return _writeRow1(ch);
        default:
            return 0;
    }
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
