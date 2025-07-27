#ifndef SEGLCD_PCx85_RAW_H
#define SEGLCD_PCx85_RAW_H

#include <Wire.h>
#include <SegDriver_PCx85.h>


class SegLCD_PCF85176_TempHumidity : public SegDriver_PCF85176 {
    #define MAX_ADDRESS 31
    
    #define ADDR_SIGNAL_BATT 14
    #define ADDR_TEMP_SEGS 0
    #define ADDR_HUM_SEGS 8

    public:
        typedef enum {
            LABEL_C = 0x01,
            LABEL_PROC = 0x02,
        } LabelFlags;

        SegLCD_PCF85176_TempHumidity(TwoWire& i2c, uint8_t address = DEFAULT_PCF85176_I2C_ADDRESS, uint8_t subaddress = DEFAULT_SUBADDRESS);
        void init() override;
        void clear() override;
        void setBatteryLevel(uint8_t value);
        void setSignalLevel(uint8_t value);
        void setLabels(uint8_t labels);
        void clearLabels(uint8_t labels);
        void setDecimal(uint8_t row, uint8_t col, bool state);
        void setCursor(uint8_t row, uint8_t col) override;
        size_t write(uint8_t ch) override;
    private:
        uint8_t _buffer_sigbatt = 0;

        uint8_t _buffer_temp[4] = {0};
        uint8_t _buffer_hum[3] = {0};
        
        bool _specialMinusDisplayed = false;

        // Display-specific constants
        static constexpr uint8_t TEMP_DIGITS = 4;
        static constexpr uint8_t HUM_DIGITS = 3;
        static constexpr uint8_t TEMP_ROW = 0;
        static constexpr uint8_t HUM_ROW = 1;
        static constexpr uint8_t TEMP_DECIMAL_MIN_COL = 1;
        static constexpr uint8_t TEMP_DECIMAL_MAX_COL = 3;
        static constexpr uint8_t HUM_DECIMAL_COL = 2;

        // TempHum specific bit positions
        static constexpr uint8_t DECIMAL_POINT_BIT = 0x08;
        static constexpr uint8_t LABEL_BIT = 0x08;
        static constexpr uint8_t MINUS_SIGN_BIT = 0x08;

        // Signal/Battery level constants (TempHum specific)
        static constexpr uint8_t MAX_SIGNAL_BATTERY_LEVEL = 4;
        static constexpr uint8_t SIGNAL_BATTERY_MASK_LOW = 0x0F;
        static constexpr uint8_t SIGNAL_BATTERY_MASK_HIGH = 0xF0;

        // Label address offsets
        static constexpr uint8_t TEMP_LABEL_OFFSET = 6;
        static constexpr uint8_t HUM_LABEL_OFFSET = 4;

        // Signal/Battery bit patterns (TempHum specific)
        static constexpr uint8_t SIGNAL_LEVEL_BITS[4] = {0x10, 0x20, 0x40, 0x80};
        static constexpr uint8_t BATTERY_LEVEL_BITS[4] = {8, 4, 2, 1};

        uint8_t _mapSegments(uint8_t val);
};
#endif
