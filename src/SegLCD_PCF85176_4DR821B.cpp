#include <Wire.h>
#include <SegLCD_PCF85176_4DR821B.h>

SegLCD_PCF85176_4DR821B::SegLCD_PCF85176_4DR821B(TwoWire& i2c, uint8_t address, uint8_t subaddress) :  SegDriver_PCF85176(i2c, address, subaddress) {
    _allocateBuffer(RAM_SIZE);
}

void SegLCD_PCF85176_4DR821B::init() {
    SegDriver_PCF85176::init();
    _setMode(MODE_STATUS_ENABLED, MODE_DRIVE_STATIC);
}

void SegLCD_PCF85176_4DR821B::setSymbol(uint8_t symbol, bool state) {
    if (state)
        _ramBuffer[ADDR_SYMBOLS] |= symbol;
    else
        _ramBuffer[ADDR_SYMBOLS] &= ~symbol;

    _writeRam(_ramBuffer[ADDR_SYMBOLS], ADDR_SYMBOLS);
}

void SegLCD_PCF85176_4DR821B::setColon(uint8_t row, uint8_t col, bool state) {
    if (col == 0) {
        if (state) {
            setSymbol(MINUS_BIT, false);
        }
        setSymbol(LEFT_COLON_BIT, state);
    }

    if (col == 1) {
        setSymbol(MIDDLE_COLON_BIT, state);
    }
}

void SegLCD_PCF85176_4DR821B::setDecimal(uint8_t row, uint8_t col, bool state) {

    if (row != 0) {
        return; // invalid digit
    }

    if (col < DECIMAL_MIN_COL || col > DECIMAL_MAX_COL) {
        return; // Invalid digit
    }

    if (state) {
        _ramBuffer[ADDR_SEGS + col] |= DECIMAL_POINT_BIT; // Set the decimal point bit
    } else {
        _ramBuffer[ADDR_SEGS + col] &= ~DECIMAL_POINT_BIT; // Clear the decimal point bit
    }

    _writeRam(_ramBuffer[ADDR_SEGS + col], (ADDR_SEGS + col) * 8);

}

void SegLCD_PCF85176_4DR821B::setCursor(uint8_t row, uint8_t col) {
    if (row == 0 && col <= 2) {
        _clearFlag(FLAG_COLON_DISPLAYED);
    }

    if (row == 0 && col == 0) {
        _clearFlag(FLAG_COL0_OVERLAY);
    }

    SegDriver_PCF85176::setCursor(row, col);
}

size_t SegLCD_PCF85176_4DR821B::write(uint8_t ch) {

    if (_cursorCol < 0 || _cursorCol > DIGITS - 1) {
        return 0; //Invalid digit
    }

    // Handle decimal point - only set, don't clear previous
    if (_dotWrite(ch, DECIMAL_MIN_COL, DECIMAL_MAX_COL, -1)) {
        return 1;
    }

    // Handle clock/middle colon
    if (ch != ':' && _cursorCol == 2 && !_isFlagSet(FLAG_COLON_DISPLAYED)) {
        setColon(_cursorRow, _cursorCol - 1, false);
        _clearFlag(FLAG_COLON_DISPLAYED);
    }

    if (ch == ':' && _cursorCol == 2 && !_isFlagSet(FLAG_COLON_DISPLAYED)) {
        setColon(_cursorRow, _cursorCol - 1, true);
        _setFlag(FLAG_COLON_DISPLAYED);
        return true;
    }

    // Handle overlay symbols in column 0: '-', '+', ':'
    if (!_handleCol0Overlay(ch)) {
        return 1;
    }

    // Clear decimal on current column when writing regular character
    if (_cursorCol >= DECIMAL_MIN_COL && _cursorCol <= DECIMAL_MAX_COL) {
        setDecimal(_cursorRow, _cursorCol, false);
    }

    uint8_t segment_data = _get_char_value(ch);

    // Don't preserve decimal bit when writing regular character (we just cleared it)
    _ramBuffer[ADDR_SEGS + _cursorCol] = segment_data;

    _writeRam(_ramBuffer[ADDR_SEGS + _cursorCol], (ADDR_SEGS + _cursorCol) * 8);


    _cursorCol++;
    return 1;
}

bool SegLCD_PCF85176_4DR821B::_handleCol0Overlay(uint8_t ch) {
    // overlay applies only at column 0
    if (_cursorCol != 0)
        return true;

    switch (ch)
    {
        case '-':
            setSymbol(MINUS_BIT, true);
            setSymbol(LEFT_COLON_BIT, false);
            _setFlag(FLAG_COL0_OVERLAY);
            return false;

        case ':':
            setSymbol(MINUS_BIT, false);
            setSymbol(LEFT_COLON_BIT, true);
            _setFlag(FLAG_COL0_OVERLAY);
            return false;

        case '+':
            setSymbol(MINUS_BIT, true);
            setSymbol(LEFT_COLON_BIT, true);
            _setFlag(FLAG_COL0_OVERLAY);
            return false;

        default:
            break;
    }

    // non-overlay char in column 0
    if (_isFlagSet(FLAG_COL0_OVERLAY)) {
        _clearFlag(FLAG_COL0_OVERLAY);
    } else {
        setSymbol(MINUS_BIT, false);
        setSymbol(LEFT_COLON_BIT, false);
    }

    return true;
}
