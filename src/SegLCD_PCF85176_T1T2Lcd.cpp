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
    if (level > 4)
        level = 4;

        _buffer_batt &= ~(0xf0);

    if (level > 0)
        _buffer_batt |= 0x10;
    if (level > 1)
        _buffer_batt |= 0x80;
    if (level > 2)
        _buffer_batt |= 0x40;
    if (level > 3)
        _buffer_batt |= 0x20;

    _writeRam(_buffer_batt, ADDR_BATT);
}

void SegLCD_PCF85176_T1T2Lcd::setSignalLevel(uint8_t level) {
    if (level > 5)
        level = 5;

        _buffer_sigclk &= ~(0xf8);

    if (level > 0)
        _buffer_sigclk |= 0x80;
    if (level > 1)
        _buffer_sigclk |= 0x40;
    if (level > 2)
        _buffer_sigclk |= 0x20;
    if (level > 3)
        _buffer_sigclk |= 0x10;
    if (level > 4)
        _buffer_sigclk |= 0x08;

    _writeRam(_buffer_sigclk, ADDR_SIGNAL_CLOCK);
}

void SegLCD_PCF85176_T1T2Lcd::setClockSymbol(bool status) {
    if (status)
        _buffer_sigclk |= 0x04;
    else
        _buffer_sigclk &= ~0x04;

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
      _buffer_clock[0] |= 0x01;

    if (t1t2 & SegLCD_PCF85176_T1T2Lcd::LABEL_T2)
      _buffer_clock[1] |= 0x01;

    _writeRam(_buffer_clock, 2, ADDR_CLOCK_T1T2_LABELS_SEGS);
}

void SegLCD_PCF85176_T1T2Lcd::clearT1T2Labels(uint8_t t1t2) {
    if (t1t2 & SegLCD_PCF85176_T1T2Lcd::LABEL_T1)
      _buffer_clock[0] &= ~0x01;

    if (t1t2 & SegLCD_PCF85176_T1T2Lcd::LABEL_T2)
      _buffer_clock[1] &= ~0x01;

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
    if (row < 1 || row > 2) {
        return; // Invalid digit
    }

    if (col < 0 || col > 3) {
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
        _buffer[col] |= 0x01; // Set the decimal point bit
    } else {
        _buffer[col] &= ~0x01; // Clear the decimal point bit
    }

    _writeRam(_buffer[col], address);
}

size_t SegLCD_PCF85176_T1T2Lcd::write(uint8_t ch) {
    uint8_t segment_data = _mapSegments(_get_char_value(ch));

    switch (_cursorRow) {
        case 0:
            if (ch == ':' && _cursorCol == 2) {
                setClockColon(_cursorRow, _cursorCol, true);
                return 1;
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
        case 1:
            if (ch == '.') {
                setDecimal(_cursorRow, _cursorCol - 1, true);
                return 1;
            }
            _bufferT1[_cursorCol] = segment_data;
            _writeRam(_bufferT1[_cursorCol], ADDR_T1_SEGS + (_cursorCol * 2));
            break;
        case 2:
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
