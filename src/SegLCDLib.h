#ifndef SEGLCDLIB_H
#define SEGLCDLIB_H

#include <Arduino.h>

typedef enum {
    MODE_DRIVE_STATIC = 1,
    MODE_DRIVE_12 = 2,
    MODE_DRIVE_13 = 3,
    MODE_DRIVE_14 = 0
} ModeDrive;

typedef enum {
    MODE_BIAS_13 = 0,
    MODE_BIAS_12 = 1
} ModeBias;

class SegLCDLib {
    public:
        enum class LCDSections {
            SECTION_DEFAULT,
            SECTION_TOP,
            SECTION_T1,
            SECTION_T2,
            SECTION_CLOCK,
            SECTION_DAY
        };

        virtual void begin() = 0;
        virtual void clear() = 0;
        virtual ~SegLCDLib() {}
        virtual void display() = 0;
        virtual void noDisplay() = 0;
        virtual void writeChar(uint8_t digit, char c, LCDSections section) { };

    protected:
        virtual void _write(uint8_t data, uint8_t address) = 0;
        virtual void _write(uint8_t *data, size_t length, uint8_t address) = 0;

        // Helper functions
        uint8_t _get_char_value(char ch);
        int _countDigits(long num);

};

#endif
