#include <Wire.h>
#include <SegLCD_PCF85176_6DigSigBattProgress.h>


SegLCD_PCF85176_6DigitSignalBatteryProgress::SegLCD_PCF85176_6DigitSignalBatteryProgress(TwoWire& i2c, uint8_t address, uint8_t subaddress) :  SegDriver_PCF85176(i2c, address, subaddress) {}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::begin() {
    SegDriver_PCF85176::begin();
    _setMode(MODE_STATUS_ENABLED);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setBatteryLevel(uint8_t level) {
    if (level > 4)
        level = 4;

    _buffer_sigbatt &= ~(0x0f);

    if (level > 0)
        _buffer_sigbatt |= 8;
    if (level > 1)
        _buffer_sigbatt |= 1;
    if (level > 2)
        _buffer_sigbatt |= 2;
    if (level > 3)
        _buffer_sigbatt |= 4;

    _write(_buffer_sigbatt, ADDR_SIGNAL_BATT);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setSignalLevel(uint8_t level) {
    if (level > 4)
        level = 4;

    _buffer_sigbatt &= ~(0xf0);

    if (level > 0)
        _buffer_sigbatt |= 0x80;
    if (level > 1)
        _buffer_sigbatt |= 0x40;
    if (level > 2)
        _buffer_sigbatt |= 0x20;
    if (level > 3)
        _buffer_sigbatt |= 0x10;

    _write(_buffer_sigbatt, ADDR_SIGNAL_BATT);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setProgress(uint8_t value) {
    uint8_t buffer[2] = { 0, 0 };

    if (value > 0) {
        buffer[0] |= 0x80;
    }
    if (value >= 10) {
        buffer[0] |= 0x40;
    }
    if (value >= 20) {
        buffer[0] |= 0x20;
    }
    if (value >= 30) {
        buffer[0] |= 0x10;
    }
    if (value >= 40) {
        buffer[0] |= 0x08;
    }
    if (value >= 50) {
        buffer[0] |= 0x04;
    }
    if (value >= 60) {
        buffer[0] |= 0x02;
    }
    if (value >= 70) {
        buffer[0] |= 0x01;
    }

    if (value >= 80) {
        buffer[1] |= 0x80;
    }
    if (value >= 90) {
        buffer[1] |= 0x40;
    }
    if (value >= 100) {
        buffer[1] |= 0x20;
    }
    if (value >= 110) {
        buffer[1] |= 0x10;
    }
    if (value >= 120) {
        buffer[1] |= 0x08;
    }
    if (value >= 130) {
        buffer[1] |= 0x04;
    }
    if (value >= 140) {
        buffer[1] |= 0x02;
    }
    if (value == 150) {
        buffer[1] |= 0x01;
    }

    _write(buffer, sizeof(buffer), ADDR_PROGRESS);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setWheel(uint16_t value) {
    uint8_t buffer[2] = { 0, 0 };
    
    buffer[0] = value & 0xFF;
    buffer[1] = (((value >> 8) & 0x0F) << 4);

    _write(buffer, sizeof(buffer), ADDR_WHEEL);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setLabels(LabelFlags labels) {
    _buffer_labels |= labels;
    
    _write(_buffer_labels, ADDR_PRES_LABELS);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::clearLabels(LabelFlags labels) {
    _buffer_labels &= ~labels;
    
    _write(_buffer_labels, ADDR_PRES_LABELS);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setClockColon(bool state, LCDSections section) {
    uint8_t address = 0;
    uint8_t digit = 0;
    uint8_t* _buffer = nullptr;
    switch (section) {
        case LCDSections::SECTION_DEFAULT:
            address = ADDR_BIG_SEGS; // Digit 6
            digit = 6;
            _buffer = _buffer_default;
            break;
        case LCDSections::SECTION_TOP:
        case LCDSections::SECTION_CLOCK:
            address = ADDR_SMALL_SEGS + 6; // Digit 4
            digit = 4;
            _buffer = _buffer_top;
            break;
        default:
            return; // Invalid section
    }

    if (state) {
        _buffer[(digit-1)] |= 0x10; // Set the decimal point bit
    } else {
        _buffer[(digit-1)] &= ~0x10; // Clear the decimal point bit
    }

    _write(_buffer[(digit-1)], address);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setDecimal(uint8_t digit, bool state, LCDSections section) {
    uint8_t address = 0;
    uint8_t* _buffer = nullptr;
    switch (section) {
        case LCDSections::SECTION_DEFAULT:
            if (digit < 1 || digit > 4) {
                return; // Invalid digit
            }

            address = ADDR_BIG_SEGS + ((6 - digit) * 2);
            _buffer = _buffer_default;
            break;
        case LCDSections::SECTION_TOP:
        case LCDSections::SECTION_CLOCK:
            if (digit < 1 || digit > 3) {
                return; // Invalid digit
            }

            address = ADDR_SMALL_SEGS + ((digit - 1) * 2);
            _buffer = _buffer_top;
            break;
        default:
            return; // Invalid section
    }

    if (state) {
        _buffer[(digit-1)] |= 0x10; // Set the decimal point bit
    } else {
        _buffer[(digit-1)] &= ~0x10; // Clear the decimal point bit
    }

    _write(_buffer[(digit-1)], address);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::writeFloat(float input, uint8_t decimals, LCDSections section) {
    bool isNegative = input < 0.0f;
    float scale = powf(10, decimals);
    long scaled = lroundf(fabsf(input) * scale);

    int totalDigits = _countDigits(scaled);
    int digitCount = totalDigits + (isNegative ? 1 : 0);
    if (decimals > 0 && totalDigits <= decimals) {
        digitCount++;
    }

    int startPos;
    int digitPos = 0;

    switch (section) {
        case LCDSections::SECTION_DEFAULT:
            startPos = 6 - digitCount + 1;
            break;
        case LCDSections::SECTION_TOP:
        case LCDSections::SECTION_CLOCK:
            startPos = 4 - digitCount + 1;
            break;
        default:
            return; // Invalid section
    }

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


void SegLCD_PCF85176_6DigitSignalBatteryProgress::writeChar(uint8_t digit, char c, LCDSections section) {
    uint8_t ch = _mapSegments(_get_char_value(c));

    switch (section) {
        case LCDSections::SECTION_DEFAULT:
            if (digit < 1 || digit > 6) {
                return; // Invalid digit
            }
            _buffer_default[digit-1] = ch;
            _write(_buffer_default[digit-1], ADDR_BIG_SEGS + ((6 - digit) * 2));
            break;
        case LCDSections::SECTION_TOP:
        case LCDSections::SECTION_CLOCK:
            if (digit < 1 || digit > 4) {
                return; // Invalid digit
            }
            _buffer_top[digit-1] = ch;
             _write(_buffer_top[digit-1], ADDR_SMALL_SEGS + ((digit - 1) * 2));
            break;
    }
}

// ABCD_EFGH to ABCH FGED
uint8_t SegLCD_PCF85176_6DigitSignalBatteryProgress::_mapSegments(uint8_t val) {
    uint8_t out = 0;
    out |= (val & 0b11100000);           // ABC: bits 7-5 → 7-5
    out |= (val & 0b00010000) >> 4;      // D: bit 4 → 0
    out |= (val & 0b00001000) >> 2;      // E: bit 3 → 1
    out |= (val & 0b00000100) << 1;      // F: bit 2 → 3
    out |= (val & 0b00000010) << 1;      // G: bit 1 → 2
    out |= (val & 0b00000001) << 4;      // H: bit 0 → 4
    return out;
}