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

        SegLCD_PCF85176_T1T2Lcd(TwoWire& i2c, uint8_t address = 56, uint8_t subaddress = 0);
        void begin() override;
        void setBatteryLevel(uint8_t value);
        void setSignalLevel(uint8_t value);
        void setClockSymbol(bool status);
        void setClockColon(bool status);
        void setLabels(uint8_t labels);
        void setT1T2Labels(uint8_t t1t2);
        void clearLabels(uint8_t labels);
        void clearT1T2Labels(uint8_t t1t2);
        void writeChar(uint8_t digit, char c, LCDSections section = LCDSections::SECTION_DEFAULT) override;

        ///TODO: Remove after implementation
        void write(uint8_t data, uint8_t address = 0);

    private:
        uint8_t _buffer_sigclk = 0;
        uint8_t _buffer_batt = 0;

        // Clock has T1:, T2: labels in the first 2 segments
        uint8_t _buffer_clock[4] = {0};

        // Celsius / Humidity after T1 / T2
        uint8_t _buffer_labels = 0;

        uint8_t _get_char_value(char ch);
        uint8_t _mapSegments(uint8_t val);
};
#endif
