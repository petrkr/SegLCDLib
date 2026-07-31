#include <Arduino.h>
#include <SegDriver_PCF85134.h>


void SegDriver_PCF85134::_sendCommand(uint8_t command, bool last) {
    uint8_t buffer[2] = {
        last ? CONTROL_LAST_COMMAND : CONTROL_COMMAND,
        static_cast<uint8_t>(command | CONTROL_COMMAND)
    };

    _transport.write(_address, buffer, sizeof(buffer));
}

void SegDriver_PCF85134::_writeRam(uint8_t *data, size_t length, uint8_t address) {
    uint8_t buffer[(MAX_ADDRESS / 2) + 1 + 3];
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

    buffer[0] = CONTROL_COMMAND;
    buffer[1] = address;
    buffer[2] = CONTROL_DATA;
    memcpy(buffer + 3, data, length);

    _transport.write(_address, buffer, length + 3);
}
