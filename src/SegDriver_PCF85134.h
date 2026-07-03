#ifndef SEGDRIVER_PCF85134_H
#define SEGDRIVER_PCF85134_H

#include <SegDriver_PCx85.h>

/**
 * @brief Implementation of the PCF85134 controller.
 */
class SegDriver_PCF85134 : public SegDriver_PCx85 {
    protected:
        static constexpr uint8_t DEFAULT_PCF85134_I2C_ADDRESS = 0x38;
        static constexpr uint8_t MAX_ADDRESS = 59;

        static constexpr uint8_t CONTROL_LAST_COMMAND = 0x00;
        static constexpr uint8_t CONTROL_COMMAND      = 0x80;
        static constexpr uint8_t CONTROL_DATA         = 0x40;

    public:
        /**
         * @brief Constructor for PCF85134 segment driver.
         *
         * @param transport I2C transport implementation
         * @param address I2C address of the PCF85134 device (default is 0x38)
         * @param subaddress Subaddress for the device (default is 0)
         */
        SegDriver_PCF85134(SegTransportI2C& transport, uint8_t address = DEFAULT_PCF85134_I2C_ADDRESS, uint8_t subaddress = DEFAULT_SUBADDRESS) : SegDriver_PCx85(transport, address, subaddress) {}

    protected:
        void _sendCommand(uint8_t command, bool last = true) override;
        void _writeRam(uint8_t *data, size_t length, uint8_t address = 0) override;
};

#endif
