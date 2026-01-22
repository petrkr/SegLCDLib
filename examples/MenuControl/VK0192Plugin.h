#ifndef VK0192_PLUGIN_H
#define VK0192_PLUGIN_H

#include "LCDPlugin.h"

// Avoid macro conflict with T1T2Plugin's ADDR_BATT
#ifdef ADDR_BATT
#undef ADDR_BATT
#endif

#include "SegLCD_VK0192_5DigSigBattProgress.h"

class VK0192Plugin : public LCDPlugin {
public:
    const char *name() const override { return "VK0192"; }

    SegLCDLib* create(const DisplayConfig &cfg) override {
        if (cfg.cs < 0 || cfg.wr < 0 || cfg.data < 0) {
            Serial.println("Error: CS/WR/DATA pins required");
            return nullptr;
        }

        auto *lcd = new SegLCD_VK0192_5DigSigBattProgress(cfg.cs, cfg.data, cfg.wr);
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
        delete static_cast<SegLCD_VK0192_5DigSigBattProgress*>(lcd);
    }

    bool handleCommand(SegLCDLib *lcdBase, const char *cmd, char *args, Stream &out) override {
        auto *lcd = static_cast<SegLCD_VK0192_5DigSigBattProgress*>(lcdBase);

        if (strcmp(cmd, "b") == 0) {
            lcd->setBatteryLevel((uint8_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "g") == 0) {
            lcd->setSignalLevel((uint8_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "prog") == 0) {
            lcd->setProgress((uint8_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "ls") == 0) {
            lcd->setLabels((uint16_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "lc") == 0) {
            lcd->clearLabels((uint16_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "bl") == 0) {
            lcd->setBacklight(parseBool(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "d") == 0) {
            lcd->setDecimal((uint8_t)parseNumber(nextToken(&args)),
                           (uint8_t)parseNumber(nextToken(&args)),
                           parseBool(nextToken(&args)));
            return true;
        }

        return false;
    }

    void printMenu(Stream &out) override {
        printMenuLine(out, "VK0192 commands:");
        printMenuLine(out, "  b <0-4>         - battery level");
        printMenuLine(out, "  g <0-3>         - signal level");
        printMenuLine(out, "  prog <0-150>    - progress bar");
        printMenuLine(out, "  ls/lc <mask>    - set/clear labels");
        printMenuLine(out, "  bl <0|1>        - backlight");
        printMenuLine(out, "  d <r> <c> <0|1> - decimal point");
    }

    // Capabilities
    bool hasDecimal() const override { return true; }
};

#endif
