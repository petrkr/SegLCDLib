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

void SegLCD_VK0192_5DigSigBattProgress::setCursor(uint8_t row, uint8_t col) {
    SegDriver_VK0192::setCursor(row, col);
}

size_t SegLCD_VK0192_5DigSigBattProgress::write(uint8_t ch) {
    // Basic implementation - write to main 5 digits for now
    if (_cursorCol >= 0 && _cursorCol < 5) {
        writeDigit7seg(_cursorCol + 5, ch);  // Main digits start at index 5 (after 3+2)
        _cursorCol++;
        return 1;
    }
    return 0;
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