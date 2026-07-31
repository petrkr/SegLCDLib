#if !defined(SEGLCD_DISABLE_ALL_LCDS) || defined(SEGLCD_ENABLE_PCF85134_XYGAX_SEG_I2C)
#include <SegLCD_PCF85134_XygaxSegI2C.h>

SegLCD_PCF85134_XygaxSegI2C::SegLCD_PCF85134_XygaxSegI2C(SegTransportI2C& transport, uint8_t address, uint8_t subaddress) :  SegDriver_PCF85134(transport, address, subaddress) {
    _allocateBuffer(RAM_SIZE);
}

void SegLCD_PCF85134_XygaxSegI2C::init() {
    SegDriver_PCF85134::init();
    _setMode(MODE_STATUS_ENABLED, MODE_DRIVE_STATIC);
}

void SegLCD_PCF85134_XygaxSegI2C::_setDecimal(uint8_t row, uint8_t col, bool state) {

    if (row != 0) {
        return; // invalid digit
    }

    if (col > DECIMAL_MAX_COL) {
        return; // Invalid digit
    }

    uint8_t bufIdx = col;
    uint8_t addr = col * 8;


    if (state) {
        _ramBuffer[bufIdx] |= DECIMAL_POINT_BIT; // Set the decimal point bit
    } else {
        _ramBuffer[bufIdx] &= ~DECIMAL_POINT_BIT; // Clear the decimal point bit
    }

    _writeRam(_ramBuffer[bufIdx], addr);
}

size_t SegLCD_PCF85134_XygaxSegI2C::write(uint8_t ch) {
    // Decimal point - does NOT move cursor (RAM offset -1: previous byte)
    if (_dotWrite(ch, DECIMAL_MIN_COL, DECIMAL_MAX_COL, DECIMAL_COL_OFFSET)) {
        return 1;  // Never move cursor for dot
    }

    if (_cursorCol >= DIGITS) {
        return 0;  // Invalid digit
    }

    // Clear decimal on current column when writing regular character
    _setDecimal(0, _cursorCol, false);

    // Regular character
    uint8_t segment_data = _get_char_value(ch);

    uint8_t bufIdx = _cursorCol;
    uint8_t addr = _cursorCol * 8;

    _ramBuffer[bufIdx] = segment_data;
    _writeRam(_ramBuffer[bufIdx], addr);

    _cursorCol++;
    return 1;
}

#endif
