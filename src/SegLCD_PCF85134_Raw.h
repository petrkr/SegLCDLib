#ifndef SEGLCD_PCF85134_RAW_H
#define SEGLCD_PCF85134_RAW_H

/**
 * @file SegLCD_PCF85134_Raw.h
 * @brief Raw PCF85134 LCD implementation for prototyping and testing.
 */

#include <SegDriver_PCF85134.h>

/**
 * @brief Raw PCF85134 LCD class for direct RAM access.
 *
 * Use this class to prototype new LCD displays by testing segment mappings
 * before creating a dedicated LCD implementation.
 */
class SegLCD_PCF85134_Raw : public SegDriver_PCF85134 {
    public:
        SegLCD_PCF85134_Raw(SegTransportI2C& transport, uint8_t address = DEFAULT_PCF85134_I2C_ADDRESS, uint8_t subaddress = DEFAULT_SUBADDRESS);
        void init(ModeDrive drive, ModeBias bias);
        size_t write(uint8_t ch) override;
        void writeRam(uint8_t data, uint8_t address = 0);
        void writeRam(uint8_t *data, size_t length, uint8_t address = 0);
};
#endif
