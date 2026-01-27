#ifndef SEGLCD_PCx85_RAW_H
#define SEGLCD_PCx85_RAW_H

#include <Wire.h>
#include <SegDriver_PCx85.h>

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
            LABEL_MPA = 0x01,
            LABEL_KPA = 0x02,
            LABEL_BAR = 0x04,
            LABEL_PSI = 0x08,
            LABEL_MH2O = 0x10,
            LABEL_MMHG = 0x20,
            LABEL_ATM = 0x40,
            LABEL_KGFCM2 = 0x80,
            LABEL_ALL = 0xFF
        } LabelFlags;

        SegLCD_PCF85176_6DigitSignalBatteryProgress(TwoWire& i2c, uint8_t address = DEFAULT_PCF85176_I2C_ADDRESS, uint8_t subaddress = DEFAULT_SUBADDRESS);
        void init() override;
        void setBatteryLevel(uint8_t value);
        void setSignalLevel(uint8_t value);
        void setProgress(uint8_t value);
        void setWheel(uint16_t value);
        void setLabels(LabelFlags labels);
        void clearLabels(LabelFlags labels);
        void setCursor(uint8_t row, uint8_t col);
        size_t write(uint8_t ch) override;

    private:
        void _setColon(uint8_t row, uint8_t col, bool state) override;
        void _setDecimal(uint8_t row, uint8_t col, bool state) override;
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

        // Decimal configuration: RAM offset -1 (dot belongs to previous position)
        static constexpr uint8_t DECIMAL_POINT_BIT = 0x10;
        static constexpr int8_t DECIMAL_RAM_OFFSET = -1;  // -1: dot belongs to previous position
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
