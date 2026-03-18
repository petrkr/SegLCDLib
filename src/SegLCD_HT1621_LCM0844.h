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
            // Address 0x00
            UNIT_LEFT_KILO_PREFIX       = (1UL << 0),   // 0x00:0x01
            UNIT_LEFT_MEGA_PREFIX       = (1UL << 1),   // 0x00:0x02

            // Address 0x06
            UNIT_LEFT_HZ                = (1UL << 2),   // 0x06:0x01

            // Address 0x08
            UNIT_LEFT_WATT              = (1UL << 3),   // 0x08:0x80
            UNIT_LEFT_AMPERE            = (1UL << 4),   // 0x08:0x40
            UNIT_LEFT_VOLT              = (1UL << 5),   // 0x08:0x20
            UNIT_LEFT_PERCENT           = (1UL << 6),   // 0x08:0x10
            UNIT_LEFT_WATT_HOUR_SUFFIX  = (1UL << 7),   // 0x08:0x01
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

            // Address 0x1C
            UNIT_RIGHT_WATT_HOUR_SUFFIX = (1UL << 16),  // 0x1C:0x80

            // Address 0x1E
            UNIT_RIGHT_MEGA_PREFIX      = (1UL << 17),  // 0x1E:0x20
        } UnitFlags;

        typedef enum : uint32_t {
            // Address 0x00
            LABEL_AC                    = (1UL << 0),   // 0x00:0x80
            LABEL_PV1                   = (1UL << 1),   // 0x00:0x20
            LABEL_PV2                   = (1UL << 2),   // 0x00:0x40
            LABEL_INPUT                 = (1UL << 3),   // 0x00:0x08
            LABEL_LEFT_BATT             = (1UL << 4),   // 0x00:0x04

            // Address 0x0E
            LABEL_OUTPUT                = (1UL << 5),   // 0x0E:0x08
            LABEL_RIGHT_BATT            = (1UL << 6),   // 0x0E:0x04
            LABEL_LOAD                  = (1UL << 7),   // 0x0E:0x02
            LABEL_ERROR                 = (1UL << 8),   // 0x0E:0x40
            LABEL_OVERLOAD              = (1UL << 9),   // 0x16:0x08

            // Address 0x1A
            LABEL_BATT_AGM              = (1UL << 10),  // 0x1A:0x20
            LABEL_BATT_FLD              = (1UL << 11),  // 0x1A:0x40
            LABEL_BATT_USER             = (1UL << 12),  // 0x1A:0x80
        } LabelFlags;

        typedef enum : uint32_t {
            SYMBOL_CLOCK                = (1UL << 0),
            SYMBOL_MAINTENANCE          = (1UL << 1),
            SYMBOL_WARNING_TRIANGLE     = (1UL << 2),

            SYMBOL_LEFT_ARROWS          = (1UL << 3),
            SYMBOL_RIGHT_ARROWS         = (1UL << 4),
            SYMBOL_UNDERLINES           = (1UL << 5),
            SYMBOL_MUTE                 = (1UL << 6),

            SYMBOL_CENTER_FRAME         = (1UL << 7),

            SYMBOL_BULB                 = (1UL << 8),
            SYMBOL_JUNCTION             = (1UL << 9),
            SYMBOL_LINE_JUNCTION_TO_LOAD = (1UL << 10),

            SYMBOL_ARROW_JUNCTION_TO_ACDC = (1UL << 11),
            SYMBOL_ARROW_ACDC_TO_JUNCTION = (1UL << 12),
            SYMBOL_ARROW_ACDC_TO_BATT   = (1UL << 13),
            SYMBOL_ARROW_BATT_TO_ACDC   = (1UL << 14),
            SYMBOL_AC_DC_CONVERT        = (1UL << 15),

            SYMBOL_DC_DC_CONVERT        = (1UL << 16),
            SYMBOL_LINE_DCDC_TO_ACDC    = (1UL << 17),

            SYMBOL_AC_LINE              = (1UL << 18),
            SYMBOL_AC_SYMBOL            = (1UL << 19),
            SYMBOL_PV2_SYMBOL           = (1UL << 20),
            SYMBOL_PV1_SYMBOL           = (1UL << 21),
            SYMBOL_LINE_DCDC_TO_BATT    = (1UL << 22),
            SYMBOL_LINE_PV2_TO_DCDC     = (1UL << 23),
            SYMBOL_LINE_PV1_TO_DCDC     = (1UL << 24),
        } SymbolFlags;

        SegLCD_HT1621_LCM0844(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);
        void init() override;
        void setBatteryLevel(uint8_t level);
        void setLoadLevel(uint8_t level);
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
        static constexpr uint8_t UNIT_BIT_LEFT_MEGA_PREFIX       = 0x02;

        // Unit bit constants — address 0x06
        static constexpr uint8_t UNIT_BIT_LEFT_HZ                = 0x01;

        // Unit bit constants — address 0x08
        static constexpr uint8_t UNIT_BIT_LEFT_WATT              = 0x80;
        static constexpr uint8_t UNIT_BIT_LEFT_AMPERE            = 0x40;
        static constexpr uint8_t UNIT_BIT_LEFT_VOLT              = 0x20;
        static constexpr uint8_t UNIT_BIT_LEFT_PERCENT           = 0x10;
        static constexpr uint8_t UNIT_BIT_LEFT_WATT_HOUR_SUFFIX  = 0x01;
        static constexpr uint8_t UNIT_BIT_CENTER_HOUR            = 0x08;
        static constexpr uint8_t UNIT_BIT_CENTER_MINUTE          = 0x04;

        // Unit bit constants — address 0x0E
        static constexpr uint8_t UNIT_BIT_RIGHT_KILO_PREFIX      = 0x01;

        // Unit bit constants — address 0x14
        static constexpr uint8_t UNIT_BIT_RIGHT_HZ               = 0x01;

        // Unit bit constants — address 0x16
        static constexpr uint8_t UNIT_BIT_RIGHT_WATT             = 0x80;
        static constexpr uint8_t UNIT_BIT_RIGHT_AMPERE           = 0x40;
        static constexpr uint8_t UNIT_BIT_RIGHT_VOLT             = 0x20;
        static constexpr uint8_t UNIT_BIT_RIGHT_PERCENT          = 0x10;

        // Unit bit constants — address 0x1C
        static constexpr uint8_t UNIT_BIT_RIGHT_WATT_HOUR_SUFFIX = 0x80;

        // Unit bit constants — address 0x1E
        static constexpr uint8_t UNIT_BIT_RIGHT_MEGA_PREFIX      = 0x20;

        // Label bit constants — address 0x00
        static constexpr uint8_t LABEL_BIT_AC                    = 0x80;
        static constexpr uint8_t LABEL_BIT_PV1                   = 0x20;
        static constexpr uint8_t LABEL_BIT_PV2                   = 0x40;
        static constexpr uint8_t LABEL_BIT_INPUT                 = 0x08;
        static constexpr uint8_t LABEL_BIT_LEFT_BATT             = 0x04;

        // Label bit constants — address 0x0E
        static constexpr uint8_t LABEL_BIT_OUTPUT                = 0x08;
        static constexpr uint8_t LABEL_BIT_RIGHT_BATT            = 0x04;
        static constexpr uint8_t LABEL_BIT_LOAD                  = 0x02;
        static constexpr uint8_t LABEL_BIT_ERROR                 = 0x40;

        // Label bit constants — address 0x16
        static constexpr uint8_t LABEL_BIT_OVERLOAD              = 0x08;

        // Label bit constants — address 0x1A
        static constexpr uint8_t LABEL_BIT_BATT_AGM              = 0x20;
        static constexpr uint8_t LABEL_BIT_BATT_FLD              = 0x40;
        static constexpr uint8_t LABEL_BIT_BATT_USER             = 0x80;

        // Symbol bit constants
        static constexpr uint8_t SYMBOL_BIT_LEFT_ARROWS          = 0x10;
        static constexpr uint8_t SYMBOL_BIT_CENTER_FRAME         = 0x01;
        static constexpr uint8_t SYMBOL_BIT_CLOCK                = 0x10;
        static constexpr uint8_t SYMBOL_BIT_MAINTENANCE          = 0x20;
        static constexpr uint8_t SYMBOL_BIT_WARNING_TRIANGLE     = 0x80;
        static constexpr uint8_t SYMBOL_BIT_RIGHT_ARROWS         = 0x02;
        static constexpr uint8_t SYMBOL_BIT_UNDERLINES           = 0x01;
        static constexpr uint8_t SYMBOL_BIT_MUTE                 = 0x04;
        static constexpr uint8_t SYMBOL_BIT_BULB                 = 0x40;
        static constexpr uint8_t SYMBOL_BIT_JUNCTION              = 0x20;
        static constexpr uint8_t SYMBOL_BIT_LINE_JUNCTION_TO_LOAD = 0x10;
        static constexpr uint8_t SYMBOL_BIT_ARROW_JUNCTION_TO_ACDC = 0x01;
        static constexpr uint8_t SYMBOL_BIT_ARROW_ACDC_TO_JUNCTION = 0x02;
        static constexpr uint8_t SYMBOL_BIT_ARROW_ACDC_TO_BATT  = 0x04;
        static constexpr uint8_t SYMBOL_BIT_ARROW_BATT_TO_ACDC  = 0x08;
        static constexpr uint8_t SYMBOL_BIT_AC_DC_CONVERT        = 0x10;
        static constexpr uint8_t SYMBOL_BIT_DC_DC_CONVERT        = 0x10;
        static constexpr uint8_t SYMBOL_BIT_LINE_DCDC_TO_ACDC    = 0x20;
        static constexpr uint8_t SYMBOL_BIT_AC_LINE              = 0x01;
        static constexpr uint8_t SYMBOL_BIT_AC_SYMBOL            = 0x02;
        static constexpr uint8_t SYMBOL_BIT_PV2_SYMBOL           = 0x04;
        static constexpr uint8_t SYMBOL_BIT_PV1_SYMBOL           = 0x08;
        static constexpr uint8_t SYMBOL_BIT_LINE_DCDC_TO_BATT    = 0x10;
        static constexpr uint8_t SYMBOL_BIT_LINE_PV2_TO_DCDC     = 0x40;
        static constexpr uint8_t SYMBOL_BIT_LINE_PV1_TO_DCDC     = 0x80;

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

        static const AddressMapping* _getUnitMap(size_t &count) {
            static constexpr AddressMapping map[] = {
                {0x00, {
                        {UNIT_LEFT_KILO_PREFIX, UNIT_BIT_LEFT_KILO_PREFIX},
                        {UNIT_LEFT_MEGA_PREFIX, UNIT_BIT_LEFT_MEGA_PREFIX}
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
                        {UNIT_LEFT_PERCENT, UNIT_BIT_LEFT_PERCENT},
                        {UNIT_LEFT_WATT_HOUR_SUFFIX, UNIT_BIT_LEFT_WATT_HOUR_SUFFIX},
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
                {0x1C, {
                        {UNIT_RIGHT_WATT_HOUR_SUFFIX, UNIT_BIT_RIGHT_WATT_HOUR_SUFFIX}
                    }
                },
                {0x1E, {
                        {UNIT_RIGHT_MEGA_PREFIX, UNIT_BIT_RIGHT_MEGA_PREFIX}
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
                        {LABEL_PV1, LABEL_BIT_PV1},
                        {LABEL_PV2, LABEL_BIT_PV2},
                        {LABEL_INPUT, LABEL_BIT_INPUT},
                        {LABEL_LEFT_BATT, LABEL_BIT_LEFT_BATT}
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
                        {LABEL_OVERLOAD, LABEL_BIT_OVERLOAD}
                    }
                },
                {0x1A, {
                        {LABEL_BATT_AGM, LABEL_BIT_BATT_AGM},
                        {LABEL_BATT_FLD, LABEL_BIT_BATT_FLD},
                        {LABEL_BATT_USER, LABEL_BIT_BATT_USER}
                    }
                },
            };
            count = sizeof(map) / sizeof(map[0]);
            return map;
        }

        static const AddressMapping* _getSymbolMap(size_t &count) {
            static constexpr AddressMapping map[] = {
                {0x00, {{SYMBOL_LEFT_ARROWS, SYMBOL_BIT_LEFT_ARROWS}}},
                {0x0C, {{SYMBOL_CENTER_FRAME, SYMBOL_BIT_CENTER_FRAME}}},
                {0x0E, {{SYMBOL_CLOCK, SYMBOL_BIT_CLOCK}, {SYMBOL_MAINTENANCE, SYMBOL_BIT_MAINTENANCE}, {SYMBOL_WARNING_TRIANGLE, SYMBOL_BIT_WARNING_TRIANGLE}}},
                {0x16, {{SYMBOL_RIGHT_ARROWS, SYMBOL_BIT_RIGHT_ARROWS}, {SYMBOL_UNDERLINES, SYMBOL_BIT_UNDERLINES}, {SYMBOL_MUTE, SYMBOL_BIT_MUTE}}},
                {0x18, {{SYMBOL_BULB, SYMBOL_BIT_BULB}, {SYMBOL_JUNCTION, SYMBOL_BIT_JUNCTION}, {SYMBOL_LINE_JUNCTION_TO_LOAD, SYMBOL_BIT_LINE_JUNCTION_TO_LOAD}}},
                {0x1A, {{SYMBOL_ARROW_JUNCTION_TO_ACDC, SYMBOL_BIT_ARROW_JUNCTION_TO_ACDC}, {SYMBOL_ARROW_ACDC_TO_JUNCTION, SYMBOL_BIT_ARROW_ACDC_TO_JUNCTION}, {SYMBOL_ARROW_ACDC_TO_BATT, SYMBOL_BIT_ARROW_ACDC_TO_BATT}, {SYMBOL_ARROW_BATT_TO_ACDC, SYMBOL_BIT_ARROW_BATT_TO_ACDC}, {SYMBOL_AC_DC_CONVERT, SYMBOL_BIT_AC_DC_CONVERT}}},
                {0x1C, {{SYMBOL_DC_DC_CONVERT, SYMBOL_BIT_DC_DC_CONVERT}, {SYMBOL_LINE_DCDC_TO_ACDC, SYMBOL_BIT_LINE_DCDC_TO_ACDC}}},
                {0x1E, {{SYMBOL_AC_LINE, SYMBOL_BIT_AC_LINE}, {SYMBOL_AC_SYMBOL, SYMBOL_BIT_AC_SYMBOL}, {SYMBOL_PV2_SYMBOL, SYMBOL_BIT_PV2_SYMBOL}, {SYMBOL_PV1_SYMBOL, SYMBOL_BIT_PV1_SYMBOL}, {SYMBOL_LINE_DCDC_TO_BATT, SYMBOL_BIT_LINE_DCDC_TO_BATT}, {SYMBOL_LINE_PV2_TO_DCDC, SYMBOL_BIT_LINE_PV2_TO_DCDC}, {SYMBOL_LINE_PV1_TO_DCDC, SYMBOL_BIT_LINE_PV1_TO_DCDC}}},
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
