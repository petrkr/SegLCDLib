#ifndef SEGLCD_PCF85176_TEMPHUM_H
#define SEGLCD_PCF85176_TEMPHUM_H

#include <Wire.h>
#include <SegDriver_PCx85.h>

/**
 * @file SegLCD_PCF85176_TempHum.h
 * @brief PCF85176 driver for temperature/humidity LCD.
 */

/**
 * @brief Temperature/humidity LCD with dual rows (PCF85176).
 *
 * Row 0: Temperature, Row 1: Humidity. Features C and % symbols.
 * I2C interface (SA0 pin sets address, A0-A2 are subaddresses).
 *
 * @see examples/PCF85176/TempHumidity/TempHumidity.ino
 */
class SegLCD_PCF85176_TempHumidity : public SegDriver_PCF85176 {
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
        void setCursor(uint8_t row, uint8_t col) override;
        size_t write(uint8_t ch) override;

    private:
        void _setDecimal(uint8_t row, uint8_t col, bool state) override;
        static constexpr uint8_t ADDR_SIGNAL_BATT = 14;
        static constexpr uint8_t ADDR_TEMP_SEGS = 0;
        static constexpr uint8_t ADDR_HUM_SEGS = 8;

        static constexpr uint8_t RAM_SIZE = 8;
        static constexpr uint8_t OFFSET_SIGBATT = 0;
        static constexpr uint8_t OFFSET_TEMP = 1;
        static constexpr uint8_t OFFSET_HUM = 5;

        static constexpr uint8_t FLAG_MINUS_DISPLAYED = 0x01;
        static constexpr uint8_t FLAG_PENDING_DOT = 0x02;

        // Display-specific constants
        static constexpr uint8_t TEMP_DIGITS = 4;
        static constexpr uint8_t HUM_DIGITS = 3;
        static constexpr uint8_t TEMP_ROW = 0;
        static constexpr uint8_t HUM_ROW = 1;

        // Decimal configuration: RAM offset -1 (dot belongs to previous position)
        static constexpr uint8_t DECIMAL_TOP_MIN_COL = 1;
        static constexpr uint8_t DECIMAL_TOP_MAX_COL = 3;
        static constexpr uint8_t DECIMAL_BOTTOM_MIN_COL = 2;
        static constexpr uint8_t DECIMAL_BOTTOM_MAX_COL = 2;  // Only col 2 on bottom row
        static constexpr int8_t DECIMAL_RAM_OFFSET = -1;  // -1: dot belongs to previous position
        // Legacy aliases
        static constexpr uint8_t TEMP_DECIMAL_MIN_COL = DECIMAL_TOP_MIN_COL;
        static constexpr uint8_t TEMP_DECIMAL_MAX_COL = DECIMAL_TOP_MAX_COL;
        static constexpr uint8_t HUM_DECIMAL_COL = DECIMAL_BOTTOM_MIN_COL;

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
