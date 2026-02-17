#ifndef PCF85176_2ROW4DIGSIGBATPWR_PLUGIN_H
#define PCF85176_2ROW4DIGSIGBATPWR_PLUGIN_H

#include "LCDPlugin.h"
#include "SegLCD_PCF85176_2Row4DigSigBatPwr.h"
#include <Wire.h>

class PCF85176_2Row4DigSigBatPwrPlugin : public LCDPlugin {
public:
    const char *name() const override { return "2row4dig"; }

    SegLCDLib* create(const DisplayConfig &cfg) override {
        if (cfg.sda < 0 && cfg.scl < 0) {
            Wire.begin();
        } else if (cfg.sda >= 0 && cfg.scl >= 0) {
            Wire.begin(cfg.sda, cfg.scl);
        } else {
            Serial.println("Error: set both SDA and SCL or leave both default");
            return nullptr;
        }
        auto *lcd = new SegLCD_PCF85176_2Row4DigSigBatPwr(Wire, cfg.i2cAddr, cfg.subAddr);
        initPowerPin(cfg.power);
        lcd->init();
        lcd->setAutoFlush(true);
        lcd->clear();
        return lcd;
    }

    void destroy(SegLCDLib *lcd) override {
        delete static_cast<SegLCD_PCF85176_2Row4DigSigBatPwr*>(lcd);
    }

    bool handleCommand(SegLCDLib *lcdBase, const char *cmd, char *args, Stream &out) override {
        (void)out;
        auto *lcd = static_cast<SegLCD_PCF85176_2Row4DigSigBatPwr*>(lcdBase);

        if (strcmp(cmd, "b") == 0) {
            lcd->setBatteryLevel((uint8_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "g") == 0) {
            lcd->setSignalLevel((uint8_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "pwr") == 0) {
            lcd->setPowerSymbol(parseBool(nextToken(&args)));
            return true;
        }
        return false;
    }

    void printMenu(Stream &out) override {
        printMenuLine(out, "2row4dig commands:");
        printMenuLine(out, "  b <0-4>            - battery level");
        printMenuLine(out, "  g <0-1>            - signal level");
        printMenuLine(out, "  pwr <0|1>          - power symbol");
    }
};

#endif
