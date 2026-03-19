#ifndef SEGLCD_HT1621_LCM59011_H
#define SEGLCD_HT1621_LCM59011_H

#include <SegDriver_HT1621.h>

/**
 * @file SegLCD_HT1621_LCM59011.h
 * @brief HT1621 driver for LCM59011 4-digit LCD with wifi/battery/misc symbols.
 */

/**
 * @brief LCM59011 LCD (HT1621).
 *
 * 4-digit 7-segment display with colon, wifi signal, battery, and misc symbols.
 * Integrated HT1621 controller, 3-wire serial protocol.
 *
 * Memory map:
 * - 0x00: Digit 1 (7seg) + 0x80=MCB label
 * - 0x02: Digit 2 (7seg) + 0x80=A unit
 * - 0x04: Digit 3 (7seg) + 0x80=Colon between D2-D3
 * - 0x06: Digit 4 (7seg) + 0x80=M unit
 * - 0x08: wifi1-4, percent, wheel, ECO, UPS
 * - 0x0A: battery outline+1+2, H, GSM SIM, BT
 * - 0x0C: Sun, Thunder, Plug, Warning, bat5+4+3, lines glyph
 */
class SegLCD_HT1621_LCM59011 : public SegDriver_HT1621 {
    public:
        typedef enum : uint8_t {
            UNIT_A                  = (1 << 0),   // 0x02:0x80
            UNIT_M                  = (1 << 1),   // 0x06:0x80
            UNIT_PERCENT            = (1 << 2),   // 0x08:0x10
            UNIT_H                  = (1 << 3),   // 0x0A:0x08
        } UnitFlags;

        typedef enum : uint8_t {
            LABEL_MCB               = (1 << 0),   // 0x00:0x80
        } LabelFlags;

        typedef enum : uint32_t {
            // Address 0x08
            SYMBOL_WHEEL            = (1UL << 0),   // 0x08:0x20
            SYMBOL_ECO              = (1UL << 1),   // 0x08:0x40
            SYMBOL_UPS              = (1UL << 2),   // 0x08:0x80

            // Address 0x0A
            SYMBOL_GSM              = (1UL << 3),   // 0x0A:0x10
            SYMBOL_BLUETOOTH        = (1UL << 4),   // 0x0A:0x20

            // Address 0x0C
            SYMBOL_SUN              = (1UL << 5),   // 0x0C:0x01
            SYMBOL_LIGHTNING        = (1UL << 6),   // 0x0C:0x02
            SYMBOL_PLUG             = (1UL << 7),   // 0x0C:0x04
            SYMBOL_WARNING          = (1UL << 8),   // 0x0C:0x08
            SYMBOL_OUTLINE          = (1UL << 9),   // 0x0C:0x80
        } SymbolFlags;

        SegLCD_HT1621_LCM59011(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);
        void init() override;
        void setBatteryLevel(uint8_t level);
        void setSignalLevel(uint8_t level);
        void setUnits(uint8_t units);
        void clearUnits(uint8_t units);
        void setLabels(uint8_t labels);
        void clearLabels(uint8_t labels);
        void setSymbols(uint32_t symbols);
        void clearSymbols(uint32_t symbols);
        void setWarningSymbol(bool state, uint8_t index = 0);
        size_t write(uint8_t ch) override;

    private:
        static constexpr uint8_t RAM_SIZE = 7;
        static constexpr uint8_t DIGITS = 4;

        // Colon
        static constexpr uint8_t COLON_ADDRESS = 0x04;
        static constexpr uint8_t COLON_BIT = 0x80;
        static constexpr uint8_t COLON_COL = 2;  // Colon between D2 and D3
        static constexpr uint8_t FLAG_COLON_DISPLAYED = 0x01;

        // Battery: spans 0x0A and 0x0C
        static constexpr uint8_t MAX_BATTERY_LEVEL = 6;

        // Signal (wifi bars)
        static constexpr uint8_t MAX_SIGNAL_LEVEL = 4;
        static constexpr uint8_t SIGNAL_ADDRESS = 0x08;
        static constexpr uint8_t SIGNAL_MASK = 0x0F;

        // Unit bit constants
        static constexpr uint8_t UNIT_BIT_A         = 0x80;  // at 0x02
        static constexpr uint8_t UNIT_BIT_M         = 0x80;  // at 0x06
        static constexpr uint8_t UNIT_BIT_PERCENT   = 0x10;  // at 0x08
        static constexpr uint8_t UNIT_BIT_H         = 0x08;  // at 0x0A

        // Label bit constants
        static constexpr uint8_t LABEL_BIT_MCB      = 0x80;  // at 0x00

        // Symbol bit constants — address 0x08
        static constexpr uint8_t SYMBOL_BIT_WHEEL       = 0x20;
        static constexpr uint8_t SYMBOL_BIT_ECO         = 0x40;
        static constexpr uint8_t SYMBOL_BIT_UPS         = 0x80;

        // Symbol bit constants — address 0x0A
        static constexpr uint8_t SYMBOL_BIT_GSM         = 0x10;
        static constexpr uint8_t SYMBOL_BIT_BLUETOOTH   = 0x20;

        // Symbol bit constants — address 0x0C
        static constexpr uint8_t SYMBOL_BIT_SUN         = 0x01;
        static constexpr uint8_t SYMBOL_BIT_LIGHTNING   = 0x02;
        static constexpr uint8_t SYMBOL_BIT_PLUG        = 0x04;
        static constexpr uint8_t SYMBOL_BIT_WARNING     = 0x08;
        static constexpr uint8_t SYMBOL_BIT_OUTLINE     = 0x80;

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
                {0x02, {{UNIT_A, UNIT_BIT_A}}},
                {0x06, {{UNIT_M, UNIT_BIT_M}}},
                {0x08, {{UNIT_PERCENT, UNIT_BIT_PERCENT}}},
                {0x0A, {{UNIT_H, UNIT_BIT_H}}},
            };
            count = sizeof(map) / sizeof(map[0]);
            return map;
        }

        static const AddressMapping* _getSymbolMap(size_t &count) {
            static constexpr AddressMapping map[] = {
                {0x08, {
                    {SYMBOL_WHEEL, SYMBOL_BIT_WHEEL},
                    {SYMBOL_ECO, SYMBOL_BIT_ECO},
                    {SYMBOL_UPS, SYMBOL_BIT_UPS},
                }},
                {0x0A, {
                    {SYMBOL_GSM, SYMBOL_BIT_GSM},
                    {SYMBOL_BLUETOOTH, SYMBOL_BIT_BLUETOOTH},
                }},
                {0x0C, {
                    {SYMBOL_SUN, SYMBOL_BIT_SUN},
                    {SYMBOL_LIGHTNING, SYMBOL_BIT_LIGHTNING},
                    {SYMBOL_PLUG, SYMBOL_BIT_PLUG},
                    {SYMBOL_WARNING, SYMBOL_BIT_WARNING},
                    {SYMBOL_OUTLINE, SYMBOL_BIT_OUTLINE},
                }},
            };
            count = sizeof(map) / sizeof(map[0]);
            return map;
        }

        void _setColon(uint8_t row, uint8_t col, bool state) override;

        void _updateUnits(uint8_t units, bool set);
        void _updateLabels(uint8_t labels, bool set);
        void _updateSymbols(uint32_t symbols, bool set);

        uint8_t _mapSegments(uint8_t val);
};
#endif
