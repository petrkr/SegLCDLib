#ifndef HT1622_10DIG16SEG_PLUGIN_H
#define HT1622_10DIG16SEG_PLUGIN_H

#include "LCDPlugin.h"
#include "SegLCD_HT1622_10Dig16Seg.h"

class HT1622_10Dig16SegPlugin : public LCDPlugin {
public:
    const char *name() const override { return "16seg"; }

    SegLCDLib* create(const DisplayConfig &cfg) override {
        if (cfg.cs < 0 || cfg.wr < 0 || cfg.data < 0) {
            Serial.println("Error: CS/WR/DATA pins required");
            return nullptr;
        }

        auto *lcd = new SegLCD_HT1622_10Dig16Seg(cfg.cs, cfg.data, cfg.wr);
        if (cfg.pwr >= 0) {
            lcd->initBacklight(cfg.pwr);
            lcd->setBacklight(true);
        }
        lcd->init();
        lcd->setAutoFlush(true);
        lcd->clear();
        return lcd;
    }

    void destroy(SegLCDLib *lcd) override {
        delete static_cast<SegLCD_HT1622_10Dig16Seg*>(lcd);
    }

    bool handleCommand(SegLCDLib *lcdBase, const char *cmd, char *args, Stream &out) override {
        (void)out;
        auto *lcd = static_cast<SegLCD_HT1622_10Dig16Seg*>(lcdBase);

        if (strcmp(cmd, "w16") == 0) {
            uint8_t r = (uint8_t)parseNumber(nextToken(&args));
            uint8_t c = (uint8_t)parseNumber(nextToken(&args));
            const char *ch = nextToken(&args);
            if (ch && ch[0]) {
                lcd->writeDigit16seg(r, c, ch[0]);
            }
            return true;
        }
        if (strcmp(cmd, "d") == 0) {
            lcd->setDecimal((uint8_t)parseNumber(nextToken(&args)),
                           (uint8_t)parseNumber(nextToken(&args)),
                           parseBool(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "bl") == 0) {
            lcd->setBacklight(parseBool(nextToken(&args)));
            return true;
        }

        return false;
    }

    void printMenu(Stream &out) override {
        printMenuLine(out, "16seg commands:");
        printMenuLine(out, "  w16 <r> <c> <char> - write 16-seg");
        printMenuLine(out, "  d <r> <c> <0|1>    - decimal (c=0-8)");
        printMenuLine(out, "  bl <0|1>           - backlight");
    }

    bool hasDecimal() const override { return true; }
};

#endif
