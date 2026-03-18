#ifndef SEGLCD_HT1621_LCM88128_H
#define SEGLCD_HT1621_LCM88128_H

#include <SegDriver_HT1621.h>

/**
 * @file SegLCD_HT1621_LCM88128.h
 * @brief HT1621 driver for LCM88128 LCD with battery, PV panel, and diagram indicators.
 */

/**
 * @brief LCM88128 LCD (HT1621).
 *
 * Integrated HT1621 controller, 3-wire serial protocol.
 * Address-compatible with LCM0844 (same digit layout), but different icons/labels/symbols.
 */
class SegLCD_HT1621_LCM88128 : public SegDriver_HT1621 {
    public:
        typedef enum : uint32_t {
            // Address 0x00
            UNIT_LEFT_KILO_PREFIX       = (1UL << 0),   // 0x00:0x01

            // Address 0x06
            UNIT_LEFT_HZ                = (1UL << 1),   // 0x06:0x01

            // Address 0x08
            UNIT_LEFT_WATT              = (1UL << 2),   // 0x08:0x80
            UNIT_LEFT_AMPERE            = (1UL << 3),   // 0x08:0x40
            UNIT_LEFT_VOLT              = (1UL << 4),   // 0x08:0x20
            UNIT_LEFT_DEGREE_SLASH      = (1UL << 5),   // 0x08:0x10 (slash only)
            UNIT_LEFT_DEGREE            = (1UL << 6),   // 0x08:0x02 (degree circle only)
            UNIT_LEFT_CELSIUS           = (1UL << 7),   // 0x08:0x01
            UNIT_LEFT_PERCENT           = UNIT_LEFT_DEGREE_SLASH | UNIT_LEFT_DEGREE, // 0x08:0x12
            UNIT_LEFT_DEGREE_CELSIUS    = UNIT_LEFT_DEGREE | UNIT_LEFT_CELSIUS,      // 0x08:0x03
            UNIT_CENTER_HOUR            = (1UL << 8),   // 0x08:0x08
            UNIT_CENTER_MINUTE          = (1UL << 9),   // 0x08:0x04

            // Address 0x0E
            UNIT_RIGHT_KILO_PREFIX      = (1UL << 10),  // 0x0E:0x01

            // Address 0x14
            UNIT_RIGHT_HZ               = (1UL << 11),  // 0x14:0x01

            // Address 0x16
            UNIT_RIGHT_WATT             = (1UL << 12),  // 0x16:0x80
            UNIT_RIGHT_AMPERE           = (1UL << 13),  // 0x16:0x40
            UNIT_RIGHT_VOLT             = (1UL << 14),  // 0x16:0x20
            UNIT_RIGHT_PERCENT          = (1UL << 15),  // 0x16:0x10
        } UnitFlags;

        typedef enum : uint32_t {
            // Address 0x00
            LABEL_AC                    = (1UL << 0),   // 0x00:0x40
            LABEL_PV                    = (1UL << 1),   // 0x00:0x20
            LABEL_INV                   = (1UL << 2),   // 0x00:0x10
            LABEL_INPUT                 = (1UL << 3),   // 0x00:0x08
            LABEL_LEFT_BATT             = (1UL << 4),   // 0x00:0x04
            LABEL_WP                    = (1UL << 5),   // 0x00:0x80
            LABEL_TEMP                  = (1UL << 6),   // 0x00:0x02

            // Address 0x0E
            LABEL_OUTPUT                = (1UL << 7),   // 0x0E:0x08
            LABEL_RIGHT_BATT            = (1UL << 8),   // 0x0E:0x04
            LABEL_LOAD                  = (1UL << 9),   // 0x0E:0x02
            LABEL_ERROR                 = (1UL << 10),  // 0x0E:0x40

            // Address 0x16
            LABEL_OVERLOAD              = (1UL << 11),  // 0x16:0x08
            LABEL_BYPASS                = (1UL << 12),  // 0x16:0x02

            // Address 0x1A
            LABEL_BATT_SLA              = (1UL << 13),  // 0x1A:0x40
            LABEL_BATT_LI               = (1UL << 14),  // 0x1A:0x80
            LABEL_ECO                   = (1UL << 15),  // 0x1A:0x01

            // Address 0x18
            LABEL_P                     = (1UL << 17),  // 0x18:0x10

            // Address 0x1C
            LABEL_CHARGING              = (1UL << 18),  // 0x1C:0x80
        } LabelFlags;

        typedef enum : uint32_t {
            // Address 0x0E
            SYMBOL_CLOCK                   = (1UL << 0),   // 0x0E:0x10
            SYMBOL_MAINTENANCE             = (1UL << 1),   // 0x0E:0x20
            SYMBOL_WARNING_TRIANGLE        = (1UL << 2),   // 0x0E:0x80

            // Address 0x16
            SYMBOL_UNDERLINES              = (1UL << 3),   // 0x16:0x01
            SYMBOL_MUTE                    = (1UL << 4),   // 0x16:0x04

            // Address 0x0C
            SYMBOL_CENTER_FRAME            = (1UL << 5),   // 0x0C:0x01

            // Address 0x18
            SYMBOL_BULB                    = (1UL << 6),   // 0x18:0x40
            SYMBOL_LINE_DCAC_TO_LOAD       = (1UL << 7),   // 0x18:0x20

            // Address 0x1A
            SYMBOL_LINE_ACDC_TO_DCAC       = (1UL << 8),   // 0x1A:0x02
            SYMBOL_LINE_ACDC_TO_BATT       = (1UL << 9),   // 0x1A:0x04
            SYMBOL_LINE_BATT_TO_DCAC       = (1UL << 10),  // 0x1A:0x08
            SYMBOL_LINE_AC_TO_LOAD         = (1UL << 11),  // 0x1A:0x10
            SYMBOL_DC_AC_SYMBOL            = (1UL << 12),  // 0x1A:0x20

            // Address 0x1C
            SYMBOL_LINE_PV_TO_ACDC        = (1UL << 13),  // 0x1C:0x10
            SYMBOL_AC_DC_SYMBOL            = (1UL << 14),  // 0x1C:0x20

            // Address 0x1E
            SYMBOL_LINE_WIND_TO_ACDC       = (1UL << 15),  // 0x1E:0x10
            SYMBOL_LINE_AC_TO_ACDC         = (1UL << 16),  // 0x1E:0x20
            SYMBOL_AC_SYMBOL               = (1UL << 17),  // 0x1E:0x80
            SYMBOL_WIND                    = (1UL << 18),  // 0x1E:0x40
        } SymbolFlags;

        SegLCD_HT1621_LCM88128(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);
        void init() override;
        void setBatteryLevel(uint8_t level);
        void setLoadLevel(uint8_t level);
        void setPVLevel(uint8_t level);
        void setUnits(uint32_t units);
        void clearUnits(uint32_t units);
        void setLabels(uint32_t labels);
        void clearLabels(uint32_t labels);
        void setSymbols(uint32_t symbols);
        void clearSymbols(uint32_t symbols);
        size_t write(uint8_t ch) override;

    private:

        // Unit bit constants — address 0x00
        static constexpr uint8_t UNIT_BIT_LEFT_KILO_PREFIX       = 0x01;

        // Unit bit constants — address 0x06
        static constexpr uint8_t UNIT_BIT_LEFT_HZ                = 0x01;

        // Unit bit constants — address 0x08
        static constexpr uint8_t UNIT_BIT_LEFT_WATT              = 0x80;
        static constexpr uint8_t UNIT_BIT_LEFT_AMPERE            = 0x40;
        static constexpr uint8_t UNIT_BIT_LEFT_VOLT              = 0x20;
        static constexpr uint8_t UNIT_BIT_LEFT_DEGREE_SLASH      = 0x10;
        static constexpr uint8_t UNIT_BIT_LEFT_DEGREE            = 0x02; // degree circle only
        static constexpr uint8_t UNIT_BIT_LEFT_CELSIUS           = 0x01;
        static constexpr uint8_t UNIT_BIT_CENTER_HOUR            = 0x08;
        static constexpr uint8_t UNIT_BIT_CENTER_MINUTE          = 0x04;

        static constexpr uint8_t UNIT_BIT_LEFT_PERCENT           = UNIT_BIT_LEFT_DEGREE_SLASH | UNIT_BIT_LEFT_DEGREE;
        static constexpr uint8_t UNIT_BIT_LEFT_DEG_CELSIUS       = UNIT_BIT_LEFT_DEGREE | UNIT_BIT_LEFT_CELSIUS;

        // Unit bit constants — address 0x0E
        static constexpr uint8_t UNIT_BIT_RIGHT_KILO_PREFIX      = 0x01;

        // Unit bit constants — address 0x14
        static constexpr uint8_t UNIT_BIT_RIGHT_HZ               = 0x01;

        // Unit bit constants — address 0x16
        static constexpr uint8_t UNIT_BIT_RIGHT_WATT             = 0x80;
        static constexpr uint8_t UNIT_BIT_RIGHT_AMPERE           = 0x40;
        static constexpr uint8_t UNIT_BIT_RIGHT_VOLT             = 0x20;
        static constexpr uint8_t UNIT_BIT_RIGHT_PERCENT          = 0x10;

        // Label bit constants — address 0x00
        static constexpr uint8_t LABEL_BIT_TEMP                  = 0x02;
        static constexpr uint8_t LABEL_BIT_AC                    = 0x40;
        static constexpr uint8_t LABEL_BIT_PV                    = 0x20;
        static constexpr uint8_t LABEL_BIT_INV                   = 0x10;
        static constexpr uint8_t LABEL_BIT_INPUT                 = 0x08;
        static constexpr uint8_t LABEL_BIT_LEFT_BATT             = 0x04;
        static constexpr uint8_t LABEL_BIT_WP                    = 0x80;

        // Label bit constants — address 0x0E
        static constexpr uint8_t LABEL_BIT_OUTPUT                = 0x08;
        static constexpr uint8_t LABEL_BIT_RIGHT_BATT            = 0x04;
        static constexpr uint8_t LABEL_BIT_LOAD                  = 0x02;
        static constexpr uint8_t LABEL_BIT_ERROR                 = 0x40;

        // Label bit constants — address 0x16
        static constexpr uint8_t LABEL_BIT_OVERLOAD              = 0x08;
        static constexpr uint8_t LABEL_BIT_BYPASS                = 0x02;

        // Label bit constants — address 0x1A
        static constexpr uint8_t LABEL_BIT_BATT_SLA              = 0x40;
        static constexpr uint8_t LABEL_BIT_BATT_LI               = 0x80;
        static constexpr uint8_t LABEL_BIT_ECO                   = 0x01;

        // Label bit constants — address 0x18
        static constexpr uint8_t LABEL_BIT_P                     = 0x10;

        // Label bit constants — address 0x1C
        static constexpr uint8_t LABEL_BIT_CHARGING              = 0x80;

        // Symbol bit constants
        static constexpr uint8_t SYMBOL_BIT_CLOCK                = 0x10;
        static constexpr uint8_t SYMBOL_BIT_MAINTENANCE          = 0x20;
        static constexpr uint8_t SYMBOL_BIT_WARNING_TRIANGLE     = 0x80;
        static constexpr uint8_t SYMBOL_BIT_UNDERLINES           = 0x01;
        static constexpr uint8_t SYMBOL_BIT_MUTE                 = 0x04;
        static constexpr uint8_t SYMBOL_BIT_CENTER_FRAME         = 0x01;
        static constexpr uint8_t SYMBOL_BIT_BULB                 = 0x40;
        static constexpr uint8_t SYMBOL_BIT_LINE_DCAC_TO_LOAD     = 0x20;
        static constexpr uint8_t SYMBOL_BIT_LINE_ACDC_TO_DCAC    = 0x02;
        static constexpr uint8_t SYMBOL_BIT_LINE_ACDC_TO_BATT    = 0x04;
        static constexpr uint8_t SYMBOL_BIT_LINE_BATT_TO_DCAC    = 0x08;
        static constexpr uint8_t SYMBOL_BIT_LINE_AC_TO_LOAD      = 0x10;
        static constexpr uint8_t SYMBOL_BIT_DC_AC_SYMBOL         = 0x20;
        static constexpr uint8_t SYMBOL_BIT_LINE_PV_TO_ACDC     = 0x10;
        static constexpr uint8_t SYMBOL_BIT_AC_DC_SYMBOL         = 0x20;
        static constexpr uint8_t SYMBOL_BIT_LINE_WIND_TO_ACDC         = 0x10;
        static constexpr uint8_t SYMBOL_BIT_LINE_AC_TO_ACDC      = 0x20;
        static constexpr uint8_t SYMBOL_BIT_WIND                  = 0x40;
        static constexpr uint8_t SYMBOL_BIT_AC_SYMBOL            = 0x80;

        static constexpr uint8_t RAM_SIZE = 16;
        static constexpr uint8_t DIGITS = 8;

        static constexpr uint8_t DECIMAL_DIGITS[5] = {0, 1, 3, 5, 6};
        static constexpr uint8_t DECIMAL_POINT_BIT = 0x01;
        static constexpr int8_t DECIMAL_COL_OFFSET = -1;

        // Battery level constants
        static constexpr uint8_t MAX_BATTERY_LEVEL = 5;
        static constexpr uint8_t BATTERY_MASK = 0x4F;
        static constexpr uint8_t BATTERY_LEVEL_ADR = 0x1C;

        // Load level constants
        static constexpr uint8_t MAX_LOAD_LEVEL = 5;
        static constexpr uint8_t LOAD_MASK = 0x8F;
        static constexpr uint8_t LOAD_LEVEL_ADR = 0x18;

        // PV level constants
        static constexpr uint8_t MAX_PV_LEVEL = 4;
        static constexpr uint8_t PV_MASK = 0x0F;
        static constexpr uint8_t PV_LEVEL_ADR = 0x1E;

        // Label/Symbol mapping structures
        struct FlagBit {
            uint32_t flag;
            uint8_t bit;
        };

        struct AddressMapping {
            uint8_t address;
            FlagBit bits[9];  // terminated by {0, 0}
        };

        static const AddressMapping* _getUnitMap(size_t &count) {
            static constexpr AddressMapping map[] = {
                {0x00, {
                        {UNIT_LEFT_KILO_PREFIX, UNIT_BIT_LEFT_KILO_PREFIX}
                    }
                },
                {0x06, {
                        {UNIT_LEFT_HZ, UNIT_BIT_LEFT_HZ}
                    }
                },
                {0x08, {
                        {UNIT_LEFT_WATT, UNIT_BIT_LEFT_WATT},
                        {UNIT_LEFT_AMPERE, UNIT_BIT_LEFT_AMPERE},
                        {UNIT_LEFT_VOLT, UNIT_BIT_LEFT_VOLT},
                        {UNIT_LEFT_DEGREE_SLASH, UNIT_BIT_LEFT_DEGREE_SLASH},
                        {UNIT_LEFT_DEGREE, UNIT_BIT_LEFT_DEGREE},
                        {UNIT_LEFT_CELSIUS, UNIT_BIT_LEFT_CELSIUS},
                        {UNIT_CENTER_HOUR, UNIT_BIT_CENTER_HOUR},
                        {UNIT_CENTER_MINUTE, UNIT_BIT_CENTER_MINUTE}
                    }
                },
                {0x0E, {
                        {UNIT_RIGHT_KILO_PREFIX, UNIT_BIT_RIGHT_KILO_PREFIX}
                    }
                },
                {0x14, {
                        {UNIT_RIGHT_HZ, UNIT_BIT_RIGHT_HZ}
                    }
                },
                {0x16, {
                        {UNIT_RIGHT_WATT, UNIT_BIT_RIGHT_WATT},
                        {UNIT_RIGHT_AMPERE, UNIT_BIT_RIGHT_AMPERE},
                        {UNIT_RIGHT_VOLT, UNIT_BIT_RIGHT_VOLT},
                        {UNIT_RIGHT_PERCENT, UNIT_BIT_RIGHT_PERCENT}
                    }
                },
            };
            count = sizeof(map) / sizeof(map[0]);
            return map;
        }

        static const AddressMapping* _getLabelMap(size_t &count) {
            static constexpr AddressMapping map[] = {
                {0x00, {
                        {LABEL_AC, LABEL_BIT_AC},
                        {LABEL_PV, LABEL_BIT_PV},
                        {LABEL_INV, LABEL_BIT_INV},
                        {LABEL_INPUT, LABEL_BIT_INPUT},
                        {LABEL_LEFT_BATT, LABEL_BIT_LEFT_BATT},
                        {LABEL_WP, LABEL_BIT_WP},
                        {LABEL_TEMP, LABEL_BIT_TEMP}
                    }
                },
                {0x0E, {
                        {LABEL_OUTPUT, LABEL_BIT_OUTPUT},
                        {LABEL_RIGHT_BATT, LABEL_BIT_RIGHT_BATT},
                        {LABEL_LOAD, LABEL_BIT_LOAD},
                        {LABEL_ERROR, LABEL_BIT_ERROR}
                    }
                },
                {0x16, {
                        {LABEL_OVERLOAD, LABEL_BIT_OVERLOAD},
                        {LABEL_BYPASS, LABEL_BIT_BYPASS}
                    }
                },
                {0x18, {
                        {LABEL_P, LABEL_BIT_P}
                    }
                },
                {0x1A, {
                        {LABEL_BATT_SLA, LABEL_BIT_BATT_SLA},
                        {LABEL_BATT_LI, LABEL_BIT_BATT_LI},
                        {LABEL_ECO, LABEL_BIT_ECO}
                    }
                },
                {0x1C, {
                        {LABEL_CHARGING, LABEL_BIT_CHARGING}
                    }
                },
            };
            count = sizeof(map) / sizeof(map[0]);
            return map;
        }

        static const AddressMapping* _getSymbolMap(size_t &count) {
            static constexpr AddressMapping map[] = {
                {0x0C, {{SYMBOL_CENTER_FRAME, SYMBOL_BIT_CENTER_FRAME}}},
                {0x0E, {{SYMBOL_CLOCK, SYMBOL_BIT_CLOCK}, {SYMBOL_MAINTENANCE, SYMBOL_BIT_MAINTENANCE}, {SYMBOL_WARNING_TRIANGLE, SYMBOL_BIT_WARNING_TRIANGLE}}},
                {0x16, {{SYMBOL_UNDERLINES, SYMBOL_BIT_UNDERLINES}, {SYMBOL_MUTE, SYMBOL_BIT_MUTE}}},
                {0x18, {{SYMBOL_BULB, SYMBOL_BIT_BULB}, {SYMBOL_LINE_DCAC_TO_LOAD, SYMBOL_BIT_LINE_DCAC_TO_LOAD}}},
                {0x1A, {{SYMBOL_LINE_ACDC_TO_DCAC, SYMBOL_BIT_LINE_ACDC_TO_DCAC}, {SYMBOL_LINE_ACDC_TO_BATT, SYMBOL_BIT_LINE_ACDC_TO_BATT}, {SYMBOL_LINE_BATT_TO_DCAC, SYMBOL_BIT_LINE_BATT_TO_DCAC}, {SYMBOL_LINE_AC_TO_LOAD, SYMBOL_BIT_LINE_AC_TO_LOAD}, {SYMBOL_DC_AC_SYMBOL, SYMBOL_BIT_DC_AC_SYMBOL}}},
                {0x1C, {{SYMBOL_LINE_PV_TO_ACDC, SYMBOL_BIT_LINE_PV_TO_ACDC}, {SYMBOL_AC_DC_SYMBOL, SYMBOL_BIT_AC_DC_SYMBOL}}},
                {0x1E, {{SYMBOL_LINE_WIND_TO_ACDC, SYMBOL_BIT_LINE_WIND_TO_ACDC}, {SYMBOL_LINE_AC_TO_ACDC, SYMBOL_BIT_LINE_AC_TO_ACDC}, {SYMBOL_WIND, SYMBOL_BIT_WIND}, {SYMBOL_AC_SYMBOL, SYMBOL_BIT_AC_SYMBOL}}},
            };
            count = sizeof(map) / sizeof(map[0]);
            return map;
        }

        void _setDecimal(uint8_t row, uint8_t col, bool state) override;

        void _updateUnits(uint32_t units, bool set);
        void _updateLabels(uint32_t labels, bool set);
        void _updateSymbols(uint32_t symbols, bool set);

        uint8_t _mapSegments(uint8_t val);
};
#endif
