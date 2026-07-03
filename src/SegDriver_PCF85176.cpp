#include <Arduino.h>
#include <SegDriver_PCF85176.h>


void SegDriver_PCF85176::_sendCommand(uint8_t command, bool last) {
    if (last) {
        command |= CMD_LAST_COMMAND;
    }

    _transport.write(_address, command);
}

void SegDriver_PCF85176::_writeRam(uint8_t *data, size_t length, uint8_t address) {
    uint8_t buffer[(MAX_ADDRESS / 2) + 2];
    size_t maxLength;

    if (!_autoFlush) {
        return;
    }

    if (address > MAX_ADDRESS) {
        return;
    }

    maxLength = ((MAX_ADDRESS - address) / 2) + 1;
    if (length > maxLength) {
        length = maxLength;
    }

    _sendCommand(CMD_DEVICE_SELECT | _subaddress);

    buffer[0] = address;
    memcpy(buffer + 1, data, length);
    _transport.write(_address, buffer, length + 1);
}
