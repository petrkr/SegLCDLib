#ifndef SEGLCD_HT1621_4SEGDEGREE_H
#define SEGLCD_HT1621_4SEGDEGREE_H

#include <SegDriver_HT1621.h>

/**
 * @file SegLCD_HT1621_4SegDegree.h
 * @brief HT1621 driver for 4-digit LCD with degree symbol and colon.
 */

/**
 * @brief 4-digit 7-segment LCD with degree symbol and colon (HT1621).
 *
 * Features: degree symbol, colon, decimal points.
 * Integrated HT1621 controller, 3-wire serial protocol.
 *
 * @see examples/HT1621/4DigDeg/4DigDeg.ino
 */
class SegLCD_HT1621_4SegDegree : public SegDriver_HT1621 {
    public:
        SegLCD_HT1621_4SegDegree(SegTransport3Wire& transport, uint8_t chipselect);
        void init() override;
        void clear() override;
        void setCursor(uint8_t row, uint8_t col) override;
        void setDegreeSymbol(bool state, uint8_t index = 0);
        size_t write(uint8_t ch) override;

    private:
        void _setColon(uint8_t row, uint8_t col, bool state) override;
        void _setDecimal(uint8_t row, uint8_t col, bool state) override;
        static constexpr uint8_t RAM_SIZE = 4;
        static constexpr uint8_t DIGITS = 4;
        static constexpr uint8_t ADDR_SYMBOLS = 6;

        // Decimal configuration: dots at positions 0-1, stored in symbols buffer
        static constexpr uint8_t DECIMAL_MIN_COL = 0;
        static constexpr uint8_t DECIMAL_MAX_COL = 1;
        // Note: This LCD uses symbols buffer for dots/colon, not DECIMAL_POINT_BIT

        // Colon configuration
        static constexpr uint8_t COLON_COL = 1;  // Colon at position 1 (between digits 1-2)

        // Flag for colon state
        static constexpr uint8_t FLAG_COLON_DISPLAYED = 0x01;

        uint8_t _mapSegments(uint8_t val);
        void _writeSymbols(uint8_t bitnum, bool state);
};
#endif
