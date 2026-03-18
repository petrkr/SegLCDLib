#ifndef SEGLCD_HT1621_LCM0844_H
#define SEGLCD_HT1621_LCM0844_H

#include <SegDriver_HT1621.h>

/**
 * @file SegLCD_HT1621_LCM0844.h
 * @brief HT1621 driver for 6-digit LCD with battery indicator.
 */

/**
 * @brief 6-digit 7-segment LCD with battery indicator (HT1621).
 *
 * Features: battery level indicator, decimal points.
 * Integrated HT1621 controller, 3-wire serial protocol.
 */
class SegLCD_HT1621_LCM0844 : public SegDriver_HT1621 {
    public:
        SegLCD_HT1621_LCM0844(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);
        void init() override;
        void setBatteryLevel(uint8_t level);
        void setLoadLevel(uint8_t level);
        size_t write(uint8_t ch) override;

    private:

        /*
        Address mapping (from HT point view - 1 byte = 2 addresses):
        0x00 - Symbols ac/pv2/pv1/input/batt/M/k/2x arrows
        0x02 - digit 1 (dot)
        0x04 - digit 2 (dot)
        0x06 - digit 3 (symbol Hz instead of dot)
        0x08 - Symbols Wh, VA, H, M, %
        0x0A - digit 4 (nothing at dot segment)
        0x0C - digit 5 (rectangle around digits 4,5 instead of dot)
        0x0E - Symbols clock, maintenance, Error, output, batt, load, "k"
        0x10 - digit 6 (dot)
        0x12 - digit 7 (dot)
        0x14 - digit 8 (symbol Hz instead of dot)
        0x16 - Symbols w, VA, underlines (under digits), 2x arrows, no-sound symbol, overload symbol
        0x18 - Symbols bulb, percent progress near bulb (load), dots heading from symbol to bulb
          - 0x40 - bulb
          - 0x20 - middle circle
          - 0x10 - line from middle circle ---> bulb symbol
        0x1A - Symbols AGM, FLD, USER, AC/DC convert, arrows near AC/DC symbol
        0x1C - Symbols battery gauge, DC/DC convert, arrow from DC/DC
        0x1E - Symbols PV1, PV2, AC(line), arrows from pvs, ac and arrow from DC/DC to batt
        */

        void _setDecimal(uint8_t row, uint8_t col, bool state) override;
        static constexpr uint8_t RAM_SIZE = 16;
        static constexpr uint8_t DIGITS = 8;

        // Decimal configuration (physical digits 2,3,4 have decimal points)
        static constexpr uint8_t DECIMAL_MIN_COL = 0;
        static constexpr uint8_t DECIMAL_MAX_COL = 7;
        static constexpr uint8_t DECIMAL_POINT_BIT = 0x01;
        static constexpr int8_t DECIMAL_COL_OFFSET = -1;  // -1: dot belongs to previous position

        // Battery level constants
        static constexpr uint8_t MAX_BATTERY_LEVEL = 5;
        static constexpr uint8_t BATTERY_MASK = 0x4F;
        static constexpr uint8_t BATTERY_LEVEL_ADR = 0x1C;

        // Load level constants
        static constexpr uint8_t MAX_LOAD_LEVEL = 5;
        static constexpr uint8_t LOAD_MASK = 0x8F;
        static constexpr uint8_t LOAD_LEVEL_ADR = 0x18;

        uint8_t _mapSegments(uint8_t val);
};
#endif
