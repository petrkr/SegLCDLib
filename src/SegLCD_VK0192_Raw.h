#ifndef SEGLCD_VK0192_RAW_H
#define SEGLCD_VK0192_RAW_H

/**
 * @file SegLCD_VK0192_Raw.h
 * @brief Raw VK0192 LCD implementation for prototyping and testing.
 */

#include <SegDriver_VK0192.h>

/**
 * @brief Raw VK0192 LCD class for direct RAM access.
 *
 * Use this class to prototype new LCD displays by testing segment mappings
 * before creating a dedicated LCD implementation.
 */
class SegLCD_VK0192_Raw : public SegDriver_VK0192 {
    public:
        SegLCD_VK0192_Raw(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);
        void writeRam(uint8_t data, uint8_t address = 0);
        void writeRam(uint8_t *data, size_t length, uint8_t address = 0);
};
#endif
