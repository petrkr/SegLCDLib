#include <SegDriver_HT1621.h>


SegDriver_HT1621::SegDriver_HT1621(SegTransport3Wire& transport, uint8_t chipselect)
    : SegDriver_3Wire(transport, chipselect) {}

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
    }

    command(data);
}
