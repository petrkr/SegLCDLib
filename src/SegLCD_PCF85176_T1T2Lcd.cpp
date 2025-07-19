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

void SegLCD_PCF85176_T1T2Lcd::setClockColon(bool status) {
    if (status)
        _buffer_sigclk |= 0x02;
    else
        _buffer_sigclk &= ~0x02;

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

void SegLCD_PCF85176_T1T2Lcd::setDecimal(uint8_t digit, bool state, LCDSections section) {
    if (digit < 1 || digit > 4) {
        return; // Invalid digit
    }

    uint8_t address = 0;
    uint8_t* _buffer = nullptr;
    switch (section) {
        case LCDSections::SECTION_DEFAULT:
        case LCDSections::SECTION_T1:
            address = ADDR_T1_SEGS + ((digit - 1) * 2);
            _buffer = _bufferT1;
            break;
        case LCDSections::SECTION_T2:
            address = ADDR_T2_SEGS + ((digit - 1) * 2);
            _buffer= _bufferT2;
            break;
        default:
            return; // Invalid section
    }

    if (state) {
        _buffer[(digit-1)] |= 0x01; // Set the decimal point bit
    } else {
        _buffer[(digit-1)] &= ~0x01; // Clear the decimal point bit
    }

    _writeRam(_buffer[(digit-1)], address);
}

void SegLCD_PCF85176_T1T2Lcd::writeFloat(float input, uint8_t decimals, LCDSections section) {
    bool isNegative = input < 0.0f;
    float scale = powf(10, decimals);
    long scaled = lroundf(fabsf(input) * scale);

    int totalDigits = _countDigits(scaled);
    int digitCount = totalDigits + (isNegative ? 1 : 0);
    if (decimals > 0 && totalDigits <= decimals) {
        digitCount++;
    }

    int startPos = 4 - digitCount + 1;
    int digitPos = 0;

    for (int i = 0; i < totalDigits; ++i) {
        int digit = scaled % 10;
        int pos = startPos + digitCount - 1 - digitPos;
        writeChar(pos, digit + '0', section);

        if (i == decimals && decimals > 0) {
            setDecimal(pos, true, section);
        }

        scaled /= 10;
        digitPos++;
    }

    if (decimals > 0 && totalDigits <= decimals) {
        int pos = startPos + digitCount - 1 - digitPos;
        writeChar(pos, '0', section);
        setDecimal(pos, true, section);
        digitPos++;
    }

    if (isNegative) {
        int pos = startPos + digitCount - 1 - digitPos;
        writeChar(pos, '-', section);
        digitPos++;
    }
}

void SegLCD_PCF85176_T1T2Lcd::writeChar(uint8_t digit, char c, LCDSections section) {
    uint8_t ch = _mapSegments(_get_char_value(c));

    // TODO: Do some dot bit mask and make it more generic
    switch (section) {
        case LCDSections::SECTION_DEFAULT:
        case LCDSections::SECTION_T1:
            _bufferT1[digit - 1] = ch;
            _writeRam(_bufferT1[digit - 1], ADDR_T1_SEGS + ((digit - 1) * 2));
            break;
        case LCDSections::SECTION_T2:
            _bufferT2[digit - 1] = ch;
            _writeRam(_bufferT2[digit - 1], ADDR_T2_SEGS + ((digit - 1) * 2));
            break;    
        case LCDSections::SECTION_TOP:
        case LCDSections::SECTION_CLOCK:
            _buffer_clock[digit - 1] &= ~0b11111110;
            _buffer_clock[digit - 1] |= ch & 0b11111110;
             _writeRam(_buffer_clock[digit - 1], ADDR_CLOCK_T1T2_LABELS_SEGS + ((digit - 1) * 2));
            break;
        case LCDSections::SECTION_DAY:
             _writeRam(ch, ADDR_DAY_SEG + ((digit - 1) * 2));
            break;
    }
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

// For generic class allow access to low level functions
void SegLCD_PCF85176_T1T2Lcd::write(uint8_t data, uint8_t address) {
    _writeRam(data, address);
}