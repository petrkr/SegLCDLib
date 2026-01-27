#ifndef SEGLCD_PCx85_T1T2LCD_H
#define SEGLCD_PCx85_T1T2LCD_H

#include <Wire.h>
#include <SegDriver_PCx85.h>

/**
 * @file SegLCD_PCF85176_T1T2Lcd.h
 * @brief PCF85176 driver for T1T2 multi-zone LCD.
 */

/**
 * @brief Multi-zone T1/T2 LCD with clock display (PCF85176).
 *
 * 3-row display: clock, T1 zone, T2 zone. Signal/battery indicators.
 * I2C interface (SA0 pin sets address, A0-A2 are subaddresses).
 *
 * @see examples/PCF85176/T1T2Lcd/T1T2Lcd.ino
 */
class SegLCD_PCF85176_T1T2Lcd : public SegDriver_PCF85176 {
    public:
        typedef enum {
            LABEL_T1_C_UP = 0x80,
            LABEL_T1_C_DOWN = 0x40,
            LABEL_T2_C = 0x20,
            LABEL_T2_HUM = 0x10,
        } LabelFlags;

        typedef enum {
            LABEL_T1 = 0x01,
            LABEL_T2 = 0x02
        } LabelT1T2Enum;

        SegLCD_PCF85176_T1T2Lcd(TwoWire& i2c, uint8_t address = DEFAULT_PCF85176_I2C_ADDRESS, uint8_t subaddress = DEFAULT_SUBADDRESS);
        void init() override;
        void setBatteryLevel(uint8_t value);
        void setSignalLevel(uint8_t value);
        void setClockSymbol(bool status);
        void setLabels(uint8_t labels);
        void setT1T2Labels(uint8_t t1t2);
        void clearLabels(uint8_t labels);
        void clearT1T2Labels(uint8_t t1t2);
        void setCursor(uint8_t row, uint8_t col) override;
        size_t write(uint8_t ch) override;

    private:
        void _setColon(uint8_t row, uint8_t col, bool state) override;
        void _setDecimal(uint8_t row, uint8_t col, bool state) override;
        static constexpr uint8_t ADDR_SIGNAL_CLOCK = 0;
        static constexpr uint8_t ADDR_BATT = 2;
        static constexpr uint8_t ADDR_LABELS = 4;
        static constexpr uint8_t ADDR_CLOCK_T1T2_LABELS_SEGS = 6;
        static constexpr uint8_t ADDR_DAY_SEG = 14;
        static constexpr uint8_t ADDR_T1_SEGS = 16;
        static constexpr uint8_t ADDR_T2_SEGS = 24;

        static constexpr uint8_t RAM_SIZE = 16;

        // RAM layout (matches _ramBuffer allocation):
        static constexpr uint8_t OFFSET_SIGCLK = 0;
        static constexpr uint8_t OFFSET_BATT = 1;   // ADDR_BATT / 2 = 1
        static constexpr uint8_t OFFSET_LABELS = 2; // ADDR_LABELS / 2 = 2
        static constexpr uint8_t OFFSET_CLOCK = 3;  // ADDR_CLOCK_T1T2_LABELS_SEGS / 2 = 3
        static constexpr uint8_t OFFSET_DAY = 7;    // ADDR_DAY_SEG / 2 = 7
        static constexpr uint8_t OFFSET_T1 = 8;     // ADDR_T1_SEGS / 2 = 8
        static constexpr uint8_t OFFSET_T2 = 12;    // ADDR_T2_SEGS / 2 = 12

        static constexpr uint8_t FLAG_COLON_CLOCK = 0x01;

        static constexpr uint8_t ROW_CLOCK = 0;
        static constexpr uint8_t ROW_T1 = 1;
        static constexpr uint8_t ROW_T2 = 2;

        static constexpr uint8_t DECIMAL_POINT_BIT = 0x01;
        static constexpr uint8_t DECIMAL_MIN_ROW = 1;
        static constexpr uint8_t DECIMAL_MAX_ROW = 2;
        static constexpr uint8_t DECIMAL_MIN_COL = 0;
        static constexpr uint8_t DECIMAL_MAX_COL = 3;

        // Symbols constants
        static constexpr uint8_t SYMBOL_CLOCK_MASK = 0x04;
        static constexpr uint8_t SYMBOL_T1T2_MASK = 0x01;

        // Signal/Battery level constants
        static constexpr uint8_t MAX_BATTERY_LEVEL = 4;
        static constexpr uint8_t BATTERY_MASK = 0xF0;
        static constexpr uint8_t BATTERY_LEVEL_SEG[MAX_BATTERY_LEVEL] = {0x10, 0x80, 0x40, 0x20};

        static constexpr uint8_t MAX_SIGNAL_LEVEL = 5;
        static constexpr uint8_t SIGNAL_MASK = 0xF8;
        static constexpr uint8_t SIGNAL_LEVEL_BITS[MAX_SIGNAL_LEVEL] = {0x80, 0x40, 0x20, 0x10, 0x08};

        uint8_t _mapSegments(uint8_t val);
};
#endif
