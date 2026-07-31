#ifndef PCF85134_XYGAX_SEG_I2C_PLUGIN_H
#define PCF85134_XYGAX_SEG_I2C_PLUGIN_H

#include "LCDPlugin.h"
#include "SegLCD_PCF85134_XygaxSegI2C.h"

class PCF85134_XygaxSegI2CPlugin : public LCDPlugin {
public:
    PCF85134_XygaxSegI2CPlugin() : _bus(Wire) {}

    const char *name() const override { return "xygax_seg_i2c"; }

    SegLCDLib* create(const DisplayConfig &cfg) override {
        if (cfg.sda < 0 && cfg.scl < 0) {
            Wire.begin();
        } else if (cfg.sda >= 0 && cfg.scl >= 0) {
            initI2CBus(cfg);
        } else {
            Serial.println("Error: set both SDA and SCL or leave both default");
            return nullptr;
        }
        auto *lcd = new SegLCD_PCF85134_XygaxSegI2C(_bus, cfg.i2cAddr, cfg.subAddr);
        initPowerPin(cfg.power);
        lcd->init();
        lcd->setAutoFlush(true);
        lcd->clear();
        return lcd;
    }

    void destroy(SegLCDLib *lcd) override {
        delete static_cast<SegLCD_PCF85134_XygaxSegI2C*>(lcd);
    }

    bool handleCommand(SegLCDLib *lcdBase, const char *cmd, char *args, Stream &out) override {
        (void)lcdBase;
        (void)cmd;
        (void)args;
        (void)out;
        return false;
    }

    void printMenu(Stream &out) override {
        (void)out;
    }

private:
    SegTransportI2CArduino _bus;
};

#endif
