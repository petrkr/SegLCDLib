#include <SegLCD_PCF85176_2Row4DigSigBatPwr.h>


SegLCD_PCF85176_2Row4DigSigBatPwr::SegLCD_PCF85176_2Row4DigSigBatPwr(TwoWire& i2c, uint8_t address, uint8_t subaddress) :  SegDriver_PCF85176(i2c, address, subaddress) {
    _allocateBuffer(RAM_SIZE);
}

void SegLCD_PCF85176_2Row4DigSigBatPwr::init() {
    SegDriver_PCF85176::init();
    _setMode(MODE_STATUS_ENABLED);
}

void SegLCD_PCF85176_2Row4DigSigBatPwr::setBatteryLevel(uint8_t level) {

    if (level > MAX_BATTERY_LEVEL)
        level = MAX_BATTERY_LEVEL;

    _ramBuffer[OFFSET_BATT] &= ~(BATTERY_MASK);

    if (level > 0)
        _ramBuffer[OFFSET_BATT] |= BATTERY_LEVEL_SEG[0];
    if (level > 1)
        _ramBuffer[OFFSET_BATT] |= BATTERY_LEVEL_SEG[1];
    if (level > 2)
        _ramBuffer[OFFSET_BATT] |= BATTERY_LEVEL_SEG[2];
    if (level > 3)
        _ramBuffer[OFFSET_BATT] |= BATTERY_LEVEL_SEG[3];

    _writeRam(_ramBuffer[OFFSET_BATT], ADDR_BATT);
}

void SegLCD_PCF85176_2Row4DigSigBatPwr::setSignalLevel(uint8_t level) {

    if (level > MAX_SIGNAL_LEVEL)
        level = MAX_SIGNAL_LEVEL;

    _ramBuffer[OFFSET_SIGNAL] &= ~(SIGNAL_MASK);

    if (level > 0)
        _ramBuffer[OFFSET_SIGNAL] |= SIGNAL_LEVEL_BITS[0];

    _writeRam(_ramBuffer[OFFSET_SIGNAL], ADDR_SIGNAL);
}

void SegLCD_PCF85176_2Row4DigSigBatPwr::setPowerSymbol(bool state) {
    if (state) {
        _ramBuffer[OFFSET_POWER] |= POWER_BIT; // Set the decimal point bit
    } else {
        _ramBuffer[OFFSET_POWER] &= ~POWER_BIT; // Clear the decimal point bit
    }

    _writeRam(_ramBuffer[OFFSET_POWER], ADDR_POWER);
}

bool SegLCD_PCF85176_2Row4DigSigBatPwr::_handleDotNoPrevClear(uint8_t ch) {
    if (_dotWrite(ch, DECIMAL_MIN_COL, DECIMAL_MAX_COL, DECIMAL_COL_OFFSET)) {
        return true;
    }

    if (_isFlagSet(FLAG_PENDING_DOT)) {
        _clearFlag(FLAG_PENDING_DOT);
    }

    _dotClearCur(DECIMAL_MIN_COL, DECIMAL_MAX_COL);
    return false;
}

void SegLCD_PCF85176_2Row4DigSigBatPwr::_setDecimal(uint8_t row, uint8_t col, bool state) {

    if (row < DECIMAL_MIN_ROW || row > DECIMAL_MAX_ROW) {
        return; // Invalid digit
    }

    if (col < DECIMAL_MIN_COL || col > DECIMAL_MAX_COL) {
        return; // Invalid digit
    }

    uint8_t address = 0;
    uint8_t offset = 0;
    uint8_t decimalbit = 0;
    switch (row) {
        case 0:
            address = ADDR_1_SEGS + (col * 2);
            offset = OFFSET_ROW1;
            decimalbit = DECIMAL_POINT_BIT_ROW1;
            break;
        case 1:
            address = ADDR_2_SEGS + (col * 2);
            offset = OFFSET_ROW2;
            decimalbit = DECIMAL_POINT_BIT_ROW2;
            break;
        default:
            return; // Invalid section
    }

    if (state) {
        _ramBuffer[offset + col] |= decimalbit; // Set the decimal point bit
    } else {
        _ramBuffer[offset + col] &= ~decimalbit; // Clear the decimal point bit
    }

    _writeRam(_ramBuffer[offset + col], address);
}

size_t SegLCD_PCF85176_2Row4DigSigBatPwr::write(uint8_t ch) {
    if (_cursorRow > ROW2) {
        return 0;
    }

    if (_cursorRow == ROW1 && _cursorCol > 3) {
        _cursorRow = ROW2;
        _cursorCol = 0;
    } else if (_cursorRow == ROW2 && _cursorCol > 3) {
        return 0;
    }

    uint8_t char_data = _get_char_value(ch);
    uint8_t segment_data = 0;

    switch (_cursorRow) {
        case ROW1:
            segment_data = _mapSegmentsRow1(char_data);
            // Handle decimal point
            if (_handleDotNoPrevClear(ch)) {
                return 1;
            }

            _ramBuffer[OFFSET_ROW1 + _cursorCol] = segment_data;
            _writeRam(_ramBuffer[OFFSET_ROW1 + _cursorCol], ADDR_1_SEGS + (_cursorCol * 2));
            break;
        case ROW2:
            segment_data = _mapSegmentsRow2(char_data);
            // Handle decimal point
            if (_handleDotNoPrevClear(ch)) {
                return 1;
            }

            _ramBuffer[OFFSET_ROW2 + _cursorCol] = segment_data;
            _writeRam(_ramBuffer[OFFSET_ROW2 + _cursorCol], ADDR_2_SEGS + (_cursorCol * 2));
            break;
    }
    _cursorCol++;
    return 1;
}

// ABCD_EFGH to .. AFED_BGCH
uint8_t SegLCD_PCF85176_2Row4DigSigBatPwr::_mapSegmentsRow1(uint8_t val) {
    uint8_t out = 0;

    out |= (val & 0b10010001);        // ADH
    out |= (val & 0b00000100) << 4;   // F
    out |= (val & 0b00001000) << 2;   // E
    out |= (val & 0b01000000) >> 3;   // B
    out |= (val & 0b00000010) << 1;   // G
    out |= (val & 0b00100000) >> 4;   // C

    return out;
}

// ABCD_EFGH to .. DEFA_HCGB
uint8_t SegLCD_PCF85176_2Row4DigSigBatPwr::_mapSegmentsRow2(uint8_t val) {
    uint8_t out = 0;

    out |= (val & 0b00011101) << 3;  // DEFH
    out |= (val & 0b10100000) >> 3;  // AC
    out |= (val & 0b00000010);       // G
    out |= (val & 0b01000000) >> 6;  // B

    return out;
}
