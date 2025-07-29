#ifndef SEGLCD_PCx85_T1T2LCD_H
#define SEGLCD_PCx85_T1T2LCD_H

#include <Wire.h>
#include <SegDriver_PCx85.h>


class SegLCD_PCF85176_T1T2Lcd : public SegDriver_PCF85176 {
    #define MAX_ADDRESS 30

    #define ADDR_SIGNAL_CLOCK 0
    #define ADDR_BATT 2
    #define ADDR_LABELS 4

    #define ADDR_CLOCK_T1T2_LABELS_SEGS 6
    #define ADDR_DAY_SEG 14
    #define ADDR_T1_SEGS 16
    #define ADDR_T2_SEGS 24

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
        void clear() override;
        void setBatteryLevel(uint8_t value);
        void setSignalLevel(uint8_t value);
        void setClockSymbol(bool status);
        void setLabels(uint8_t labels);
        void setT1T2Labels(uint8_t t1t2);
        void clearLabels(uint8_t labels);
        void clearT1T2Labels(uint8_t t1t2);
        void setClockColon(uint8_t row, uint8_t col, bool state);
        void setDecimal(uint8_t row, uint8_t col, bool state);
        void setCursor(uint8_t row, uint8_t col) override;
        size_t write(uint8_t ch) override;

    private:
        uint8_t _buffer_sigclk = 0;
        uint8_t _buffer_batt = 0;

        // Clock has T1:, T2: labels in the first 2 segments
        uint8_t _buffer_clock[4] = {0};

        // Celsius / Humidity after T1 / T2
        uint8_t _buffer_labels = 0;

        uint8_t _bufferT1[4] = {0};
        uint8_t _bufferT2[4] = {0};

        bool _colon_clock = false;

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
