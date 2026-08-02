#ifndef SEGLCD_PCF85134_XYGAX_SEG_I2C_H
#define SEGLCD_PCF85134_XYGAX_SEG_I2C_H


#if !defined(SEGLCD_DISABLE_ALL_LCDS) || defined(SEGLCD_ENABLE_PCF85134_XYGAX_SEG_I2C)
#include <SegDriver_PCF85134.h>

/**
 * @file SegLCD_PCF85134_XygaxSegI2C.h
 * @brief PCF85134 driver for Xygax SEG_I2C LCD board.
 */

/**
 * @brief Xygax SEG_I2C LCD board with flexible segment layout (PCF85134).
 *
 * Up to 6 segments per digit. I2C interface (SA0 pin sets address, A0-A2 are subaddresses).
 *
 * @see examples/PCF85134/XygaxSegI2C/XygaxSegI2C.ino
 */
class SegLCD_PCF85134_XygaxSegI2C : public SegDriver_PCF85134 {
    public:
        SegLCD_PCF85134_XygaxSegI2C(SegTransportI2C& transport, uint8_t address = DEFAULT_PCX85_I2C_ADDRESS, uint8_t subaddress = DEFAULT_SUBADDRESS);
        void init() override;
        using SegLCDLib::write;
        size_t write(uint8_t ch) override;

    private:
        void _setDecimal(uint8_t row, uint8_t col, bool state) override;
        static constexpr uint8_t RAM_SIZE = 6;

        static constexpr uint8_t DIGITS = 6;

        // Decimal configuration: RAM offset -1 (dot belongs to previous position)
        static constexpr uint8_t DECIMAL_POINT_BIT = 0x01;
        static constexpr uint8_t DECIMAL_MIN_COL = 0;
        static constexpr uint8_t DECIMAL_MAX_COL = DIGITS-1; // All digits supports decimal
        static constexpr int8_t DECIMAL_COL_OFFSET = -1;  // -1: dot belongs to previous position
};
#endif

#endif
