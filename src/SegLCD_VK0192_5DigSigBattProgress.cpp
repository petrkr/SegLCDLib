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

void SegLCD_VK0192_5DigSigBattProgress::writeDigit7seg(uint8_t digitIndex, char c) {  
    uint8_t mapped = _mapSegments(_get_char_value(c));
    uint8_t addr;

    switch (digitIndex) {
        case 0:
            // Shift bits 0x0E → 0x07, keep upper 4 bits unchanged
            mapped = (mapped & 0xF0) | ((mapped & 0x0E) >> 1);
            // address same as for 1 and 2, so do not break here
        case 1:
        case 2:
            addr = 8 + (digitIndex * 2);
            break;
        case 3:
        case 4:
            addr = 20 + ((digitIndex-3) * 2);
            // Shift bits 0x0E → 0x07, keep upper 4 bits unchanged
            mapped = (mapped & 0xF0) | ((mapped & 0x0E) >> 1);
            break;
        case 5:
            addr = 18;
            // Shift bits 0x0E → 0x07, keep upper 4 bits unchanged
            mapped = (mapped & 0xF0) | ((mapped & 0x0E) >> 1);
            break;
        case 6:
        case 7:
        case 8:
        case 9:
            addr = (digitIndex-6) * 2;
            break;
        default:
            return;
    }

    _buffer[addr] = (mapped & 0x0F) << 4;   // lower 4 bits
    _buffer[addr + 1] = (mapped >> 4) << 4; // upper 4 bits

    // Write to RAM
    _writeRam(_buffer[addr], addr * 2);     
    _writeRam(_buffer[addr+1], addr * 2 + 2);        
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