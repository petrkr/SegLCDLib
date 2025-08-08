#include <SegLCD_HT1622_10Dig16Seg.h>


SegLCD_HT1622_10Dig16Seg::SegLCD_HT1622_10Dig16Seg(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read) : SegDriver_HT1622(chipselect, data, write, read) {}

void SegLCD_HT1622_10Dig16Seg::init() {
    SegDriver_HT1622::init();

    command(CMD_SYS_EN);
    command(CMD_LCD_ON);
    command(CMD_NORMAL);
}

void SegLCD_HT1622_10Dig16Seg::clear() {
    memset(_buffer, 0, sizeof(_buffer));
    _writeRam(_buffer, sizeof(_buffer), 0);
}

size_t SegLCD_HT1622_10Dig16Seg::write(uint8_t ch) {
    writeDigit16seg(_cursorRow, _cursorCol, ch);
    _cursorCol++;

    return 1;
}

// For generic class allow access to low level functions
void SegLCD_HT1622_10Dig16Seg::writeRam(uint8_t data, uint8_t address) {
    _writeRam(data, address);
}

void SegLCD_HT1622_10Dig16Seg::writeRam(uint8_t *data, size_t length, uint8_t address) {
    _writeRam(data, length, address);
}

void SegLCD_HT1622_10Dig16Seg::writeDigit16seg(uint8_t row, uint8_t col, char c) {
    uint16_t mapped = _map16Segments(_get_16char_value(c));
    int8_t addr = _get16SegmentsAddress(row, col);

    // Invalid address
    if (addr < 0) {
        return;
    }

    _buffer[addr]   = (mapped >> 8) & 0xFF;
    _buffer[addr+1] = (mapped) & 0xFF;

    // Write to RAM
    _writeRam(_buffer[addr], addr * 2);
    _writeRam(_buffer[addr+1], addr * 2 + 2);
}

int8_t SegLCD_HT1622_10Dig16Seg::_get16SegmentsAddress(uint8_t row, uint8_t col) {
    if (row != 0 || col > 10) {
        return -1; // Invalid row
    }

    return (9-col) * 2;
}

// A1HFG1 KLED2 A2IBJ G2MCD1
uint16_t SegLCD_HT1622_10Dig16Seg::_map16Segments(uint16_t val) {
    uint16_t out = 0;
    out |= (val & 0b1000001000000000);       // A1, E
    out |= (val & 0b0100000000000000) >> 7;  // A2
    out |= (val & 0b0010000000000000) >> 8;  // B
    out |= (val & 0b0001100000000000) >> 11; // C, D2
    out |= (val & 0b0000010000000000) >> 2;  // D1
    out |= (val & 0b0000000110000000) << 5;  // F, G1
    out |= (val & 0b0000000001000000) >> 3;  // G2
    out |= (val & 0b0000000000100110) << 9;  // H, L, K
    out |= (val & 0b0000000000010001) << 2;  // I, M
    out |= (val & 0b0000000000001000) << 1;  // J
    return out;
}
