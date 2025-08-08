#ifndef SEGLCD_VT0192_5DigSigBattProgress_H
#define SEGLCD_VT0192_5DigSigBattProgress_H

#include <SegDriver_VK0192.h>


class SegLCD_VK0192_5DigSigBattProgress : public SegDriver_VK0192 {
    public:
        typedef enum {
            LABEL_P = 0x0001,
            LABEL_T = 0x0002,
            LABEL_PROC = 0x0004,
            LABEL_C = 0x0008,
            LABEL_KPA = 0x0010,
            LABEL_MPA = 0x0020,
            LABEL_M = 0x0040,
            LABEL_A = 0x0080,
            LABEL_V = 0x0100,
        } LabelFlags;

        SegLCD_VK0192_5DigSigBattProgress(uint8_t chipselect, uint8_t data, uint8_t write, uint8_t read = -1);
        void init() override;
        void clear() override;
        void setBatteryLevel(uint8_t value);
        void setSignalLevel(uint8_t value);
        void setProgress(uint8_t value);
        void setLabels(uint16_t labels);
        void clearLabels(uint16_t labels);
        void setCursor(uint8_t row, uint8_t col) override;
        void setDecimal(uint8_t row, uint8_t col, bool state);
        size_t write(uint8_t ch) override;

        void writeDigit7seg(uint8_t row, uint8_t col, char c);
        void writeDigit16seg(uint8_t row, uint8_t col, char c);
        
        // TODO: Remove after display implementation
        void writeRam(uint8_t data, uint8_t address = 0);
        void writeRam(uint8_t *data, size_t length, uint8_t address = 0);

    private:
        uint8_t _buffer[48] = {0};  // VK0192 RAM buffer
        
        static constexpr uint8_t DECIMAL_POINT_BIT = 0x10;
        static constexpr uint8_t NUM_7SEG_DIGITS = 10;  // 3+2+5 7-seg digits

        static constexpr uint8_t MAX_SIGNAL_LEVEL = 3;
        static constexpr uint8_t ADDR_SIGNAL_LOW = 0x10;
        static constexpr uint8_t SIGNAL_MASK_LOW = 0xC0;
        static constexpr uint8_t ADDR_SIGNAL_HIGH = 0x16;
        static constexpr uint8_t SIGNAL_MASK_HIGH = 0x80;
        static constexpr uint8_t SIGNAL_LEVEL_SEG[3] = {0x40, 0x80, 0x80};

        static constexpr uint8_t MAX_BATTERY_LEVEL = 4;
        static constexpr uint8_t ADDR_BATT = 0x11;
        static constexpr uint8_t BATTERY_MASK = 0xF0;
        static constexpr uint8_t BATTERY_LEVEL_SEG[4] = {0x10, 0x80, 0x40, 0x20};

        static constexpr uint8_t MAX_PROGRESS = 150;
        static constexpr uint8_t ADDR_PROGRESS_P1 = 0x10;
        static constexpr uint8_t ADDR_PROGRESS_P2 = 0x11;
        static constexpr uint8_t ADDR_PROGRESS_P3 = 0x12;
        static constexpr uint8_t ADDR_PROGRESS_P4 = 0x13;
        static constexpr uint8_t PROGRESS_MASK  = 0x0F;
        static constexpr uint8_t PROGRESS_LEVEL_SEG[16] =
            { 0x01,
              0x08, 0x04, 0x02, 0x01,
              0x01, 0x02, 0x04, 0x08,
              0x08, 0x04, 0x02,
              0x08, 0x04, 0x02, 0x01 };

        static constexpr uint8_t ADDR_LABELS_1 = 0x0E;
        static constexpr uint8_t ADDR_LABELS_2 = 0x0F;
        static constexpr uint8_t ADDR_LABELS_3 = 0x10;

        uint8_t _mapSegments(uint8_t val);
        uint16_t _map16Segments(uint16_t val);
        int8_t _get7SegmentsAddress(uint8_t row, uint8_t col);
        int8_t _get16SegmentsAddress(uint8_t row, uint8_t col);
        void _updateLabels(uint16_t labels, bool set);
};
#endif
