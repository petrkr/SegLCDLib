#include <Wire.h>
#include <SegLCD_PCF85176_6DigSigBattProgress.h>


SegLCD_PCF85176_6DigitSignalBatteryProgress::SegLCD_PCF85176_6DigitSignalBatteryProgress(TwoWire& i2c, uint8_t address, uint8_t subaddress) :  SegDriver_PCF85176(i2c, address, subaddress) {
    _allocateBuffer(RAM_SIZE);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::init() {
    SegDriver_PCF85176::init();
    _setMode(MODE_STATUS_ENABLED);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setBatteryLevel(uint8_t level) {

    if (level > 4)
        level = 4;

    _ramBuffer[OFFSET_SIGBATT] &= ~(0x0f);

    if (level > 0)
        _ramBuffer[OFFSET_SIGBATT] |= 8;
    if (level > 1)
        _ramBuffer[OFFSET_SIGBATT] |= 1;
    if (level > 2)
        _ramBuffer[OFFSET_SIGBATT] |= 2;
    if (level > 3)
        _ramBuffer[OFFSET_SIGBATT] |= 4;

    _writeRam(_ramBuffer[OFFSET_SIGBATT], ADDR_SIGNAL_BATT);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setSignalLevel(uint8_t level) {

    if (level > 4)
        level = 4;

    _ramBuffer[OFFSET_SIGBATT] &= ~(0xf0);

    if (level > 0)
        _ramBuffer[OFFSET_SIGBATT] |= 0x80;
    if (level > 1)
        _ramBuffer[OFFSET_SIGBATT] |= 0x40;
    if (level > 2)
        _ramBuffer[OFFSET_SIGBATT] |= 0x20;
    if (level > 3)
        _ramBuffer[OFFSET_SIGBATT] |= 0x10;

    _writeRam(_ramBuffer[OFFSET_SIGBATT], ADDR_SIGNAL_BATT);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setProgress(uint8_t value) {
    _ramBuffer[OFFSET_PROGRESS] = 0;
    _ramBuffer[OFFSET_PROGRESS + 1] = 0;

    if (value > 0) {
        _ramBuffer[OFFSET_PROGRESS] |= 0x80;
    }
    if (value >= 10) {
        _ramBuffer[OFFSET_PROGRESS] |= 0x40;
    }
    if (value >= 20) {
        _ramBuffer[OFFSET_PROGRESS] |= 0x20;
    }
    if (value >= 30) {
        _ramBuffer[OFFSET_PROGRESS] |= 0x10;
    }
    if (value >= 40) {
        _ramBuffer[OFFSET_PROGRESS] |= 0x08;
    }
    if (value >= 50) {
        _ramBuffer[OFFSET_PROGRESS] |= 0x04;
    }
    if (value >= 60) {
        _ramBuffer[OFFSET_PROGRESS] |= 0x02;
    }
    if (value >= 70) {
        _ramBuffer[OFFSET_PROGRESS] |= 0x01;
    }

    if (value >= 80) {
        _ramBuffer[OFFSET_PROGRESS + 1] |= 0x80;
    }
    if (value >= 90) {
        _ramBuffer[OFFSET_PROGRESS + 1] |= 0x40;
    }
    if (value >= 100) {
        _ramBuffer[OFFSET_PROGRESS + 1] |= 0x20;
    }
    if (value >= 110) {
        _ramBuffer[OFFSET_PROGRESS + 1] |= 0x10;
    }
    if (value >= 120) {
        _ramBuffer[OFFSET_PROGRESS + 1] |= 0x08;
    }
    if (value >= 130) {
        _ramBuffer[OFFSET_PROGRESS + 1] |= 0x04;
    }
    if (value >= 140) {
        _ramBuffer[OFFSET_PROGRESS + 1] |= 0x02;
    }
    if (value == 150) {
        _ramBuffer[OFFSET_PROGRESS + 1] |= 0x01;
    }

    _writeRam(&_ramBuffer[OFFSET_PROGRESS], 2, ADDR_PROGRESS);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setWheel(uint16_t value) {
    _ramBuffer[OFFSET_WHEEL] = value & 0xFF;
    _ramBuffer[OFFSET_WHEEL + 1] = (((value >> 8) & 0x0F) << 4);

    _writeRam(&_ramBuffer[OFFSET_WHEEL], 2, ADDR_WHEEL);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setLabels(LabelFlags labels) {
    _ramBuffer[OFFSET_LABELS] |= labels;

    _writeRam(_ramBuffer[OFFSET_LABELS], ADDR_PRES_LABELS);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::clearLabels(LabelFlags labels) {
    _ramBuffer[OFFSET_LABELS] &= ~labels;

    _writeRam(_ramBuffer[OFFSET_LABELS], ADDR_PRES_LABELS);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setColon(uint8_t row, uint8_t col, bool state) {

    uint8_t address = 0;
    uint8_t offset = 0;
    uint8_t digit = 0;
    switch (row) {
        case 0:
            if (col != 2) {
                return; // Invalid digit
            }
            digit = 4;
            address = ADDR_SMALL_SEGS + 6; // Digit 4
            offset = OFFSET_TOP;
            break;
        case 1:
            if (col != 4) {
                return; // Invalid digit
            }

            address = ADDR_BIG_SEGS; // Digit 6
            offset = OFFSET_DEFAULT;
            digit = 1;  // Use digit=1 so offset+(digit-1)=offset
            break;
        default:
            return; // Invalid section
    }

    if (state) {
        _ramBuffer[offset + (digit-1)] |= 0x10; // Set the decimal point bit
    } else {
        _ramBuffer[offset + (digit-1)] &= ~0x10; // Clear the decimal point bit
    }

    _writeRam(_ramBuffer[offset + (digit-1)], address);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setDecimal(uint8_t row, uint8_t col, bool state) {

    uint8_t address = 0;
    uint8_t offset = 0;
    switch (row) {
        case 0:
            if (col < 0 || col > 2) {
                return; // Invalid digit
            }

            address = ADDR_SMALL_SEGS + (col * 2);
            offset = OFFSET_TOP;
            break;
        case 1:
            if (col < 0 || col > 4) {
                return; // Invalid digit
            }

            address = ADDR_BIG_SEGS + ((6 - col - 1) * 2);
            offset = OFFSET_DEFAULT + (5 - col);  // Reverse order in buffer
            col = 0;  // Use offset directly, ignore col
            break;
        default:
            return; // Invalid section
    }

    if (state) {
        _ramBuffer[offset + col] |= DECIMAL_POINT_BIT; // Set the decimal point bit
    } else {
        _ramBuffer[offset + col] &= ~DECIMAL_POINT_BIT; // Clear the decimal point bit
    }

    _writeRam(_ramBuffer[offset + col], address);
}

void SegLCD_PCF85176_6DigitSignalBatteryProgress::setCursor(uint8_t row, uint8_t col) {
    if (row == 0 && col < 2) {
        _clearFlag(FLAG_COLON_TOP);
    }

    if (row == 1 && col < 5) {
        _clearFlag(FLAG_COLON_DEFAULT);
    }

    SegDriver_PCF85176::setCursor(row, col);
}

size_t SegLCD_PCF85176_6DigitSignalBatteryProgress::write(uint8_t ch) {

    // Wrap to next row if cursor exceeds current row capacity
    if (_cursorRow == 0 && _cursorCol >= 4) {
        _cursorRow = 1;
        _cursorCol = 0;
    } else if (_cursorRow == 1 && _cursorCol >= 6) {
        return 0;  // End of display
    }

    switch (_cursorRow) {
        case 0: {
            // Top row (4 digits)

            // Handle decimal point and colon
            if (_handleSpecialChars(ch, DECIMAL_TOP_MIN_COL, DECIMAL_TOP_MAX_COL, -1,
                                    true, COLON_TOP_COL, FLAG_COLON_TOP)) {
                return 1;
            }

            // Clear colon if not flagged (additional handling needed because of colon)
            _colonClearIfNotFlagged(ch, COLON_TOP_COL, FLAG_COLON_TOP);

            // Regular character
            uint8_t segment_data = _mapSegments(_get_char_value(ch));

            // Preserve colon bit after colon position
            if (_cursorCol == 3 && _isFlagSet(FLAG_COLON_TOP)) {
                segment_data |= _ramBuffer[OFFSET_TOP + _cursorCol] & DECIMAL_POINT_BIT;
            }

            _ramBuffer[OFFSET_TOP + _cursorCol] = segment_data;
            _writeRam(_ramBuffer[OFFSET_TOP + _cursorCol], ADDR_SMALL_SEGS + (_cursorCol * 2));
            _cursorCol++;
            return 1;
        }
        case 1: {
            // Bottom row (6 digits)

            // Handle decimal point and colon
            if (_handleSpecialChars(ch, DECIMAL_BOTTOM_MIN_COL, DECIMAL_BOTTOM_MAX_COL, -1,
                                    true, COLON_BOTTOM_COL, FLAG_COLON_DEFAULT)) {
                return 1;
            }

            // Clear colon if not flagged (additional handling needed because of colon)
            _colonClearIfNotFlagged(ch, COLON_BOTTOM_COL, FLAG_COLON_DEFAULT);

            // Regular character
            uint8_t segment_data = _mapSegments(_get_char_value(ch));

            // Store in reverse order in buffer for flush compatibility
            uint8_t bufIdx = OFFSET_DEFAULT + (5 - _cursorCol);

            // Preserve colon bit after colon position
            if (_cursorCol == 5 && _isFlagSet(FLAG_COLON_DEFAULT)) {
                segment_data |= _ramBuffer[bufIdx] & DECIMAL_POINT_BIT;
            }

            _ramBuffer[bufIdx] = segment_data;
            _writeRam(segment_data, ADDR_BIG_SEGS + ((6 - _cursorCol - 1) * 2));
            _cursorCol++;
            return 1;
        }
        default:
            return 0;  // Invalid row
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