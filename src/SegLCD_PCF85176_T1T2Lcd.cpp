#include <Wire.h>
#include <SegLCD_PCF85176_T1T2Lcd.h>


SegLCD_PCF85176_T1T2Lcd::SegLCD_PCF85176_T1T2Lcd(TwoWire& i2c, uint8_t address, uint8_t subaddress) :  SegDriver_PCF85176(i2c, address, subaddress) {}

void SegLCD_PCF85176_T1T2Lcd::init() {
    SegDriver_PCF85176::init();
    _setMode(MODE_STATUS_ENABLED);
}

void SegLCD_PCF85176_T1T2Lcd::clear() {
    memset(_bufferT1, 0x00, sizeof(_bufferT1));
    memset(_bufferT2, 0x00, sizeof(_bufferT2));
    SegDriver_PCx85::clear();
}

void SegLCD_PCF85176_T1T2Lcd::setBatteryLevel(uint8_t level) {
    if (level > MAX_BATTERY_LEVEL)
        level = MAX_BATTERY_LEVEL;

        _buffer_batt &= ~(BATTERY_MASK);

    if (level > 0)
        _buffer_batt |= BATTERY_LEVEL_SEG[0];
    if (level > 1)
        _buffer_batt |= BATTERY_LEVEL_SEG[1];
    if (level > 2)
        _buffer_batt |= BATTERY_LEVEL_SEG[2];
    if (level > 3)
        _buffer_batt |= BATTERY_LEVEL_SEG[3];

    _writeRam(_buffer_batt, ADDR_BATT);
}

void SegLCD_PCF85176_T1T2Lcd::setSignalLevel(uint8_t level) {
    if (level > MAX_SIGNAL_LEVEL)
        level = MAX_SIGNAL_LEVEL;

        _buffer_sigclk &= ~(SIGNAL_MASK);

    if (level > 0)
        _buffer_sigclk |= SIGNAL_LEVEL_BITS[0];
    if (level > 1)
        _buffer_sigclk |= SIGNAL_LEVEL_BITS[1];
    if (level > 2)
        _buffer_sigclk |= SIGNAL_LEVEL_BITS[2];
    if (level > 3)
        _buffer_sigclk |= SIGNAL_LEVEL_BITS[3];
    if (level > 4)
        _buffer_sigclk |= SIGNAL_LEVEL_BITS[4];

    _writeRam(_buffer_sigclk, ADDR_SIGNAL_CLOCK);
}

void SegLCD_PCF85176_T1T2Lcd::setClockSymbol(bool status) {
    if (status)
        _buffer_sigclk |= SYMBOL_CLOCK_MASK;
    else
        _buffer_sigclk &= ~SYMBOL_CLOCK_MASK;

    _writeRam(_buffer_sigclk, ADDR_SIGNAL_CLOCK);
}

void SegLCD_PCF85176_T1T2Lcd::setLabels(uint8_t labels) {
    _buffer_labels |= labels;
    _writeRam(_buffer_labels, ADDR_LABELS);
}

void SegLCD_PCF85176_T1T2Lcd::clearLabels(uint8_t labels) {
    _buffer_labels &= ~labels;
    _writeRam(_buffer_labels, ADDR_LABELS);
}

void SegLCD_PCF85176_T1T2Lcd::setT1T2Labels(uint8_t t1t2) {
    if (t1t2 & SegLCD_PCF85176_T1T2Lcd::LABEL_T1)
      _buffer_clock[0] |= SYMBOL_T1T2_MASK;

    if (t1t2 & SegLCD_PCF85176_T1T2Lcd::LABEL_T2)
      _buffer_clock[1] |= SYMBOL_T1T2_MASK;

    _writeRam(_buffer_clock, 2, ADDR_CLOCK_T1T2_LABELS_SEGS);
}

void SegLCD_PCF85176_T1T2Lcd::clearT1T2Labels(uint8_t t1t2) {
    if (t1t2 & SegLCD_PCF85176_T1T2Lcd::LABEL_T1)
      _buffer_clock[0] &= ~SYMBOL_T1T2_MASK;

    if (t1t2 & SegLCD_PCF85176_T1T2Lcd::LABEL_T2)
      _buffer_clock[1] &= ~SYMBOL_T1T2_MASK;

    _writeRam(_buffer_clock, 2, ADDR_CLOCK_T1T2_LABELS_SEGS);
}

void SegLCD_PCF85176_T1T2Lcd::setClockColon(uint8_t row, uint8_t col, bool state) {
    if (state)
        _buffer_sigclk |= 0x02;
    else
        _buffer_sigclk &= ~0x02;

    _writeRam(_buffer_sigclk, ADDR_SIGNAL_CLOCK);
}

void SegLCD_PCF85176_T1T2Lcd::setDecimal(uint8_t row, uint8_t col, bool state) {
    if (row < DECIMAL_MIN_ROW || row > DECIMAL_MAX_ROW) {
        return; // Invalid digit
    }

    if (col < DECIMAL_MIN_COL || col > DECIMAL_MAX_COL) {
        return; // Invalid digit
    }

    uint8_t address = 0;
    uint8_t* _buffer = nullptr;
    switch (row) {
        case 1:
            address = ADDR_T1_SEGS + (col * 2);
            _buffer = _bufferT1;
            break;
        case 2:
            address = ADDR_T2_SEGS + (col * 2);
            _buffer = _bufferT2;
            break;
        default:
            return; // Invalid section
    }

    if (state) {
        _buffer[col] |= DECIMAL_POINT_BIT; // Set the decimal point bit
    } else {
        _buffer[col] &= ~DECIMAL_POINT_BIT; // Clear the decimal point bit
    }

    _writeRam(_buffer[col], address);
}

void SegLCD_PCF85176_T1T2Lcd::setCursor(uint8_t row, uint8_t col) {
    if (row == ROW_CLOCK && col < 2) {
        _colon_clock = false;
    }

    SegDriver_PCF85176::setCursor(row, col);
}

size_t SegLCD_PCF85176_T1T2Lcd::write(uint8_t ch) {
    uint8_t segment_data = _mapSegments(_get_char_value(ch));

    switch (_cursorRow) {
        case ROW_CLOCK:
            // Set colon if next char is colon and flag, we want it keep
            if (ch == ':' && _cursorCol == 2) {
                setClockColon(_cursorRow, _cursorCol, true);
                _colon_clock = true;
                return 1;
            }

            // Clear Clock colon if we do not want it and colon column is not colon
            if (ch != ':' && _cursorCol == 2 && !_colon_clock) {
                setClockColon(_cursorRow, _cursorCol, false);
            }

            if (_cursorCol >=0 && _cursorCol < 4) {
                _buffer_clock[_cursorCol] &= ~0b11111110;
                _buffer_clock[_cursorCol] |= segment_data & 0b11111110;
                _writeRam(_buffer_clock[_cursorCol], ADDR_CLOCK_T1T2_LABELS_SEGS + (_cursorCol * 2));
            }

            if (_cursorCol == 4) {
                _writeRam(segment_data, ADDR_DAY_SEG);
            }
            break;
        case ROW_T1:
            if (ch == '.') {
                setDecimal(_cursorRow, _cursorCol - 1, true);
                return 1;
            }
            _bufferT1[_cursorCol] = segment_data;
            _writeRam(_bufferT1[_cursorCol], ADDR_T1_SEGS + (_cursorCol * 2));
            break;
        case ROW_T2:
            if (ch == '.') {
                setDecimal(_cursorRow, _cursorCol - 1, true);
                return 1;
            }
            _bufferT2[_cursorCol] = segment_data;
            _writeRam(_bufferT2[_cursorCol], ADDR_T2_SEGS + (_cursorCol * 2));
            break;
    }
    _cursorCol++;
    return 1;
}

// ABCD_EFGH to FGED ABCH..
uint8_t SegLCD_PCF85176_T1T2Lcd::_mapSegments(uint8_t val) {
    uint8_t out = 0;
    out |= (val & 0b11100000) >> 4;   // ABC
    out |= (val & 0b00010000);        // D
    out |= (val & 0b00001000) << 2;   // E
    out |= (val & 0b00000110) << 5;   // FG
    out |= (val & 0b00000001);        // H
    return out;
}
