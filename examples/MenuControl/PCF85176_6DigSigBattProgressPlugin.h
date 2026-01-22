#ifndef PCF85176_6DIGSIGBATTPROGRESS_PLUGIN_H
#define PCF85176_6DIGSIGBATTPROGRESS_PLUGIN_H

#include "LCDPlugin.h"
#include "SegLCD_PCF85176_6DigSigBattProgress.h"
#include <Wire.h>

class PCF85176_6DigSigBattProgressPlugin : public LCDPlugin {
public:
    const char *name() const override { return "6prog"; }

    SegLCDLib* create(const DisplayConfig &cfg) override {
        if (cfg.sda < 0 && cfg.scl < 0) {
            Wire.begin();
        } else if (cfg.sda >= 0 && cfg.scl >= 0) {
            Wire.begin(cfg.sda, cfg.scl);
        } else {
            Serial.println("Error: set both SDA and SCL or leave both default");
            return nullptr;
        }
        auto *lcd = new SegLCD_PCF85176_6DigitSignalBatteryProgress(Wire, cfg.i2cAddr, cfg.subAddr);
        lcd->init();
        lcd->setAutoFlush(true);
        lcd->clear();
        return lcd;
    }

    void destroy(SegLCDLib *lcd) override {
        delete static_cast<SegLCD_PCF85176_6DigitSignalBatteryProgress*>(lcd);
    }

    bool handleCommand(SegLCDLib *lcdBase, const char *cmd, char *args, Stream &out) override {
        (void)out;
        auto *lcd = static_cast<SegLCD_PCF85176_6DigitSignalBatteryProgress*>(lcdBase);

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
        if (strcmp(cmd, "wheel") == 0) {
            lcd->setWheel((uint16_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "ls") == 0) {
            lcd->setLabels((SegLCD_PCF85176_6DigitSignalBatteryProgress::LabelFlags)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "lc") == 0) {
            lcd->clearLabels((SegLCD_PCF85176_6DigitSignalBatteryProgress::LabelFlags)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "colon") == 0) {
            lcd->setColon((uint8_t)parseNumber(nextToken(&args)),
                         (uint8_t)parseNumber(nextToken(&args)),
                         parseBool(nextToken(&args)));
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
        printMenuLine(out, "6prog commands:");
        printMenuLine(out, "  b <0-4>            - battery level");
        printMenuLine(out, "  g <0-4>            - signal level");
        printMenuLine(out, "  prog <0-150>       - progress bar");
        printMenuLine(out, "  wheel <value>      - wheel");
        printMenuLine(out, "  ls/lc <mask>       - set/clear labels");
        printMenuLine(out, "  colon <r> <c> <0|1> - colon");
        printMenuLine(out, "  d <r> <c> <0|1>    - decimal");
    }

    bool hasDecimal() const override { return true; }
    bool hasColon() const override { return true; }
};

#endif
