#ifndef HT1621_6SEGBAT_PLUGIN_H
#define HT1621_6SEGBAT_PLUGIN_H

#include "LCDPlugin.h"
#include "SegLCD_HT1621_6SegBat.h"

class HT1621_6SegBatPlugin : public LCDPlugin {
public:
    const char *name() const override { return "6bat"; }

    SegLCDLib* create(const DisplayConfig &cfg) override {
        if (cfg.cs < 0 || cfg.wr < 0 || cfg.data < 0) {
            Serial.println("Error: CS/WR/DATA pins required");
            return nullptr;
        }

        auto *lcd = new SegLCD_HT1621_6SegBat(cfg.cs, cfg.data, cfg.wr);
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
        delete static_cast<SegLCD_HT1621_6SegBat*>(lcd);
    }

    bool handleCommand(SegLCDLib *lcdBase, const char *cmd, char *args, Stream &out) override {
        (void)out;
        auto *lcd = static_cast<SegLCD_HT1621_6SegBat*>(lcdBase);

        if (strcmp(cmd, "b") == 0) {
            lcd->setBatteryLevel((uint8_t)parseNumber(nextToken(&args)));
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
        printMenuLine(out, "6bat commands:");
        printMenuLine(out, "  b <0-3>         - battery level");
        printMenuLine(out, "  d <r> <c> <0|1> - decimal (c=2-4)");
        printMenuLine(out, "  bl <0|1>        - backlight");
    }

    bool hasDecimal() const override { return true; }
};

#endif
