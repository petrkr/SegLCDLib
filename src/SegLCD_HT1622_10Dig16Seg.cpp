#include <SegLCD_HT1622_10Dig16Seg.h>


SegLCD_HT1622_10Dig16Seg::SegLCD_HT1622_10Dig16Seg(SegTransport3Wire& transport, uint8_t chipselect)
    : SegDriver_HT1622(transport, chipselect) {
    _allocateBuffer(RAM_SIZE);
}

void SegLCD_HT1622_10Dig16Seg::init() {
    SegDriver_HT1622::init();

    command(CMD_SYS_EN);
    command(CMD_LCD_ON);
    command(CMD_NORMAL);
}

void SegLCD_HT1622_10Dig16Seg::_setDecimal(uint8_t row, uint8_t col, bool state) {

    if (row != 0) {
        return; // Only row 0 is valid for this display
    }

    if (col > DECIMAL_MAX_COL) {
        return; // Only digits 0-8 have decimal points
    }

    uint8_t address;
    uint8_t bitPosition;

    if (col <= 2) {
        address = 40;
        bitPosition = 2 - col; // Reverse: col 0→bit 2, col 1→bit 1, col 2→bit 0
    } else if (col <= 5) {
        address = 42;
        bitPosition = 5 - col; // Reverse: col 3→bit 2, col 4→bit 1, col 5→bit 0
    } else { // col 6-8
        address = 44;
        bitPosition = 8 - col; // Reverse: col 6→bit 2, col 7→bit 1, col 8→bit 0
    }

    // Read current value, modify bit, write back
    if (state) {
        _ramBuffer[address/2] |= (1 << bitPosition);
    } else {
        _ramBuffer[address/2] &= ~(1 << bitPosition);
    }

    _writeRam(_ramBuffer[address/2], address);
}

size_t SegLCD_HT1622_10Dig16Seg::write(uint8_t ch) {
    if (_cursorRow != 0 || _cursorCol >= DIGITS) {
        return 0;
    }

    // Decimal point - only set, don't clear previous
    if (_dotWrite(ch, DECIMAL_MIN_COL, DECIMAL_MAX_COL, DECIMAL_COL_OFFSET)) {
        return 1;
    }

    // Clear decimal on current column when writing regular character
    if (_cursorCol <= DECIMAL_MAX_COL) {
        _setDecimal(0, _cursorCol, false);
    }

    _writeDigit16seg(_cursorRow, _cursorCol, ch);
    _cursorCol++;

    return 1;
}

void SegLCD_HT1622_10Dig16Seg::_writeDigit16seg(uint8_t row, uint8_t col, char c) {

    uint16_t mapped = _map16Segments(_get_16char_value(c));
    int8_t addr = _get16SegmentsAddress(row, col);

    // Invalid address
    if (addr < 0) {
        return;
    }

    _ramBuffer[addr]   = (mapped >> 8) & 0xFF;
    _ramBuffer[addr+1] = (mapped) & 0xFF;

    // Write to RAM
    _writeRam(_ramBuffer[addr], addr * 2);
    _writeRam(_ramBuffer[addr+1], addr * 2 + 2);
}

int8_t SegLCD_HT1622_10Dig16Seg::_get16SegmentsAddress(uint8_t row, uint8_t col) {
    if (row != 0 || col >= DIGITS) {
        return -1; // Invalid row
    }

    return (DIGITS - col - 1) * 2;
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
