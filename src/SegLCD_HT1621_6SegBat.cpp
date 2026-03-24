#include <SegLCD_HT1621_6SegBat.h>


SegLCD_HT1621_6SegBat::SegLCD_HT1621_6SegBat(SegTransport3Wire& transport, uint8_t chipselect)
    : SegDriver_HT1621(transport, chipselect) {
    _allocateBuffer(RAM_SIZE);
}

void SegLCD_HT1621_6SegBat::init() {
    SegDriver_HT1621::init();
    _setMode(MODE_DRIVE_14, MODE_BIAS_13);

    command(CMD_SYS_EN);
    command(CMD_LCD_ON);
    command(CMD_NORMAL);
}


void SegLCD_HT1621_6SegBat::setBatteryLevel(uint8_t level) {

    if (level > MAX_BATTERY_LEVEL)
        level = MAX_BATTERY_LEVEL;

    uint8_t data0 = (level > 0) ? BATTERY_MASK : 0x00;
    uint8_t data1 = (level > 1) ? BATTERY_MASK : 0x00;
    uint8_t data2 = (level > 2) ? BATTERY_MASK : 0x00;

    // Use masked write to preserve digit segments (only modify battery bit 0x80)
    _writeRamMasked(data0, (DIGITS - BATTERY_LEVEL_SEG[0] - 1) * 2, BATTERY_MASK);
    _writeRamMasked(data1, (DIGITS - BATTERY_LEVEL_SEG[1] - 1) * 2, BATTERY_MASK);
    _writeRamMasked(data2, (DIGITS - BATTERY_LEVEL_SEG[2] - 1) * 2, BATTERY_MASK);
}

void SegLCD_HT1621_6SegBat::_setDecimal(uint8_t row, uint8_t col, bool state) {

    if (row != 0) {
        return; // invalid digit
    }

    if (col < DECIMAL_MIN_COL || col > DECIMAL_MAX_COL) {
        return; // Invalid digit
    }

    uint8_t data = state ? DECIMAL_POINT_BIT : 0x00;

    // Decimal point is stored in RAM of the NEXT digit (col+1)
    _writeRamMasked(data, (DIGITS - col - 2) * 2, DECIMAL_POINT_BIT);
}

size_t SegLCD_HT1621_6SegBat::write(uint8_t ch) {
    if (_cursorCol >= DIGITS) {
        return 0;
    }

    // Handle decimal point - only set, don't clear previous
    if (_dotWrite(ch, DECIMAL_MIN_COL, DECIMAL_MAX_COL, DECIMAL_COL_OFFSET)) {
        return 1;
    }

    // Clear decimal on current column when writing regular character
    if (_cursorCol >= DECIMAL_MIN_COL && _cursorCol <= DECIMAL_MAX_COL) {
        _setDecimal(0, _cursorCol, false);
    }

    // Regular character
    uint8_t segment_data = _mapSegments(_get_char_value(ch));
    uint8_t hw_addr = (DIGITS - (_cursorCol + 1)) * 2;

    // Bit 0x80 is always special (battery on col 0,1,2 or decimal of previous digit on col 3,4,5)
    _writeRamMasked(segment_data, hw_addr, 0x7F);

    _cursorCol++;
    return 1;
}

// ABCD_EFGH to HCBA_DEGF
uint8_t SegLCD_HT1621_6SegBat::_mapSegments(uint8_t val) {
    uint8_t out = 0;

    out |= (val & 0b01011000) >> 1;      // BDE: bits 6,4,3 → 5,3,2
    out |= (val & 0b10000000) >> 3;      // A: bit 7 → 4
    out |= (val & 0b00100000) << 1;      // C: bit 5 → 6
    out |= (val & 0b00000100) >> 2;      // F: bit 2 → 0
    out |= (val & 0b00000010);           // G: bit 1 → 1
    out |= (val & 0b00000001) << 7;      // H: bit 0 → 7
    return out;
}
