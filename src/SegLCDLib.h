#ifndef SEGLCDLIB_H
#define SEGLCDLIB_H

#include <Arduino.h>
#include "Print.h"

/**
 * @file SegLCDLib.h
 * @brief Base class for segment LCD display drivers.
 */

/**
 * @brief Drive mode used by LCD controller.
 */
typedef enum {
    MODE_DRIVE_STATIC = 1, ///< Static drive (1/1 duty)
    MODE_DRIVE_12 = 2,     ///< 1/2 multiplexing
    MODE_DRIVE_13 = 3,     ///< 1/3 multiplexing
    MODE_DRIVE_14 = 0      ///< 1/4 multiplexing
} ModeDrive;

/**
 * @brief Bias mode used by LCD controller. Not care if drive mode is static
 */
typedef enum {
    MODE_BIAS_13 = 0, ///< 1/3 bias
    MODE_BIAS_12 = 1  ///< 1/2 bias
} ModeBias;

/**
 * @brief Abstract base class for LCD segment display drivers.
 *
 * This class defines the generic interface required by all LCD segment drivers,
 * including display control, character output, and data writing methods.
 */
class SegLCDLib : public Print {
    public:
        /**
         * @brief Backlight mode for GPIO control.
         */
        enum BacklightMode {
            BACKLIGHT_DIGITAL,  ///< Digital on/off mode
            BACKLIGHT_PWM       ///< PWM brightness control (0-255)
        };

        /**
         * @brief Virtual destructor to ensure proper cleanup in derived classes.
         */
        virtual ~SegLCDLib();

        /**
         * @brief Logical display sections that can be targeted by higher-level rendering logic.
         */

        // ---------------------------
        // LCD API 1.0 Mandatory part
        // ---------------------------
        // Spec: https://playground.arduino.cc/Code/LCDAPI/

        /**
         * @brief Initialize the display driver, clear display and set position to 0,0.
         */
        virtual void init() = 0;

        /**
         * @brief Clear all visible segments on the display.
         */
        virtual void clear();

        /**
         * @brief Set cursor to 0, 0 without clear display
         */
        void home();

        /**
         * @brief Set cursor on exact digit.
         *
         * Because LCD API is mainly used for character displays in this segment displays this function will be littlebit
         * confusing. Because most of Segment LCD has only one row, thus this will be like row 0 and column will represent
         * actual digit.
         *
         * But if there is LCD like T1T2 LCD. Row 0 will be clock part, Row 1 will be T1: part and Row 2 will be T2: part of LCD
         *
         * @param row Row, where 0-MAXROWS
         * @param col Column 0-MAXDIGITS in current row
         */
        virtual void setCursor(uint8_t row, uint8_t col);

        virtual size_t write(uint8_t ch) { };

        /**
         * @brief Send RAW command to controller
         *
         * @param command Raw command byte
         */
        virtual void command(uint8_t command) = 0;
        using Print::write;

        // --------------------------
        // LCD API 1.0 Optional part
        // --------------------------
        /**
         * @brief Turn the display on.
         */
        virtual void on() = 0;

        /**
         * @brief Turn the display off.
         */
        virtual void off() = 0;

        /**
         * @brief Initialize GPIO backlight control.
         *
         * Must be called from init() of concrete implementations to setup the backlight GPIO.
         *
         * @param backlightPin GPIO pin for backlight control (-1 to disable)
         * @param backlightMode Backlight mode (DIGITAL or PWM)
         * @param backlightActiveHigh true if backlight is active high (default), false for active low
         */
        void initBacklight(int8_t backlightPin, BacklightMode backlightMode = BACKLIGHT_DIGITAL, bool backlightActiveHigh = true);

        /**
         * @brief Set backlight state (on/off).
         *
         * @param state true to turn backlight on, false to turn off
         */
        virtual void setBacklight(bool state);

        /**
         * @brief Set backlight brightness (0-255).
         *
         * @param brightness Brightness level 0-255, where 0 is off and 255 is full brightness
         */
        virtual void setBacklight(int brightness);

        // ----------------------
        // Flush/Autoflush API
        // ----------------------

        /**
         * @brief Enable or disable autoflush mode.
         *
         * In autoflush mode (default), each write() call immediately updates the display.
         * This maintains LCD API 1.0 compatibility but can cause flicker when multiple
         * segments are updated rapidly (e.g., writing "123.4" causes decimal point to flicker).
         *
         * In manual flush mode, write() only updates the internal buffer. Call flush()
         * to write all buffered changes to the display at once, eliminating flicker.
         *
         * Example usage:
         * \code{.cpp}
         * // Disable autoflush for rapid updates
         * lcd.setAutoFlush(false);
         *
         * void loop() {
         *     lcd.setCursor(0, 0);
         *     lcd.print(value);  // Buffer only, no HW write
         *     lcd.flush();       // Single HW write of all changes
         *     delay(100);
         * }
         * \endcode
         *
         * @param enable true to enable autoflush (default), false for manual flush mode
         * @see flush()
         */
        void setAutoFlush(bool enable);

        /**
         * @brief Get current autoflush state.
         *
         * @return true if autoflush is enabled, false if in manual flush mode
         */
        bool getAutoFlush() const;

        /**
         * @brief Flush all buffered changes to the display.
         *
         * Writes the entire display buffer to the hardware. This is useful when
         * autoflush is disabled to batch multiple write() calls into a single
         * hardware update cycle.
         *
         * Safe to call even when autoflush is enabled (no effect).
         *
         * @see setAutoFlush()
         */
        virtual void flush();

        /**
         * @brief Flush specific range of buffered changes to the display.
         *
         * Writes a specific address range from the display buffer to hardware.
         * This is an advanced API for optimizing updates when only part of the
         * display needs to be refreshed.
         *
         * For example, to update only the first row of a multi-row display:
         * \code{.cpp}
         * lcd.flush(0, 6);  // Update first 6 bytes (row 0)
         * \endcode
         *
         * Safe to call even when autoflush is enabled (no effect).
         *
         * @param startAddr Starting buffer address in bytes (RAM buffer index)
         * @param length Number of bytes to flush
         * @see setAutoFlush()
         */
        virtual void flush(uint8_t startAddr, uint8_t length);

        /**
         * @brief Get pointer to internal RAM buffer (read-only).
         *
         * @return Pointer to RAM buffer or nullptr if not allocated.
         */
        const uint8_t* getRamBuffer() const;

        /**
         * @brief Get size of internal RAM buffer in bytes.
         *
         * @return Buffer size in bytes (0 if not allocated).
         */
        size_t getRamBufferSize() const;

        // ----------------------
        // SegLCDLib specific part
        // ----------------------

    protected:
        /**
         * @brief Autoflush mode enabled flag (default: true for LCD API 1.0 compatibility).
         */
        bool _autoFlush = true;

        /**
         * @brief Backlight GPIO pin (-1 if disabled)
         */
        int8_t _backlightPin = -1;

        /**
         * @brief Backlight mode (DIGITAL or PWM)
         */
        BacklightMode _backlightMode = BACKLIGHT_DIGITAL;

        /**
         * @brief Backlight active high flag
         */
        bool _backlightActiveHigh = true;

        /**
         * @brief Cached backlight brightness
         */
        uint8_t _backlightBrightness = 0;

        /**
         * @brief LEDC channel for old ESP32 Arduino Core (2.0.x)
         */
        #if defined(ESP32) && ESP_IDF_VERSION_MAJOR < 5
        uint8_t _backlightChannel = 0;
        #endif

        /**
         * @brief Generic flag storage for display-specific state tracking.
         *
         * Individual LCD implementations can use these bits to track state like:
         * - Colon positions active
         * - Previous dot written
         * - Special characters displayed
         *
         * Usage: Define bit constants in derived class, use helper methods to set/clear/check.
         */
        uint8_t _displayFlags = 0;

        /**
         * @brief Common flag for decimal handling when RAM offset is 0.
         *
         * Used when decimal point shares the same RAM byte as the digit.
         * Set when '.' is written, cleared after next character preserves it.
         */
        static constexpr uint8_t FLAG_PENDING_DOT = 0x80;

        /**
         * @brief Set a specific flag bit.
         * @param mask Bitmask with bit(s) to set
         */
        void _setFlag(uint8_t mask);

        /**
         * @brief Clear a specific flag bit.
         * @param mask Bitmask with bit(s) to clear
         */
        void _clearFlag(uint8_t mask);

        /**
         * @brief Check if a specific flag bit is set.
         * @param mask Bitmask with bit(s) to check
         * @return true if all bits in mask are set
         */
        bool _isFlagSet(uint8_t mask) const;

        /**
         * @brief Clear all flags.
         */
        void _clearAllFlags();

        /**
         * @brief Write decimal point.
         *
         * Internal helper for display-specific write() implementations.
         *
         * @param ch Character to process
         * @param minCol Minimum valid column for decimal
         * @param maxCol Maximum valid column for decimal
         * @param ramOffset RAM offset for decimal (-1, 0, or +1)
         * @return true if character '.' was processed
         */
        bool _dotWrite(uint8_t ch, int8_t minCol, int8_t maxCol, int8_t ramOffset);

        /**
         * @brief Clear current decimal (overwrite).
         *
         * Internal helper for display-specific write() implementations.
         *
         * @param minCol Minimum valid column
         * @param maxCol Maximum valid column
         */
        void _dotClearCur(int8_t minCol, int8_t maxCol);

        /**
         * @brief Clear colon if its flag is not set.
         *
         * Internal helper for display-specific write() implementations.
         *
         * @param ch Character being processed
         * @param colonCol Colon position
         * @param colonFlag Flag bit for this colon
         */
        void _colonClearIfNotFlagged(uint8_t ch, uint8_t colonCol, uint8_t colonFlag);

        /**
         * @brief Virtual method for setting decimal point.
         *
         * Each LCD has its own implementation with HW specifics.
         * Default implementation is empty (no-op).
         */
        virtual void _setDecimal(uint8_t row, uint8_t col, bool state) { }

        /**
         * @brief Virtual method for setting colon.
         *
         * Default implementation is empty (no-op).
         */
        virtual void _setColon(uint8_t row, uint8_t col, bool state) { }

        /**
         * @brief Dynamic RAM buffer for display data (allocated by derived classes).
         */
        uint8_t* _ramBuffer = nullptr;

        /**
         * @brief Size of allocated RAM buffer in bytes.
         */
        size_t _ramBufferSize = 0;

        /**
         * @brief Allocate RAM buffer for display data.
         *
         * @param size Buffer size in bytes
         */
        void _allocateBuffer(size_t size);

        /**
         * @brief Write to display RAM with nibble-aware masking support.
         *
         * Handles both byte-aligned and nibble-aligned addresses with read-modify-write
         * when mask is not 0xFF. Updates internal buffer and writes to hardware.
         *
         * @param data Value to write
         * @param address RAM address (nibble-level addressing)
         * @param mask Bitmask for selective bit updates (0xFF = write all bits)
         */
        void _writeRamMasked(uint8_t data, uint8_t address, uint8_t mask = 0xFF);

        /**
         * @brief Low-level method to write a single byte to display RAM.
         *
         * @param data Data byte
         * @param address RAM address
         */
        virtual void _writeRam(uint8_t data, uint8_t address) = 0;

        /**
         * @brief Low-level method to write a data buffer to display RAM.
         *
         * @param data Pointer to data buffer
         * @param length Length of data buffer
         * @param address Start RAM address
         */
        virtual void _writeRam(uint8_t *data, size_t length, uint8_t address) = 0;

        /**
         * @brief Helper method to convert a character to its segment representation.
         */
        uint8_t _get_char_value(char ch);

        /**
         * @brief Helper method to convert a character to its 16-segment representation.
         */
        uint16_t _get_16char_value(char ch);


        /**
         * @brief Current Row
         */
        uint8_t _cursorRow = 0;

        /**
         * @brief Current Column
         */
        uint8_t _cursorCol = 0;
};

#endif
