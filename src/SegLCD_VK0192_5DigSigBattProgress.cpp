#include <SegLCD_VK0192_5DigSigBattProgress.h>


SegLCD_VK0192_5DigSigBattProgress::SegLCD_VK0192_5DigSigBattProgress(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read) : SegDriver_VK0192(chipselect, data, write, read) {
    _allocateBuffer(RAM_SIZE);
}

void SegLCD_VK0192_5DigSigBattProgress::init() {
    SegDriver_VK0192::init();

    command(CMD_SYS_EN);
    command(CMD_LCD_ON);
    command(CMD_NORMAL);
}

void SegLCD_VK0192_5DigSigBattProgress::setBatteryLevel(uint8_t level) {
    if (level > MAX_BATTERY_LEVEL)
        level = MAX_BATTERY_LEVEL;

    _ramBuffer[ADDR_BATT] &= ~(BATTERY_MASK);

    if (level > 0)
        _ramBuffer[ADDR_BATT] |= BATTERY_LEVEL_SEG[0];
    if (level > 1)
        _ramBuffer[ADDR_BATT] |= BATTERY_LEVEL_SEG[1];
    if (level > 2)
        _ramBuffer[ADDR_BATT] |= BATTERY_LEVEL_SEG[2];
    if (level > 3)
        _ramBuffer[ADDR_BATT] |= BATTERY_LEVEL_SEG[3];

    _writeRam(_ramBuffer[ADDR_BATT], ADDR_BATT * 2);
}

void SegLCD_VK0192_5DigSigBattProgress::setSignalLevel(uint8_t level) {
    if (level > MAX_SIGNAL_LEVEL)
        level = MAX_SIGNAL_LEVEL;

    _ramBuffer[ADDR_SIGNAL_LOW] &= ~(SIGNAL_MASK_LOW);
    _ramBuffer[ADDR_SIGNAL_HIGH] &= ~(SIGNAL_MASK_HIGH);

    if (level > 0)
        _ramBuffer[ADDR_SIGNAL_LOW] |= SIGNAL_LEVEL_SEG[0];
    if (level > 1)
        _ramBuffer[ADDR_SIGNAL_LOW] |= SIGNAL_LEVEL_SEG[1];
    if (level > 2)
        _ramBuffer[ADDR_SIGNAL_HIGH] |= SIGNAL_LEVEL_SEG[2];

    _writeRam(_ramBuffer[ADDR_SIGNAL_LOW], ADDR_SIGNAL_LOW * 2);
    _writeRam(_ramBuffer[ADDR_SIGNAL_HIGH], ADDR_SIGNAL_HIGH * 2);
}

void SegLCD_VK0192_5DigSigBattProgress::setProgress(uint8_t value) {
    _ramBuffer[ADDR_PROGRESS_P1] &= ~(PROGRESS_MASK);
    _ramBuffer[ADDR_PROGRESS_P2] &= ~(PROGRESS_MASK);
    _ramBuffer[ADDR_PROGRESS_P3] &= ~(PROGRESS_MASK);
    _ramBuffer[ADDR_PROGRESS_P4] &= ~(PROGRESS_MASK);

    if (value > 0) {
        _ramBuffer[ADDR_PROGRESS_P3] |= PROGRESS_LEVEL_SEG[0];
    }
    if (value >= 10) {
        _ramBuffer[ADDR_PROGRESS_P1] |= PROGRESS_LEVEL_SEG[1];
    }
    if (value >= 20) {
        _ramBuffer[ADDR_PROGRESS_P1] |= PROGRESS_LEVEL_SEG[2];
    }
    if (value >= 30) {
        _ramBuffer[ADDR_PROGRESS_P1] |= PROGRESS_LEVEL_SEG[3];
    }
    if (value >= 40) {
        _ramBuffer[ADDR_PROGRESS_P1] |= PROGRESS_LEVEL_SEG[4];
    }
    if (value >= 50) {
        _ramBuffer[ADDR_PROGRESS_P2] |= PROGRESS_LEVEL_SEG[5];
    }
    if (value >= 60) {
        _ramBuffer[ADDR_PROGRESS_P2] |= PROGRESS_LEVEL_SEG[6];
    }
    if (value >= 70) {
        _ramBuffer[ADDR_PROGRESS_P2] |= PROGRESS_LEVEL_SEG[7];
    }
    if (value >= 80) {
        _ramBuffer[ADDR_PROGRESS_P2] |= PROGRESS_LEVEL_SEG[8];
    }
    if (value >= 90) {
        _ramBuffer[ADDR_PROGRESS_P3] |= PROGRESS_LEVEL_SEG[9];
    }
    if (value >= 100) {
        _ramBuffer[ADDR_PROGRESS_P3] |= PROGRESS_LEVEL_SEG[10];
    }
    if (value >= 110) {
        _ramBuffer[ADDR_PROGRESS_P3] |= PROGRESS_LEVEL_SEG[11];
    }
    if (value >= 120) {
        _ramBuffer[ADDR_PROGRESS_P4] |= PROGRESS_LEVEL_SEG[12];
    }
    if (value >= 130) {
        _ramBuffer[ADDR_PROGRESS_P4] |= PROGRESS_LEVEL_SEG[13];
    }
    if (value >= 140) {
        _ramBuffer[ADDR_PROGRESS_P4] |= PROGRESS_LEVEL_SEG[14];
    }
    if (value >= 150) {
        _ramBuffer[ADDR_PROGRESS_P4] |= PROGRESS_LEVEL_SEG[15];
    }

    _writeRam(_ramBuffer[ADDR_PROGRESS_P1], ADDR_PROGRESS_P1 * 2);
    _writeRam(_ramBuffer[ADDR_PROGRESS_P2], ADDR_PROGRESS_P2 * 2);
    _writeRam(_ramBuffer[ADDR_PROGRESS_P3], ADDR_PROGRESS_P3 * 2);
    _writeRam(_ramBuffer[ADDR_PROGRESS_P4], ADDR_PROGRESS_P4 * 2);
}

void SegLCD_VK0192_5DigSigBattProgress::setLabels(uint16_t labels) {
    _updateLabels(labels, true);
}

void SegLCD_VK0192_5DigSigBattProgress::clearLabels(uint16_t labels) {
    _updateLabels(labels, false);
}

void SegLCD_VK0192_5DigSigBattProgress::setCursor(uint8_t row, uint8_t col) {
    SegDriver_VK0192::setCursor(row, col);
}

void SegLCD_VK0192_5DigSigBattProgress::setDecimal(uint8_t row, uint8_t col, bool state) {
    if (row > 2) {
        return; // invalid digit
    }

    if (row == 0 && (col < DECIMAL_TOP_MIN_COL || col > DECIMAL_TOP_MAX_COL)) {
        return; // Invalid digit
    }

    if (row == 1 && (col < DECIMAL_BOTTOM_MIN_COL || col > DECIMAL_BOTTOM_MAX_COL)) {
        return; // Invalid digit
    }

    if (row == 2) {
        if (col < DECIMAL_16SEG_MIN_COL || col > DECIMAL_16SEG_MAX_COL) {
            return; // Invalid digit
        }

        uint8_t addr = 0;
        switch (col) {
            case 0: addr = DECIMAL_16SEG_ADDR_COL0; break;
            case 1: addr = DECIMAL_16SEG_ADDR_COL1; break;
            case 2: addr = DECIMAL_16SEG_ADDR_COL2; break;
            case 3: addr = DECIMAL_16SEG_ADDR_COL3; break;
            default: return;
        }

        if (state) {
            _ramBuffer[addr] |= DECIMAL_16SEG_BIT;
        } else {
            _ramBuffer[addr] &= ~DECIMAL_16SEG_BIT;
        }

        _writeRam(_ramBuffer[addr], addr * 2);
        return;
    }

    int8_t addr = _get7SegmentsAddress(row, col + DECIMAL_RAM_OFFSET);

    // Invalid address
    if (addr < 0) {
        return;
    }

    if (state) {
        _ramBuffer[addr] |= DECIMAL_POINT_BIT;
    } else {
        _ramBuffer[addr] &= ~DECIMAL_POINT_BIT;
    }

    _writeRam(_ramBuffer[addr], addr * 2);
}

size_t SegLCD_VK0192_5DigSigBattProgress::write(uint8_t ch) {
    if (_cursorRow > 2) {
        return 0;
    }

    // Handle decimal point - only set, don't clear previous
    if (_cursorRow == 0 && _dotWrite(ch, DECIMAL_TOP_MIN_COL, DECIMAL_TOP_MAX_COL, -1)) {
        return 1;
    }
    if (_cursorRow == 1 && _dotWrite(ch, DECIMAL_BOTTOM_MIN_COL, DECIMAL_BOTTOM_MAX_COL, -1)) {
        return 1;
    }
    if (_cursorRow == 2 && _dotWrite(ch, DECIMAL_16SEG_MIN_COL, DECIMAL_16SEG_MAX_COL, -1)) {
        return 1;
    }

    if (_cursorCol > MAX_COL) {
        if (_cursorRow < 2) {
            _cursorRow++;
            _cursorCol = 0;
        } else {
            return 0;
        }
    }

    // Clear decimal on current column when writing regular character
    if (_cursorRow == 0 && _cursorCol >= DECIMAL_TOP_MIN_COL && _cursorCol <= DECIMAL_TOP_MAX_COL) {
        setDecimal(_cursorRow, _cursorCol, false);
    } else if (_cursorRow == 1 && _cursorCol >= DECIMAL_BOTTOM_MIN_COL && _cursorCol <= DECIMAL_BOTTOM_MAX_COL) {
        setDecimal(_cursorRow, _cursorCol, false);
    } else if (_cursorRow == 2 && _cursorCol >= DECIMAL_16SEG_MIN_COL && _cursorCol <= DECIMAL_16SEG_MAX_COL) {
        setDecimal(_cursorRow, _cursorCol, false);
    }

    // Regular character
    if (_cursorRow == 0 || _cursorRow == 1) {
        writeDigit7seg(_cursorRow, _cursorCol, ch);
    } else if (_cursorRow == 2) {
        writeDigit16seg(_cursorRow, _cursorCol, ch);
    }
    _cursorCol++;

    return 1;
}

// TODO: rename to private function and use in write
void SegLCD_VK0192_5DigSigBattProgress::writeDigit7seg(uint8_t row, uint8_t col, char c) {
    uint8_t mapped = _mapSegments(_get_char_value(c));
    int8_t addr = _get7SegmentsAddress(row, col);

    // Invalid address
    if (addr < 0) {
        return;
    }

    if (row == 0 && (col == 0 || col == 3 || col == 4)) {
        // Shift bits 0x0E → 0x07, keep upper 4 bits unchanged
        mapped = (mapped & 0xF0) | ((mapped & 0x0E) >> 1);
    }

    if (row == 1 && col == 0) {
        // Shift bits 0x0E → 0x07, keep upper 4 bits unchanged
        mapped = (mapped & 0xF0) | ((mapped & 0x0E) >> 1);
    }

    // Clear segment bits, preserve low nibble and only the decimal bit that belongs here
    uint8_t preserveMask = 0x0F;
    if (DECIMAL_RAM_OFFSET != 0) {
        int8_t prevCol = static_cast<int8_t>(col) - DECIMAL_RAM_OFFSET;
        if ((row == 0 && prevCol >= DECIMAL_TOP_MIN_COL && prevCol <= DECIMAL_TOP_MAX_COL) ||
            (row == 1 && prevCol >= DECIMAL_BOTTOM_MIN_COL && prevCol <= DECIMAL_BOTTOM_MAX_COL)) {
            preserveMask |= DECIMAL_POINT_BIT;
        }
    }
    if (addr == DECIMAL_16SEG_ADDR_COL0 || addr == DECIMAL_16SEG_ADDR_COL1 ||
        addr == DECIMAL_16SEG_ADDR_COL2 || addr == DECIMAL_16SEG_ADDR_COL3) {
        preserveMask |= DECIMAL_16SEG_BIT;
    }

    if (addr == ADDR_SIGNAL_HIGH) {
        preserveMask |= SIGNAL_MASK_HIGH;
    }

    _ramBuffer[addr] &= preserveMask;
    _ramBuffer[addr + 1] &= ~0xF0;

    // Set only high bits
    _ramBuffer[addr] |= (mapped & 0x0F) << 4;   // lower 4 bits
    _ramBuffer[addr + 1] |= (mapped >> 4) << 4; // upper 4 bits

    // Write to RAM
    _writeRam(_ramBuffer[addr], addr * 2);
    _writeRam(_ramBuffer[addr+1], addr * 2 + 2);
}

void SegLCD_VK0192_5DigSigBattProgress::writeDigit16seg(uint8_t row, uint8_t col, char c) {
    uint16_t mapped = _map16Segments(_get_16char_value(c));
    int8_t addr = _get16SegmentsAddress(row, col);

    // Invalid address
    if (addr < 0) {
        return;
    }

    _ramBuffer[addr]   &= ~0x0F;
    _ramBuffer[addr+1] &= ~0x0F;
    _ramBuffer[addr+2] &= ~0x0F;
    _ramBuffer[addr+3] &= ~0x0F;

    _ramBuffer[addr]   |= (mapped >> 12) & 0x0F;
    _ramBuffer[addr+1] |= (mapped >> 8) & 0x0F;
    _ramBuffer[addr+2] |= (mapped >> 4) & 0x0F;
    _ramBuffer[addr+3] |= (mapped) & 0x0F;

    // Write to RAM
    _writeRam(_ramBuffer[addr], addr * 2);
    _writeRam(_ramBuffer[addr+1], addr * 2 + 2);
    _writeRam(_ramBuffer[addr+2], addr * 2 + 4);
    _writeRam(_ramBuffer[addr+3], addr * 2 + 6);
}

int8_t SegLCD_VK0192_5DigSigBattProgress::_get7SegmentsAddress(uint8_t row, uint8_t col) {
    uint8_t addr;
    uint8_t digitIndex = col;

    if (row < 0 || row > 1) {
        return -1; // Invalid row
    }

    if (col > 5) {
        return -1; // Invalid col
    }

    if (row == 1) {
        digitIndex += 5;
    }

    switch (digitIndex) {
        case 0:
        case 1:
        case 2:
            addr = 8 + (digitIndex * 2);
            break;
        case 3:
        case 4:
            addr = 20 + ((digitIndex-3) * 2);
            break;
        case 5:
            addr = 18;
            break;
        case 6:
        case 7:
        case 8:
        case 9:
            addr = (digitIndex-6) * 2;
            break;
        default:
            return -1;
    }

    return addr;
}

int8_t SegLCD_VK0192_5DigSigBattProgress::_get16SegmentsAddress(uint8_t row, uint8_t col) {
    uint8_t addr;

    if (row != 2) {
        return -1; // Invalid row
    }

    if (col > 5) {
        return -1; // Invalid col
    }

    switch (col) {
        case 0:
        case 1:
        case 2:
        case 3:
            addr =  col * 4;
            break;
        case 4:
            addr = 0x14;
            break;
        default:
            return -1;
    }

    return addr;
}

uint8_t SegLCD_VK0192_5DigSigBattProgress::_mapSegments(uint8_t val) {
    uint8_t out = 0;
    out |= (val & 0b11110000);           // ABCD: bits 7-4
    out |= (val & 0b00001000) >> 2;      // E: bit 3 → 1
    out |= (val & 0b00000100) << 1;      // F: bit 2 → 3
    out |= (val & 0b00000010) << 1;      // G: bit 1 → 2
    out |= (val & 0b00000001);           // P: bit 0
    return out;
}

uint16_t SegLCD_VK0192_5DigSigBattProgress::_map16Segments(uint16_t val) {
    uint16_t out = 0;
    out |= (val & 0b1000000000000000);       // A1
    out |= (val & 0b0100000000000000) >> 3;  // A2
    out |= (val & 0b0010000000000000) >> 10; // B
    out |= (val & 0b0001000000000000) >> 12; // C
    out |= (val & 0b0000100000000000) >> 7;  // D2
    out |= (val & 0b0000010000000000) >> 2;  // D1
    out |= (val & 0b0000001000000000) << 3;  // E
    out |= (val & 0b0000000110000000) << 6;  // F, G1
    out |= (val & 0b0000000001000000) >> 4;  // G2
    out |= (val & 0b0000000000100000) << 5;  // H
    out |= (val & 0b0000000000010000) << 2;  // I
    out |= (val & 0b0000000000001010) << 4;  // J, L
    out |= (val & 0b0000000000000100) << 7;  // K
    out |= (val & 0b0000000000000001) << 1;  // M
    return out;
}

void SegLCD_VK0192_5DigSigBattProgress::_updateLabels(uint16_t labels, bool set) {
    if (labels & LABEL_P) {
        if (set) {
            _ramBuffer[ADDR_LABELS_3] |= 0x20;
        } else {
            _ramBuffer[ADDR_LABELS_3] &= ~0x20;
        }
    }

    if (labels & LABEL_T) {
        if (set) {
            _ramBuffer[ADDR_LABELS_3] |= 0x10;
        } else {
            _ramBuffer[ADDR_LABELS_3] &= ~0x10;
        }
    }

    if (labels & LABEL_PROC) {
        if (set) {
            _ramBuffer[ADDR_LABELS_1] |= 0x80;
        } else {
            _ramBuffer[ADDR_LABELS_1] &= ~0x80;
        }
    }

    if (labels & LABEL_C) {
        if (set) {
            _ramBuffer[ADDR_LABELS_2] |= 0x40;
        } else {
            _ramBuffer[ADDR_LABELS_2] &= ~0x40;
        }
    }

    if (labels & LABEL_KPA) {
        if (set) {
            _ramBuffer[ADDR_LABELS_1] |= 0x40;
        } else {
            _ramBuffer[ADDR_LABELS_1] &= ~0x40;
        }
    }

    if (labels & LABEL_MPA) {
        if (set) {
            _ramBuffer[ADDR_LABELS_1] |= 0x20;
        } else {
            _ramBuffer[ADDR_LABELS_1] &= ~0x20;
        }
    }

    if (labels & LABEL_M) {
        if (set) {
            _ramBuffer[ADDR_LABELS_1] |= 0x10;
        } else {
            _ramBuffer[ADDR_LABELS_1] &= ~0x10;
        }
    }

    if (labels & LABEL_A) {
        if (set) {
            _ramBuffer[ADDR_LABELS_2] |= 0x10;
        } else {
            _ramBuffer[ADDR_LABELS_2] &= ~0x10;
        }
    }

    if (labels & LABEL_V) {
        if (set) {
            _ramBuffer[ADDR_LABELS_2] |= 0x20;
        } else {
            _ramBuffer[ADDR_LABELS_2] &= ~0x20;
        }
    }

    _writeRam(_ramBuffer[ADDR_LABELS_1], ADDR_LABELS_1 * 2);
    _writeRam(_ramBuffer[ADDR_LABELS_2], ADDR_LABELS_2 * 2);
    _writeRam(_ramBuffer[ADDR_LABELS_3], ADDR_LABELS_3 * 2);
}
