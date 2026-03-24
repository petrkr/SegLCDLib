#ifndef PCF85176_TEMPHUM_PLUGIN_H
#define PCF85176_TEMPHUM_PLUGIN_H

#include "LCDPlugin.h"
#include "SegLCD_PCF85176_TempHum.h"
#include "SegTransport.h"
#include <Wire.h>

class PCF85176_TempHumPlugin : public LCDPlugin {
public:
    PCF85176_TempHumPlugin() : _bus(Wire) {}

    const char *name() const override { return "temphum"; }

    SegLCDLib* create(const DisplayConfig &cfg) override {
        if (cfg.sda < 0 && cfg.scl < 0) {
            Wire.begin();
        } else if (cfg.sda >= 0 && cfg.scl >= 0) {
            initI2CBus(cfg);
        } else {
            Serial.println("Error: set both SDA and SCL or leave both default");
            return nullptr;
        }
        auto *lcd = new SegLCD_PCF85176_TempHumidity(_bus, cfg.i2cAddr, cfg.subAddr);
        initPowerPin(cfg.power);
        lcd->init();
        lcd->setAutoFlush(true);
        lcd->clear();
        return lcd;
    }

    void destroy(SegLCDLib *lcd) override {
        auto *typed = static_cast<SegLCD_PCF85176_TempHumidity*>(lcd);
        delete typed;
    }

    bool handleCommand(SegLCDLib *lcdBase, const char *cmd, char *args, Stream &out) override {
        (void)out; // Unused parameter
        auto *lcd = static_cast<SegLCD_PCF85176_TempHumidity*>(lcdBase);

        if (strcmp(cmd, "b") == 0) {
            lcd->setBatteryLevel((uint8_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "g") == 0) {
            lcd->setSignalLevel((uint8_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "us") == 0) {
            lcd->setUnits((uint8_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "uc") == 0) {
            lcd->clearUnits((uint8_t)parseNumber(nextToken(&args)));
            return true;
        }
        return false;
    }

    void printMenu(Stream &out) override {
        printMenuLine(out, "temphum commands:");
        printMenuLine(out, "  b <0-4>         - battery level");
        printMenuLine(out, "  g <0-4>         - signal level");
        printMenuLine(out, "  us/uc <mask>    - set/clear units");
    }

private:
    SegTransportI2CArduino _bus;
};

#endif
