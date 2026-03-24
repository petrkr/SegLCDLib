#ifndef SEGLCD_HT1621_RAW_H
#define SEGLCD_HT1621_RAW_H

/**
 * @file SegLCD_HT1621_Raw.h
 * @brief Raw HT1621 LCD implementation for prototyping and testing.
 */

#include <SegDriver_HT1621.h>

/**
 * @brief Raw HT1621 LCD class for direct RAM access.
 *
 * Use this class to prototype new LCD displays by testing segment mappings
 * before creating a dedicated LCD implementation.
 */
class SegLCD_HT1621_Raw : public SegDriver_HT1621 {
    public:
        SegLCD_HT1621_Raw(SegTransport3Wire& transport, uint8_t chipselect);
        void init(ModeDrive drive, ModeBias bias);
        void writeRam(uint8_t data, uint8_t address = 0);
        void writeRam(uint8_t *data, size_t length, uint8_t address = 0);
};
#endif
