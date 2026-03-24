#ifndef PCF85176_6DIGSIGBATTPROGRESS_PLUGIN_H
#define PCF85176_6DIGSIGBATTPROGRESS_PLUGIN_H

#include "LCDPlugin.h"
#include "SegLCD_PCF85176_6DigSigBattProgress.h"
#include "SegTransport.h"
#include <Wire.h>

class PCF85176_6DigSigBattProgressPlugin : public LCDPlugin {
public:
    PCF85176_6DigSigBattProgressPlugin() : _bus(Wire) {}

    const char *name() const override { return "6prog"; }

    SegLCDLib* create(const DisplayConfig &cfg) override {
        if (cfg.sda < 0 && cfg.scl < 0) {
            Wire.begin();
        } else if (cfg.sda >= 0 && cfg.scl >= 0) {
            initI2CBus(cfg);
        } else {
            Serial.println("Error: set both SDA and SCL or leave both default");
            return nullptr;
        }
        auto *lcd = new SegLCD_PCF85176_6DigitSignalBatteryProgress(_bus, cfg.i2cAddr, cfg.subAddr);
        initPowerPin(cfg.power);
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
        return false;
    }

    void printMenu(Stream &out) override {
        printMenuLine(out, "6prog commands:");
        printMenuLine(out, "  b <0-4>            - battery level");
        printMenuLine(out, "  g <0-4>            - signal level");
        printMenuLine(out, "  prog <0-150>       - progress bar");
        printMenuLine(out, "  wheel <value>      - wheel");
        printMenuLine(out, "  ls/lc <mask>       - set/clear labels");
    }

private:
    SegTransportI2CArduino _bus;
};

#endif
