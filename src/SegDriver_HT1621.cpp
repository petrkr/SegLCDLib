#include <Arduino.h>
#include <SegDriver_HT1621.h>


SegDriver_HT1621::SegDriver_HT1621(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read)
    : SegDriver_3Wire(chipselect, data, write, read) {
    _maxAddress = 30;
}

void SegDriver_HT1621::clear() {
    // TODO: Make this more generic or dynamic
    uint8_t tmp[(MAX_ADDRESS / 2)+1] = { 0 };

    switch (_drive) {
        case MODE_DRIVE_STATIC:
            return;
        //TODO: Get riht size values here
        case MODE_DRIVE_12:
        case MODE_DRIVE_13:
            break;
        case MODE_DRIVE_14:
            _writeRam(tmp, sizeof(tmp), 0);
            return;
        default:
            break;
    }

    SegLCDLib::clear();
}


void SegDriver_HT1621::_setMode(ModeDrive drive, ModeBias bias) {
    _drive = drive; // Store the current drive mode
    uint8_t data;

    data = 0; // Clear data

    // Set bias mode
    switch (bias) {
        case MODE_BIAS_12:
            data |= CMD_BIAS_12; // Set bias mode 1/2
            break;
        case MODE_BIAS_13:
            data |= CMD_BIAS_13; // Set bias mode 1/3
            break;
        default:
            return; // Invalid bias mode, do nothing
    }

    switch (drive) {
        case MODE_DRIVE_STATIC:
            // we do not support static drive mode
            break;
        case MODE_DRIVE_12:
            data |= DRIVE_MODE_12_MASK; // 1/2 multiplexing drive mode
            break;
        case MODE_DRIVE_13:
            data |= DRIVE_MODE_13_MASK; // 1/3 multiplexing drive mode
            break;
        case MODE_DRIVE_14:
            data |= DRIVE_MODE_14_MASK; // 1/4 multiplexing drive mode
            break;
        default:
            return; // Invalid drive mode, do nothing
            break;
    }

    command(data);
}
