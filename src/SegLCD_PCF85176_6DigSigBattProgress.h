#ifndef SEGLCD_PCF85176_6DIGSIGBATTPROGRESS_H
#define SEGLCD_PCF85176_6DIGSIGBATTPROGRESS_H


#if !defined(SEGLCD_DISABLE_ALL_LCDS) || defined(SEGLCD_ENABLE_PCF85176_6DIGSIGBATTPROGRESS)
#include <SegDriver_PCF85176.h>

/**
 * @file SegLCD_PCF85176_6DigSigBattProgress.h
 * @brief PCF85176 driver for 6-digit LCD with signal/battery/progress.
 */

/**
 * @brief 6-digit LCD with signal/battery/progress indicators (PCF85176).
 *
 * Features: 4-bar signal, 3-level battery, progress wheel, unit labels.
 * I2C interface (SA0 pin sets address, A0-A2 are subaddresses).
 *
 * @see examples/PCF85176/6DigSigBattProgress/6DigSigBattProgress.ino
 */
class SegLCD_PCF85176_6DigitSignalBatteryProgress : public SegDriver_PCF85176 {
    public:
        typedef enum {
            UNIT_MPA = 0x01,
            UNIT_KPA = 0x02,
            UNIT_BAR = 0x04,
            UNIT_PSI = 0x08,
            UNIT_MH2O = 0x10,
            UNIT_MMHG = 0x20,
            UNIT_ATM = 0x40,
            UNIT_KGFCM2 = 0x80,
            UNIT_ALL = 0xFF
        } UnitFlags;

        SegLCD_PCF85176_6DigitSignalBatteryProgress(SegTransportI2C& transport, uint8_t address = DEFAULT_PCX85_I2C_ADDRESS, uint8_t subaddress = DEFAULT_SUBADDRESS);
        void init() override;
        void setBatteryLevel(uint8_t value);
        void setSignalLevel(uint8_t value);
        void setProgress(uint8_t value);
        void setWheel(uint16_t value);
        void setUnits(UnitFlags units);
        void clearUnits(UnitFlags units);
        void setCursor(uint8_t row, uint8_t col) override;
        size_t write(uint8_t ch) override;

    protected:
        void _setColon(uint8_t row, uint8_t col, bool state) override;
        void _setDecimal(uint8_t row, uint8_t col, bool state) override;

    private:
        static constexpr uint8_t ADDR_SIGNAL_BATT = 0;
        static constexpr uint8_t ADDR_WHEEL = 2;
        static constexpr uint8_t ADDR_SMALL_SEGS = 6;
        static constexpr uint8_t ADDR_PRES_LABELS = 14;
        static constexpr uint8_t ADDR_PROGRESS = 16;
        static constexpr uint8_t ADDR_BIG_SEGS = 20;

        static constexpr uint8_t RAM_SIZE = 16;
        static constexpr uint8_t OFFSET_SIGBATT = 0;     // HW 0
        static constexpr uint8_t OFFSET_WHEEL = 1;       // HW 2
        static constexpr uint8_t OFFSET_TOP = 3;         // HW 6
        static constexpr uint8_t OFFSET_LABELS = 7;      // HW 14
        static constexpr uint8_t OFFSET_PROGRESS = 8;    // HW 16
        static constexpr uint8_t OFFSET_DEFAULT = 10;    // HW 20

        static constexpr uint8_t FLAG_COLON_TOP = 0x01;
        static constexpr uint8_t FLAG_COLON_DEFAULT = 0x02;

        // Decimal configuration: column offset -1 (dot belongs to previous position)
        static constexpr uint8_t DECIMAL_POINT_BIT = 0x10;
        static constexpr int8_t DECIMAL_COL_OFFSET = -1;  // -1: dot belongs to previous position
        static constexpr uint8_t DECIMAL_TOP_MIN_COL = 0;
        static constexpr uint8_t DECIMAL_TOP_MAX_COL = 2;
        static constexpr uint8_t DECIMAL_BOTTOM_MIN_COL = 0;
        static constexpr uint8_t DECIMAL_BOTTOM_MAX_COL = 4;

        // Colon configuration
        static constexpr uint8_t COLON_TOP_COL = 2;
        static constexpr uint8_t COLON_BOTTOM_COL = 4;

        uint8_t _mapSegments(uint8_t val);
};
#endif

#endif
