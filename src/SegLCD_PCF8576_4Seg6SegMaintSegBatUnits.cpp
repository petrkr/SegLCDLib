#include <Wire.h>
#include <SegLCD_PCF8576_4Seg6SegMaintSegBatUnits.h>


SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::SegLCD_PCF8576_4Seg6SegMaintSegBatUnits(TwoWire& i2c, uint8_t address, uint8_t subaddress) :  SegDriver_PCF8576(i2c, address, subaddress) {}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::init() {
    SegDriver_PCF8576::init();
    _setMode(MODE_STATUS_ENABLED);
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::clear() {
    memset(_buffer, 0x00, sizeof(_buffer));
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

    _writeRamAtAddr(data, ADDR_BATT, 0x1F);
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

    _writeRamAtAddr(data, ADDR_SIGNAL, 0xF0);
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
    _writeRamAtAddr(label_bits, ADDR_LABELS, label_bits);
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::clearLabels(uint8_t labels) {
    uint8_t label_bits = _convertLabelFlags(labels);
    _writeRamAtAddr(0x00, ADDR_LABELS, label_bits);
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::setDegree(bool state) {
    _writeRamAtAddr(state ? 0x10 : 0x00, 0x0B, 0x10);
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::setMaintenance(bool state) {
    _writeRamAtAddr(state ? 0x20 : 0x00, 0x08, 0x20);
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::setClockColon(uint8_t row, uint8_t col, bool state) {
    switch (row) {
        case 0:
            if (col != 2) {
                return; // Invalid digit
            }
            _writeRamAtAddr(state ? 0x80 : 0x00, 0x07, 0x80);
            _colon_top = state;
            break;
        case 1:
            if (col != 2 && col != 4) {
                return; // Invalid digit
            }

            if (col == 2) {
                _writeRamAtAddr(state ? 0x80 : 0x00, 0x08, 0x80);
                _colon_default_left = state;
            } else {
                _writeRamAtAddr(state ? 0x40 : 0x00, 0x08, 0x40);
                _colon_default_right = state;
            }
            break;
        default:
            return; // Invalid section
    }
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::setDecimal(uint8_t row, uint8_t col, bool state) {
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
            current = _buffer[address >> 1];
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

    _writeRamAtAddr(current, address, mask);
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::setCursor(uint8_t row, uint8_t col) {
    if (row == 0 && col <= 2) {
        _colon_top = false;
    }

    if (row == 1) {
        if (col <= 2) {
            _colon_default_left = false;
        }
        if (col <= 4) {
            _colon_default_right = false;
        }
    }

    SegDriver_PCF8576::setCursor(row, col);
}

size_t SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::_writeRow0(uint8_t ch) {
    if (_cursorCol > 3) {
        return 0;
    }

    if (ch == '.') {
        setDecimal(_cursorRow, _cursorCol - 1, true);
        return 1;
    }

    if (ch == ':' && _cursorCol == 2 && !_colon_top) {
        setClockColon(_cursorRow, _cursorCol, true);
        return 1;
    }

    if (_cursorCol == 2 && ch != ':' && !_colon_top) {
        setClockColon(_cursorRow, _cursorCol, false);
    }

    uint8_t segment_data = _mapSegmentsTop(_get_char_value(ch));
    uint8_t addr = ADDR_SMALL_SEGS + (_cursorCol * 2);
    uint8_t dp_mask = 0x08;

    if (_cursorCol == 2 && ch != ':' && !_colon_top && (_buffer[addr >> 1] & dp_mask)) {
        uint8_t cleared = _buffer[addr >> 1] & ~dp_mask;
        _writeRamAtAddr(cleared, addr, addr == 0x06 ? 0xF7 : 0xFF);
    }

    if (_cursorCol == 3 && _colon_top) {
        uint8_t current = _buffer[addr >> 1] & dp_mask;
        segment_data = current | (segment_data & ~dp_mask);
    }

    _writeRamAtAddr(segment_data, addr, addr == 0x06 ? 0xF7 : 0xFF);
    _cursorCol++;
    return 1;
}

size_t SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::_writeRow1(uint8_t ch) {
    if (_cursorCol > 5) {
        return 0;
    }

    if (ch == '.') {
        setDecimal(_cursorRow, _cursorCol - 1, true);
        return 1;
    }

    if (ch == ':') {
        if (_cursorCol == 2 && !_colon_default_left) {
            setClockColon(_cursorRow, _cursorCol, true);
            return 1;
        }
        if (_cursorCol == 4 && !_colon_default_right) {
            setClockColon(_cursorRow, _cursorCol, true);
            return 1;
        }
    }

    if (_cursorCol == 2 && ch != ':' && !_colon_default_left) {
        setClockColon(_cursorRow, _cursorCol, false);
    }
    if (_cursorCol == 4 && ch != ':' && !_colon_default_right) {
        setClockColon(_cursorRow, _cursorCol, false);
    }

    uint8_t dp_mask = 0x10;
    if (_cursorCol == 4 && ch != ':' && !_colon_default_right &&
        (_buffer_default[_cursorCol] & dp_mask)) {
        _buffer_default[_cursorCol] &= ~dp_mask;
        _writeRamAtAddr(_buffer_default[_cursorCol],
                       ADDR_BIG_SEGS + ((6 - _cursorCol - 1) * 2));
    }

    uint8_t segment_data = _mapSegments(_get_char_value(ch));

    if (_cursorCol == 5 && _colon_default_right) {
        _buffer_default[_cursorCol] &= dp_mask;
        _buffer_default[_cursorCol] |= (segment_data & ~dp_mask);
    } else {
        _buffer_default[_cursorCol] = segment_data;
    }

    uint8_t col = 5 - _cursorCol;
    uint8_t addr = ADDR_BIG_SEGS + (col * 2);
    if (addr >= 0x11) addr += 2;

    _writeRamAtAddr(_buffer_default[_cursorCol], addr,
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

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::_writeRamAtAddr(uint8_t data, uint8_t address, uint8_t mask) {
    uint8_t byteIndex = address >> 1;

    if (byteIndex >= RAM_BYTE_COUNT) {
        return;  // Invalid address
    }

    if (mask != 0xFF) {
        uint8_t current;
        if ((address & 0x01) == 0) {
            current = _buffer[byteIndex];
        } else {
            uint8_t high = _buffer[byteIndex] & 0x0F;
            uint8_t low = 0;
            if (byteIndex + 1 < RAM_BYTE_COUNT) {
                low = (_buffer[byteIndex + 1] >> 4) & 0x0F;
            }
            current = low | (high << 4);
        }
        data = (current & ~mask) | (data & mask);
    }
    if ((address & 0x01) == 0) {
        _buffer[byteIndex] = data;
    } else {
        uint8_t low = data & 0x0F;
        uint8_t high = (data >> 4) & 0x0F;
        _buffer[byteIndex] = (_buffer[byteIndex] & 0xF0) | high;
        if (byteIndex + 1 < RAM_BYTE_COUNT) {
            _buffer[byteIndex + 1] = (_buffer[byteIndex + 1] & 0x0F) | (low << 4);
        }
    }
    _writeRam(data, address);
}



// For generic class allow access to low level functions
void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::writeRam(uint8_t data, uint8_t address) {
    _writeRam(data, address);
}

void SegLCD_PCF8576_4Seg6SegMaintSegBatUnits::writeRam(uint8_t *data, size_t length, uint8_t address) {
    _writeRam(data, length, address);
}
