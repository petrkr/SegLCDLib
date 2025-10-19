#include <Wire.h>
#include <SegLCD_PCF85176_4DR821B.h>

SegLCD_PCF85176_4DR821B::SegLCD_PCF85176_4DR821B(TwoWire& i2c, uint8_t address, uint8_t subaddress) :  SegDriver_PCF85176(i2c, address, subaddress) {}

void SegLCD_PCF85176_4DR821B::init() {
    SegDriver_PCF85176::init();
    _setMode(MODE_STATUS_ENABLED, MODE_DRIVE_STATIC);
}

void SegLCD_PCF85176_4DR821B::clear() {
    memset(_buffer, 0x00, sizeof(_buffer));
    SegDriver_PCF85176::clear();
}

void SegLCD_PCF85176_4DR821B::setClockColon(uint8_t row, uint8_t col, bool state) {
    if (col == 0) {
        if (state) {
            _setSymbol(MINUS_BIT, false);
        }
        _setSymbol(LEFT_COLON_BIT, state);
    }

    if (col == 1) {
        _setSymbol(MIDDLE_COLON_BIT, state);
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
        _buffer[ADDR_SEGS + col] |= DECIMAL_POINT_BIT; // Set the decimal point bit
    } else {
        _buffer[ADDR_SEGS + col] &= ~DECIMAL_POINT_BIT; // Clear the decimal point bit
    }

    _writeRam(_buffer[ADDR_SEGS + col], (ADDR_SEGS + col) * 8);
    
}

void SegLCD_PCF85176_4DR821B::setCursor(uint8_t row, uint8_t col) {
    if (row == 0 && col <= 2) {
        _colonDisplayed = false;
    }

    if (row == 0 && col == 0) {
        _col0OverlayActive = false;
    }

    SegDriver_PCF85176::setCursor(row, col);
}

size_t SegLCD_PCF85176_4DR821B::write(uint8_t ch) {
    if (_cursorCol < 0 || _cursorCol > DIGITS - 1) {
        return 0; //Invalid digit
    }

    if (ch == '.') {
        setDecimal(_cursorRow, _cursorCol - 1, true);
        return 1;
    }

    // Handle clock/middle colon
    if (ch != ':' && _cursorCol == 2 && !_colonDisplayed) {
        setClockColon(_cursorRow, _cursorCol - 1, false);
        _colonDisplayed = false;
    }

    if (ch == ':' && _cursorCol == 2 && !_colonDisplayed) {
        setClockColon(_cursorRow, _cursorCol - 1, true);
        _colonDisplayed = true;
        return true;
    }

    // Handle overlay symbols in column 0: '-', '+', ':'
    if (!_handleCol0Overlay(ch)) {
        return 1;
    }

    uint8_t segment_data = _get_char_value(ch);

    _buffer[ADDR_SEGS + _cursorCol] = segment_data;

    
    _writeRam(_buffer[ADDR_SEGS + _cursorCol], (ADDR_SEGS + _cursorCol) * 8);
    

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
            _setSymbol(MINUS_BIT, true);
            _setSymbol(LEFT_COLON_BIT, false);
            _col0OverlayActive = true;
            return false;

        case ':':
            _setSymbol(MINUS_BIT, false);
            _setSymbol(LEFT_COLON_BIT, true);
            _col0OverlayActive = true;
            return false;

        case '+':
            _setSymbol(MINUS_BIT, true);
            _setSymbol(LEFT_COLON_BIT, true);
            _col0OverlayActive = true;
            return false;

        default:
            break;
    }

    // non-overlay char in column 0
    if (_col0OverlayActive) {
        _col0OverlayActive = false;
    } else {
        _setSymbol(MINUS_BIT, false);
        _setSymbol(LEFT_COLON_BIT, false);
    }

    return true;
}

void SegLCD_PCF85176_4DR821B::_setSymbol(uint8_t symbol, bool state) {
    if (state)
        _buffer[ADDR_SYMBOLS] |= symbol;
    else
        _buffer[ADDR_SYMBOLS] &= ~symbol;

    _writeRam(_buffer[ADDR_SYMBOLS], ADDR_SYMBOLS);
}
