#ifndef SEGDRIVER_PCF85176_H
#define SEGDRIVER_PCF85176_H

#include <SegDriver_PCx85.h>

/**
 * @brief Implementation of the PCF85176 controllers.
 */
class SegDriver_PCF85176 : public SegDriver_PCx85 {
    protected:
        static constexpr uint8_t MAX_ADDRESS = 39;
        static constexpr uint8_t CMD_LAST_COMMAND = 0x80;

    public:
        /**
         * @brief Constructor for PCF85176 segment driver
         *
         * @param transport I2C transport implementation
         * @param address I2C address of the PCF85176 device (default is 0x38)
         * @param subaddress Subaddress for the device (default is 0)
         */
        SegDriver_PCF85176(SegTransportI2C& transport, uint8_t address = DEFAULT_PCX85_I2C_ADDRESS, uint8_t subaddress = DEFAULT_SUBADDRESS) : SegDriver_PCx85(transport, address, subaddress) {}

    protected:
        using SegLCDLib::_writeRam;
        void _sendCommand(uint8_t command, bool last = true) override;
        void _writeRam(uint8_t *data, size_t length, uint8_t address = 0) override;
};

#endif
