#ifndef LCD_PLUGIN_H
#define LCD_PLUGIN_H

#include <Arduino.h>
#include "SegLCDLib.h"

// Configuration structure - same as UniversalMenuControl
enum BusType : uint8_t { BUS_I2C = 0, BUS_3WIRE = 1 };

struct DisplayConfig {
    uint8_t version = 1;
    uint8_t displayId = 0;
    uint8_t busType = BUS_I2C;
    int8_t sda = -1, scl = -1;
    int8_t cs = -1, wr = -1, data = -1;
    int8_t pwr = -1, backlight = -1;
    uint8_t i2cAddr = 0x38, subAddr = 0x00;
};

// Helper functions
static long parseNumber(const char *s) {
    if (!s) return 0;
    return strtol(s, nullptr, 0);
}

static const size_t MENU_INNER_WIDTH = 38;

static inline void printMenuLine(Stream &out, const char *text) {
    size_t len = strlen(text);
    if (len > MENU_INNER_WIDTH) len = MENU_INNER_WIDTH;
    out.print("│ ");
    for (size_t i = 0; i < len; ++i) out.print(text[i]);
    for (size_t i = len; i < MENU_INNER_WIDTH; ++i) out.print(' ');
    out.println(" │");
}

static inline void printMenuTop(Stream &out, const char *title) {
    size_t title_len = strlen(title);
    size_t fixed = title_len + 3; // "─ " + title + " "
    size_t dashes = (MENU_INNER_WIDTH + 2 > fixed) ? (MENU_INNER_WIDTH + 2 - fixed) : 0;
    out.print("┌─ ");
    out.print(title);
    out.print(' ');
    for (size_t i = 0; i < dashes; ++i) out.print("─");
    out.println("┐");
}

static inline void printMenuSeparator(Stream &out, const char *title) {
    size_t title_len = strlen(title);
    size_t fixed = title_len + 3; // "─ " + title + " "
    size_t dashes = (MENU_INNER_WIDTH + 2 > fixed) ? (MENU_INNER_WIDTH + 2 - fixed) : 0;
    out.print("├─ ");
    out.print(title);
    out.print(' ');
    for (size_t i = 0; i < dashes; ++i) out.print("─");
    out.println("┤");
}

static inline void printMenuBottom(Stream &out) {
    out.print("└");
    for (size_t i = 0; i < MENU_INNER_WIDTH + 2; ++i) out.print("─");
    out.println("┘");
}

static bool parseBool(const char *s) {
    if (!s) return false;
    if (strcmp(s, "1") == 0) return true;
    if (strcasecmp(s, "on") == 0) return true;
    if (strcasecmp(s, "true") == 0) return true;
    return false;
}

static char *nextToken(char **args) {
    if (!args || !*args) return nullptr;
    char *s = *args;
    while (*s == ' ' || *s == '\t') s++;
    if (*s == '\0') { *args = s; return nullptr; }
    char *tok = s;
    while (*s && *s != ' ' && *s != '\t') s++;
    if (*s) { *s = '\0'; s++; }
    *args = s;
    return tok;
}

static void dumpBuffer(const SegLCDLib &lcd, Stream &out) {
    const uint8_t *ram = lcd.getRamBuffer();
    size_t ramSize = lcd.getRamBufferSize();
    if (!ram || ramSize == 0) {
        out.println("No buffer available.");
        return;
    }

    // Hexdump
    out.println("HEX:");
    for (size_t i = 0; i < ramSize; i += 16) {
        char off[5];
        snprintf(off, sizeof(off), "%04X", (unsigned)i);
        out.print(off); out.print(": ");
        for (size_t j = 0; j < 16; j++) {
            if (i + j < ramSize) {
                uint8_t v = ram[i + j];
                if (v < 0x10) out.print('0');
                out.print(v, HEX); out.print(' ');
                if (j == 7) out.print(' ');
            } else {
                out.print("   ");
                if (j == 7) out.print(' ');
            }
        }
        out.print(" |");
        for (size_t j = 0; j < 16; j++) {
            if (i + j < ramSize) {
                uint8_t v = ram[i + j];
                out.print((v >= 32 && v <= 126) ? (char)v : '.');
            } else out.print(' ');
        }
        out.println('|');
    }

    // COM bits
    out.println("\nCOM:");
    const uint8_t perRow = 16;
    for (size_t base = 0; base < ramSize; base += perRow) {
        out.print("addr:");
        for (size_t i = base; i < base + perRow && i < ramSize; i++) {
            uint8_t addr = (uint8_t)(i * 2);
            out.print(' ');
            if (addr < 0x10) out.print('0');
            out.print(addr, HEX);
            if (((i - base + 1) % 4) == 0) out.print(' ');
        }
        out.println();
        for (int8_t b = 0; b <= 7; b++) {
            out.print('C'); out.print((char)('0' + b)); out.print("  :");
            for (size_t i = base; i < base + perRow && i < ramSize; i++) {
                out.print(' '); out.print((ram[i] >> b) & 0x01); out.print(' ');
                if (((i - base + 1) % 4) == 0) out.print(' ');
            }
            out.println();
        }
        out.println();
    }
}

// Plugin interface
class LCDPlugin {
public:
    virtual const char *name() const = 0;

    // Create LCD instance - returns nullptr on error
    virtual SegLCDLib* create(const DisplayConfig &cfg) = 0;
    virtual void destroy(SegLCDLib *lcd) = 0;

    // Handle LCD-specific commands
    // Returns true if command was handled, false otherwise
    virtual bool handleCommand(SegLCDLib *lcd, const char *cmd, char *args, Stream &out) = 0;
    virtual void printMenu(Stream &out) = 0;

    // Capabilities
    virtual bool hasDecimal() const { return false; }
    virtual bool hasColon() const { return false; }

    virtual ~LCDPlugin() {}
};

#endif
