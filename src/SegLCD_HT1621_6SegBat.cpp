#include <SegLCD_HT1621_6SegBat.h>


SegLCD_HT1621_6SegBat::SegLCD_HT1621_6SegBat(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read) : SegDriver_HT1621(chipselect, data, write, read) {
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

    _ramBuffer[BATTERY_LEVEL_SEG[0]] &= ~(BATTERY_MASK);
    _ramBuffer[BATTERY_LEVEL_SEG[1]] &= ~(BATTERY_MASK);
    _ramBuffer[BATTERY_LEVEL_SEG[2]] &= ~(BATTERY_MASK);

    if (level > 0)
        _ramBuffer[BATTERY_LEVEL_SEG[0]] |= BATTERY_MASK;
    if (level > 1)
        _ramBuffer[BATTERY_LEVEL_SEG[1]] |= BATTERY_MASK;
    if (level > 2)
        _ramBuffer[BATTERY_LEVEL_SEG[2]] |= BATTERY_MASK;

    _writeRam(_ramBuffer[BATTERY_LEVEL_SEG[0]], 6);
    _writeRam(_ramBuffer[BATTERY_LEVEL_SEG[1]], 8);
    _writeRam(_ramBuffer[BATTERY_LEVEL_SEG[2]], 10);
}

void SegLCD_HT1621_6SegBat::setDecimal(uint8_t row, uint8_t col, bool state) {

    if (row != 0) {
        return; // invalid digit
    }

    if (col < DECIMAL_MIN_COL || col > DECIMAL_MAX_COL) {
        return; // Invalid digit
    }

    if (state) {
        _ramBuffer[col] |= DECIMAL_POINT_BIT; // Set the decimal point bit
    } else {
        _ramBuffer[col] &= ~DECIMAL_POINT_BIT; // Clear the decimal point bit
    }

    _writeRam(_ramBuffer[col], ((DIGITS - col - 1) * 2));
}

size_t SegLCD_HT1621_6SegBat::write(uint8_t ch) {

    if (_cursorCol < 0 || _cursorCol >= DIGITS) {
        return 0;  // Invalid digit
    }

    // Decimal point - does NOT move cursor (RAM offset 0: same byte)
    if (ch == '.') {
        if (_cursorCol >= DECIMAL_MIN_COL && _cursorCol <= DECIMAL_MAX_COL) {
            setDecimal(_cursorRow, _cursorCol, true);
            _setFlag(FLAG_PENDING_DOT);
        }
        return 1;  // Never move cursor for dot
    }

    // Regular character
    uint8_t segment_data = _mapSegments(_get_char_value(ch));

    // Preserve decimal point if FLAG_PENDING_DOT set (RAM offset 0 pattern)
    if (_isFlagSet(FLAG_PENDING_DOT)) {
        segment_data |= _ramBuffer[_cursorCol] & DECIMAL_POINT_BIT;
        _clearFlag(FLAG_PENDING_DOT);
    }

    _ramBuffer[_cursorCol] = segment_data;
    _writeRam(_ramBuffer[_cursorCol], (DIGITS - (_cursorCol + 1)) * 2);

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
