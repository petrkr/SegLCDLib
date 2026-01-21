#ifndef SEGLCD_PCx85_RAW_H
#define SEGLCD_PCx85_RAW_H

#include <Wire.h>
#include <SegDriver_PCx85.h>

/**
 * @brief PCF85176 driver for 6-digit LCD with signal/battery/progress indicators
 *
 * 6-digit 7-segment display with signal strength, battery level, and progress bar.
 * Features: 4-bar signal indicator, 3-level battery, 5-segment progress wheel.
 *
 * @note Separate PCF85176 I2C controller (not integrated)
 * @note I2C protocol (SDA, SCL pins)
 * @note Default I2C address: 0x38
 *
 * @example examples/PCF85176/6DigSigBattProgress/6DigSigBattProgress.ino
 */
class SegLCD_PCF85176_6DigitSignalBatteryProgress : public SegDriver_PCF85176 {

    #define MAX_ADDRESS 31

    #define ADDR_SIGNAL_BATT 0
    #define ADDR_WHEEL 2
    #define ADDR_SMALL_SEGS 6
    #define ADDR_PRES_LABELS 14
    #define ADDR_PROGRESS 16
    #define ADDR_BIG_SEGS 20

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
        void setColon(uint8_t row, uint8_t col, bool state);
        void setDecimal(uint8_t row, uint8_t col, bool state);
        void setCursor(uint8_t row, uint8_t col);
        size_t write(uint8_t ch) override;

    private:
        static constexpr uint8_t RAM_SIZE = 12;
        static constexpr uint8_t OFFSET_SIGBATT = 0;
        static constexpr uint8_t OFFSET_LABELS = 1;
        static constexpr uint8_t OFFSET_TOP = 2;
        static constexpr uint8_t OFFSET_DEFAULT = 6;

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
