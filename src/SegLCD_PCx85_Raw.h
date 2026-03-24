#ifndef SEGLCD_PCx85_RAW_H
#define SEGLCD_PCx85_RAW_H

/**
 * @file SegLCD_PCx85_Raw.h
 * @brief Raw PCx85 LCD implementation for prototyping and testing.
 */

#include <SegDriver_PCx85.h>

/**
 * @brief Raw PCx85 LCD class for direct RAM access.
 *
 * Use this class to prototype new LCD displays by testing segment mappings
 * before creating a dedicated LCD implementation. Works with PCF85176 and PCF8576.
 */
class SegLCD_PCx85_Raw : public SegDriver_PCx85 {
    public:
        SegLCD_PCx85_Raw(SegTransportI2C& transport, uint8_t address = DEFAULT_PCF85176_I2C_ADDRESS, uint8_t subaddress = DEFAULT_SUBADDRESS);
        void init(ModeDrive drive, ModeBias bias);
        size_t write(uint8_t ch) override;
        void writeRam(uint8_t data, uint8_t address = 0);
        void writeRam(uint8_t *data, size_t length, uint8_t address = 0);
};
#endif
