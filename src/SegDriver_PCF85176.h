#ifndef SEGDRIVER_PCF85176_H
#define SEGDRIVER_PCF85176_H

#include <SegDriver_PCx85.h>

/**
 * @brief Implementation of the PCF85176 controllers.
 */
class SegDriver_PCF85176 : public SegDriver_PCx85 {
    public:
        /**
         * @brief Constructor for PCF85176 segment driver
         *
         * @param transport I2C transport implementation
         * @param address I2C address of the PCF85176 device (default is 0x38)
         * @param subaddress Subaddress for the device (default is 0)
         */
        SegDriver_PCF85176(SegTransportI2C& transport, uint8_t address = DEFAULT_PCF85176_I2C_ADDRESS, uint8_t subaddress = DEFAULT_SUBADDRESS) : SegDriver_PCx85(transport, address, subaddress) {}
};

#endif
