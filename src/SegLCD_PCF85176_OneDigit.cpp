#include <SegLCD_PCF85176_OneDigit.h>

SegLCD_PCF85176_OneDigit::SegLCD_PCF85176_OneDigit(TwoWire& i2c, uint8_t address, uint8_t subaddress) :  SegDriver_PCF85176(i2c, address, subaddress) {
    _allocateBuffer(RAM_SIZE);
}

void SegLCD_PCF85176_OneDigit::init() {
    init(false);
}

void SegLCD_PCF85176_OneDigit::init(bool reverse) {
    init(reverse, false);
}

void SegLCD_PCF85176_OneDigit::init(bool reverse, bool v1fix) {
    SegDriver_PCF85176::init();
    _setMode(MODE_STATUS_ENABLED, MODE_DRIVE_STATIC);
    if (reverse) _setFlag(FLAG_REVERSE);
    if (v1fix) _setFlag(FLAG_V1FIX);
}

void SegLCD_PCF85176_OneDigit::_setDecimal(uint8_t row, uint8_t col, bool state) {

    if (row != 0) {
        return; // invalid digit
    }

    if (col < DECIMAL_MIN_COL || col > DECIMAL_MAX_COL) {
        return; // Invalid digit
    }

    uint8_t bufIdx = col;
    uint8_t addr = col * 8;

    if (_isFlagSet(FLAG_REVERSE)) {
        bufIdx = DIGITS - (col + 1);
        addr = bufIdx * 8;
    }

    if (state) {
        _ramBuffer[bufIdx] |= DECIMAL_POINT_BIT; // Set the decimal point bit
    } else {
        _ramBuffer[bufIdx] &= ~DECIMAL_POINT_BIT; // Clear the decimal point bit
    }

    _writeRam(_ramBuffer[bufIdx], addr);
}

size_t SegLCD_PCF85176_OneDigit::write(uint8_t ch) {

    if (_cursorCol >= DIGITS) {
        return 0;  // Invalid digit
    }

    // Decimal point - does NOT move cursor (RAM offset -1: previous byte)
    if (_dotWrite(ch, DECIMAL_MIN_COL, DECIMAL_MAX_COL, DECIMAL_COL_OFFSET)) {
        return 1;  // Never move cursor for dot
    }

    // Clear decimal on current column when writing regular character
    _setDecimal(0, _cursorCol, false);

    // Regular character
    uint8_t segment_data = _get_char_value(ch);

    // Board version v1 have swapped two segments
    if (_isFlagSet(FLAG_V1FIX)) {
        bool bit_2 = (segment_data >> 2) & 1;
        bool bit_3 = (segment_data >> 3) & 1;

        if (bit_2 != bit_3) {
            segment_data ^= (1 << 2) | (1 << 3);
        }
    }

    uint8_t bufIdx = _cursorCol;
    uint8_t addr = _cursorCol * 8;

    if (_isFlagSet(FLAG_REVERSE)) {
        bufIdx = DIGITS - (_cursorCol + 1);
        addr = bufIdx * 8;
    }

    _ramBuffer[bufIdx] = segment_data;
    _writeRam(_ramBuffer[bufIdx], addr);

    _cursorCol++;
    return 1;
}
