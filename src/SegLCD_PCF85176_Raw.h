#ifndef SEGLCD_PCF85176_RAW_H
#define SEGLCD_PCF85176_RAW_H


#if !defined(SEGLCD_DISABLE_ALL_LCDS) || defined(SEGLCD_ENABLE_PCF85176_RAW)
/**
 * @file SegLCD_PCF85176_Raw.h
 * @brief Raw PCF85176 LCD implementation for prototyping and testing.
 */

#include <SegDriver_PCF85176.h>

/**
 * @brief Raw PCF85176 LCD class for direct RAM access.
 *
 * Use this class to prototype new LCD displays by testing segment mappings
 * before creating a dedicated LCD implementation. Works with PCF85176 and PCF8576.
 */
class SegLCD_PCF85176_Raw : public SegDriver_PCF85176 {
    public:
        SegLCD_PCF85176_Raw(SegTransportI2C& transport, uint8_t address = DEFAULT_PCX85_I2C_ADDRESS, uint8_t subaddress = DEFAULT_SUBADDRESS);
        void init(ModeDrive drive, ModeBias bias);
        size_t write(uint8_t ch) override;
        void writeRam(uint8_t data, uint8_t address = 0);
        void writeRam(uint8_t *data, size_t length, uint8_t address = 0);
};
#endif

#endif
