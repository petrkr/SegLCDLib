#include <SegLCD_VK0192_5DigSigBattProgress.h>


SegLCD_VK0192_5DigSigBattProgress::SegLCD_VK0192_5DigSigBattProgress(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read) : SegDriver_VK0192(chipselect, data, write, read) {}

void SegLCD_VK0192_5DigSigBattProgress::init() {
    SegDriver_VK0192::init();

    command(CMD_SYS_EN);
    command(CMD_LCD_ON);
    command(CMD_NORMAL);
}

void SegLCD_VK0192_5DigSigBattProgress::clear() {
    memset(_buffer, 0, sizeof(_buffer));
    _writeRam(_buffer, sizeof(_buffer), 0);
}

void SegLCD_VK0192_5DigSigBattProgress::setBatteryLevel(uint8_t level) {
    if (level > MAX_BATTERY_LEVEL)
        level = MAX_BATTERY_LEVEL;

    _buffer[ADDR_BATT] &= ~(BATTERY_MASK);

    if (level > 0)
        _buffer[ADDR_BATT] |= BATTERY_LEVEL_SEG[0];
    if (level > 1)
        _buffer[ADDR_BATT] |= BATTERY_LEVEL_SEG[1];
    if (level > 2)
        _buffer[ADDR_BATT] |= BATTERY_LEVEL_SEG[2];
    if (level > 3)
        _buffer[ADDR_BATT] |= BATTERY_LEVEL_SEG[3];

    _writeRam(_buffer[ADDR_BATT], ADDR_BATT * 2);
}

void SegLCD_VK0192_5DigSigBattProgress::setSignalLevel(uint8_t level) {
    if (level > MAX_SIGNAL_LEVEL)
        level = MAX_SIGNAL_LEVEL;

    _buffer[ADDR_SIGNAL_LOW] &= ~(SIGNAL_MASK_LOW);
    _buffer[ADDR_SIGNAL_HIGH] &= ~(SIGNAL_MASK_HIGH);

    if (level > 0)
        _buffer[ADDR_SIGNAL_LOW] |= SIGNAL_LEVEL_SEG[0];
    if (level > 1)
        _buffer[ADDR_SIGNAL_LOW] |= SIGNAL_LEVEL_SEG[1];
    if (level > 2)
        _buffer[ADDR_SIGNAL_HIGH] |= SIGNAL_LEVEL_SEG[2];

    _writeRam(_buffer[ADDR_SIGNAL_LOW], ADDR_SIGNAL_LOW * 2);
    _writeRam(_buffer[ADDR_SIGNAL_HIGH], ADDR_SIGNAL_HIGH * 2);
}

void SegLCD_VK0192_5DigSigBattProgress::setProgress(uint8_t value) {
    _buffer[ADDR_PROGRESS_P1] &= ~(PROGRESS_MASK);
    _buffer[ADDR_PROGRESS_P2] &= ~(PROGRESS_MASK);
    _buffer[ADDR_PROGRESS_P3] &= ~(PROGRESS_MASK);
    _buffer[ADDR_PROGRESS_P4] &= ~(PROGRESS_MASK);

    if (value > 0) {
        _buffer[ADDR_PROGRESS_P3] |= PROGRESS_LEVEL_SEG[0];
    }
    if (value >= 10) {
        _buffer[ADDR_PROGRESS_P1] |= PROGRESS_LEVEL_SEG[1];
    }
    if (value >= 20) {
        _buffer[ADDR_PROGRESS_P1] |= PROGRESS_LEVEL_SEG[2];
    }
    if (value >= 30) {
        _buffer[ADDR_PROGRESS_P1] |= PROGRESS_LEVEL_SEG[3];
    }
    if (value >= 40) {
        _buffer[ADDR_PROGRESS_P1] |= PROGRESS_LEVEL_SEG[4];
    }
    if (value >= 50) {
        _buffer[ADDR_PROGRESS_P2] |= PROGRESS_LEVEL_SEG[5];
    }
    if (value >= 60) {
        _buffer[ADDR_PROGRESS_P2] |= PROGRESS_LEVEL_SEG[6];
    }
    if (value >= 70) {
        _buffer[ADDR_PROGRESS_P2] |= PROGRESS_LEVEL_SEG[7];
    }
    if (value >= 80) {
        _buffer[ADDR_PROGRESS_P2] |= PROGRESS_LEVEL_SEG[8];
    }
    if (value >= 90) {
        _buffer[ADDR_PROGRESS_P3] |= PROGRESS_LEVEL_SEG[9];
    }
    if (value >= 100) {
        _buffer[ADDR_PROGRESS_P3] |= PROGRESS_LEVEL_SEG[10];
    }
    if (value >= 110) {
        _buffer[ADDR_PROGRESS_P3] |= PROGRESS_LEVEL_SEG[11];
    }
    if (value >= 120) {
        _buffer[ADDR_PROGRESS_P4] |= PROGRESS_LEVEL_SEG[12];
    }
    if (value >= 130) {
        _buffer[ADDR_PROGRESS_P4] |= PROGRESS_LEVEL_SEG[13];
    }
    if (value >= 140) {
        _buffer[ADDR_PROGRESS_P4] |= PROGRESS_LEVEL_SEG[14];
    }
    if (value == 150) {
        _buffer[ADDR_PROGRESS_P4] |= PROGRESS_LEVEL_SEG[15];
    }

    _writeRam(_buffer[ADDR_PROGRESS_P1], ADDR_PROGRESS_P1 * 2);
    _writeRam(_buffer[ADDR_PROGRESS_P2], ADDR_PROGRESS_P2 * 2);
    _writeRam(_buffer[ADDR_PROGRESS_P3], ADDR_PROGRESS_P3 * 2);
    _writeRam(_buffer[ADDR_PROGRESS_P4], ADDR_PROGRESS_P4 * 2);
}

void SegLCD_VK0192_5DigSigBattProgress::setCursor(uint8_t row, uint8_t col) {
    SegDriver_VK0192::setCursor(row, col);
}

void SegLCD_VK0192_5DigSigBattProgress::setDecimal(uint8_t row, uint8_t col, bool state) {
    if (row > 2) {
        return; // invalid digit
    }

    if (row == 0 && (col < 0 || col > 1)) {
        return; // Invalid digit
    }

    if (row == 1 && (col < 0 || col > 3)) {
        return; // Invalid digit
    }

    // decimal is on next digit address
    int8_t addr = _get7SegmentsAddress(row, col + 1);

    // Invalid address
    if (addr < 0) {
        return;
    }

    if (state) {
        _buffer[addr] |= DECIMAL_POINT_BIT; // Set the decimal point bit
    } else {
        _buffer[addr] &= ~DECIMAL_POINT_BIT; // Clear the decimal point bit
    }

    // Write to RAM
    _writeRam(_buffer[addr], addr * 2);
}

size_t SegLCD_VK0192_5DigSigBattProgress::write(uint8_t ch) {
    writeDigit7seg(_cursorRow, _cursorCol, ch);
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

    _buffer[addr] = (mapped & 0x0F) << 4;   // lower 4 bits
    _buffer[addr + 1] = (mapped >> 4) << 4; // upper 4 bits

    // Write to RAM
    _writeRam(_buffer[addr], addr * 2);
    _writeRam(_buffer[addr+1], addr * 2 + 2);
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

// ABCD_EFGP to VK0192 specific mapping based on the table
uint8_t SegLCD_VK0192_5DigSigBattProgress::_mapSegments(uint8_t val) {
    uint8_t out = 0;
    out |= (val & 0b11110000);           // ABCD: bits 7-4
    out |= (val & 0b00001000) >> 2;      // E: bit 3 → 1 
    out |= (val & 0b00000100) << 1;      // F: bit 2 → 3 
    out |= (val & 0b00000010) << 1;      // G: bit 1 → 2
    out |= (val & 0b00000001);           // P: bit 0
    return out;
}

// TODO: Remove after display implementation
void SegLCD_VK0192_5DigSigBattProgress::writeRam(uint8_t data, uint8_t address) {
    _writeRam(data, address);
}

void SegLCD_VK0192_5DigSigBattProgress::writeRam(uint8_t *data, size_t length, uint8_t address) {
    _writeRam(data, length, address);
}