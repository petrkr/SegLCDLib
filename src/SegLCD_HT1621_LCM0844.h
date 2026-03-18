#ifndef SEGLCD_HT1621_LCM0844_H
#define SEGLCD_HT1621_LCM0844_H

#include <SegDriver_HT1621.h>

/**
 * @file SegLCD_HT1621_LCM0844.h
 * @brief HT1621 driver for 6-digit LCD with battery indicator.
 */

/**
 * @brief LCM0844 LCD (HT1621).
 *
 * Integrated HT1621 controller, 3-wire serial protocol.
 */
class SegLCD_HT1621_LCM0844 : public SegDriver_HT1621 {
    public:
        typedef enum : uint32_t {
            LABEL_AC                    = (1UL << 0),
            LABEL_PV1                   = (1UL << 1),
            LABEL_PV2                   = (1UL << 2),
            LABEL_INPUT                 = (1UL << 3),

            LABEL_LEFT_BATT             = (1UL << 4),
            LABEL_LEFT_KILO_PREFIX      = (1UL << 5),
            LABEL_LEFT_MEGA_PREFIX      = (1UL << 6),

            LABEL_LEFT_WATT             = (1UL << 7),
            LABEL_LEFT_AMPERE           = (1UL << 8),
            LABEL_LEFT_VOLT             = (1UL << 9),
            LABEL_LEFT_PERCENT          = (1UL << 10),
            LABEL_LEFT_WATT_HOUR_SUFFIX = (1UL << 11),

            LABEL_CENTER_HOUR           = (1UL << 12),
            LABEL_CENTER_MINUTE         = (1UL << 13),
            LABEL_LEFT_HZ               = (1UL << 14),

            LABEL_RIGHT_BATT            = (1UL << 15),
            LABEL_OUTPUT                = (1UL << 16),
            LABEL_LOAD                  = (1UL << 17),
            LABEL_OVERLOAD              = (1UL << 18),
            LABEL_ERROR                 = (1UL << 19),

            LABEL_RIGHT_KILO_PREFIX      = (1UL << 20),
            LABEL_RIGHT_WATT             = (1UL << 21),
            LABEL_RIGHT_AMPERE           = (1UL << 22),
            LABEL_RIGHT_VOLT             = (1UL << 23),
            LABEL_RIGHT_PERCENT          = (1UL << 24),
            LABEL_RIGHT_WATT_HOUR_SUFFIX = (1UL << 25),
            LABEL_RIGHT_MEGA_PREFIX      = (1UL << 26),
            LABEL_RIGHT_HZ               = (1UL << 27),

            LABEL_BATT_AGM               = (1UL << 28),
            LABEL_BATT_FLD               = (1UL << 29),
            LABEL_BATT_USER              = (1UL << 30),
        } LabelFlags;

        typedef enum : uint8_t {
            SYMBOL_CLOCK            = (1U << 0),
            SYMBOL_MAINTENANCE      = (1U << 1),
            SYMBOL_WARNING_TRIANGLE = (1U << 2),

            SYMBOL_LEFT_ARROWS      = (1U << 3),
            SYMBOL_RIGHT_ARROWS     = (1U << 4),
            SYMBOL_UNDERLINES       = (1U << 5),
            SYMBOL_MUTE             = (1U << 6),

            SYMBOL_CENTER_FRAME     = (1U << 7),
        } SymbolFlags;

        SegLCD_HT1621_LCM0844(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);
        void init() override;
        void setBatteryLevel(uint8_t level);
        void setLoadLevel(uint8_t level);
        void setLabels(uint32_t labels);
        void clearLabels(uint32_t labels);
        void setSymbols(uint8_t symbols);
        void clearSymbols(uint8_t symbols);
        size_t write(uint8_t ch) override;

    private:

        // Label bit constants
        static constexpr uint8_t LABEL_BIT_AC                    = 0x80;
        static constexpr uint8_t LABEL_BIT_PV2                   = 0x40;
        static constexpr uint8_t LABEL_BIT_PV1                   = 0x20;
        static constexpr uint8_t LABEL_BIT_INPUT                 = 0x08;
        static constexpr uint8_t LABEL_BIT_LEFT_BATT             = 0x04;
        static constexpr uint8_t LABEL_BIT_LEFT_MEGA_PREFIX      = 0x02;
        static constexpr uint8_t LABEL_BIT_LEFT_KILO_PREFIX      = 0x01;

        static constexpr uint8_t LABEL_BIT_LEFT_WATT             = 0x80;
        static constexpr uint8_t LABEL_BIT_LEFT_AMPERE           = 0x40;
        static constexpr uint8_t LABEL_BIT_LEFT_VOLT             = 0x20;
        static constexpr uint8_t LABEL_BIT_LEFT_PERCENT          = 0x10;
        static constexpr uint8_t LABEL_BIT_CENTER_HOUR           = 0x08;
        static constexpr uint8_t LABEL_BIT_CENTER_MINUTE         = 0x04;
        static constexpr uint8_t LABEL_BIT_LEFT_WATT_HOUR_SUFFIX = 0x01;

        static constexpr uint8_t LABEL_BIT_LEFT_HZ               = 0x01;

        static constexpr uint8_t LABEL_BIT_OUTPUT                = 0x08;
        static constexpr uint8_t LABEL_BIT_RIGHT_BATT            = 0x04;
        static constexpr uint8_t LABEL_BIT_LOAD                  = 0x02;
        static constexpr uint8_t LABEL_BIT_RIGHT_KILO_PREFIX     = 0x01;
        static constexpr uint8_t LABEL_BIT_ERROR                 = 0x40;

        static constexpr uint8_t LABEL_BIT_RIGHT_HZ              = 0x01;

        static constexpr uint8_t LABEL_BIT_RIGHT_WATT            = 0x80;
        static constexpr uint8_t LABEL_BIT_RIGHT_AMPERE          = 0x40;
        static constexpr uint8_t LABEL_BIT_RIGHT_VOLT            = 0x20;
        static constexpr uint8_t LABEL_BIT_RIGHT_PERCENT         = 0x10;
        static constexpr uint8_t LABEL_BIT_OVERLOAD              = 0x08;

        static constexpr uint8_t LABEL_BIT_BATT_AGM              = 0x20;
        static constexpr uint8_t LABEL_BIT_BATT_FLD              = 0x40;
        static constexpr uint8_t LABEL_BIT_BATT_USER             = 0x80;

        static constexpr uint8_t LABEL_BIT_RIGHT_WATT_HOUR_SUFFIX = 0x80;

        static constexpr uint8_t LABEL_BIT_RIGHT_MEGA_PREFIX     = 0x20;

        // Symbol bit constants
        static constexpr uint8_t SYMBOL_BIT_LEFT_ARROWS      = 0x10;
        static constexpr uint8_t SYMBOL_BIT_CENTER_FRAME     = 0x01;
        static constexpr uint8_t SYMBOL_BIT_CLOCK            = 0x10;
        static constexpr uint8_t SYMBOL_BIT_MAINTENANCE      = 0x20;
        static constexpr uint8_t SYMBOL_BIT_WARNING_TRIANGLE = 0x80;
        static constexpr uint8_t SYMBOL_BIT_RIGHT_ARROWS     = 0x02;
        static constexpr uint8_t SYMBOL_BIT_UNDERLINES       = 0x01;
        static constexpr uint8_t SYMBOL_BIT_MUTE             = 0x04;

        static constexpr uint8_t RAM_SIZE = 16;
        static constexpr uint8_t DIGITS = 8;

        static constexpr uint8_t DECIMAL_DIGITS[4] = {0, 1, 5, 6};
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

        // Label/Symbol mapping structures
        struct FlagBit {
            uint32_t flag;
            uint8_t bit;
        };

        struct AddressMapping {
            uint8_t address;
            FlagBit bits[8];  // terminated by {0, 0}
        };

        static constexpr AddressMapping LABEL_MAP[] = {
            {0x00, {
                    {LABEL_AC, LABEL_BIT_AC},
                    {LABEL_PV2, LABEL_BIT_PV2},
                    {LABEL_PV1, LABEL_BIT_PV1},
                    {LABEL_INPUT, LABEL_BIT_INPUT},
                    {LABEL_LEFT_BATT, LABEL_BIT_LEFT_BATT},
                    {LABEL_LEFT_MEGA_PREFIX, LABEL_BIT_LEFT_MEGA_PREFIX},
                    {LABEL_LEFT_KILO_PREFIX, LABEL_BIT_LEFT_KILO_PREFIX}
                }
            },
            {0x06, {
                    {LABEL_LEFT_HZ, LABEL_BIT_LEFT_HZ}
                }
            },
            {0x08, {
                    {LABEL_LEFT_WATT, LABEL_BIT_LEFT_WATT},
                    {LABEL_LEFT_AMPERE, LABEL_BIT_LEFT_AMPERE},
                    {LABEL_LEFT_VOLT, LABEL_BIT_LEFT_VOLT},
                    {LABEL_LEFT_PERCENT, LABEL_BIT_LEFT_PERCENT},
                    {LABEL_CENTER_HOUR, LABEL_BIT_CENTER_HOUR},
                    {LABEL_CENTER_MINUTE, LABEL_BIT_CENTER_MINUTE},
                    {LABEL_LEFT_WATT_HOUR_SUFFIX, LABEL_BIT_LEFT_WATT_HOUR_SUFFIX}
                }
            },
            {0x0E, {
                    {LABEL_OUTPUT, LABEL_BIT_OUTPUT},
                    {LABEL_RIGHT_BATT, LABEL_BIT_RIGHT_BATT},
                    {LABEL_LOAD, LABEL_BIT_LOAD},
                    {LABEL_RIGHT_KILO_PREFIX, LABEL_BIT_RIGHT_KILO_PREFIX},
                    {LABEL_ERROR, LABEL_BIT_ERROR}
                }
            },
            {0x14, {
                    {LABEL_RIGHT_HZ, LABEL_BIT_RIGHT_HZ}
                }
            },
            {0x16, {
                    {LABEL_RIGHT_WATT, LABEL_BIT_RIGHT_WATT},
                    {LABEL_RIGHT_AMPERE, LABEL_BIT_RIGHT_AMPERE},
                    {LABEL_RIGHT_VOLT, LABEL_BIT_RIGHT_VOLT},
                    {LABEL_RIGHT_PERCENT, LABEL_BIT_RIGHT_PERCENT},
                    {LABEL_OVERLOAD, LABEL_BIT_OVERLOAD}
                }
            },
            {0x1A, {
                    {LABEL_BATT_AGM, LABEL_BIT_BATT_AGM},
                    {LABEL_BATT_FLD, LABEL_BIT_BATT_FLD},
                    {LABEL_BATT_USER, LABEL_BIT_BATT_USER}
                }
            },
            {0x1C, {
                    {LABEL_RIGHT_WATT_HOUR_SUFFIX, LABEL_BIT_RIGHT_WATT_HOUR_SUFFIX}
                }
            },
            {0x1E, {
                    {LABEL_RIGHT_MEGA_PREFIX, LABEL_BIT_RIGHT_MEGA_PREFIX}
                }
            },
        };

        static constexpr AddressMapping SYMBOL_MAP[] = {
            {0x00, {{SYMBOL_LEFT_ARROWS, SYMBOL_BIT_LEFT_ARROWS}}},
            {0x0C, {{SYMBOL_CENTER_FRAME, SYMBOL_BIT_CENTER_FRAME}}},
            {0x0E, {{SYMBOL_CLOCK, SYMBOL_BIT_CLOCK}, {SYMBOL_MAINTENANCE, SYMBOL_BIT_MAINTENANCE}, {SYMBOL_WARNING_TRIANGLE, SYMBOL_BIT_WARNING_TRIANGLE}}},
            {0x16, {{SYMBOL_RIGHT_ARROWS, SYMBOL_BIT_RIGHT_ARROWS}, {SYMBOL_UNDERLINES, SYMBOL_BIT_UNDERLINES}, {SYMBOL_MUTE, SYMBOL_BIT_MUTE}}},
        };

        void _setDecimal(uint8_t row, uint8_t col, bool state) override;

        void _updateLabels(uint32_t labels, bool set);
        void _updateSymbols(uint8_t symbols, bool set);

        uint8_t _mapSegments(uint8_t val);
};
#endif
