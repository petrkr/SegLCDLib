#ifndef HT1621_6SEGBAT_PLUGIN_H
#define HT1621_6SEGBAT_PLUGIN_H

#include "LCDPlugin.h"
#include "SegLCD_HT1621_6SegBat.h"

class HT1621_6SegBatPlugin : public LCDPlugin {
private:
    SegTransport3WireArduino *_transport = nullptr;

public:
    const char *name() const override { return "6bat"; }

    SegLCDLib* create(const DisplayConfig &cfg) override {
        if (cfg.cs < 0 || cfg.wr < 0 || cfg.data < 0) {
            Serial.println("Error: CS/WR/DATA pins required");
            return nullptr;
        }

        _transport = new SegTransport3WireArduino(cfg.data, cfg.wr);
        auto *lcd = new SegLCD_HT1621_6SegBat(*_transport, cfg.cs);
        initPowerPin(cfg.power);
        lcd->init();
        lcd->setAutoFlush(true);
        lcd->clear();
        return lcd;
    }

    void destroy(SegLCDLib *lcd) override {
        delete static_cast<SegLCD_HT1621_6SegBat*>(lcd);
        delete _transport;
        _transport = nullptr;
    }

    bool handleCommand(SegLCDLib *lcdBase, const char *cmd, char *args, Stream &out) override {
        (void)out;
        auto *lcd = static_cast<SegLCD_HT1621_6SegBat*>(lcdBase);

        if (strcmp(cmd, "b") == 0) {
            lcd->setBatteryLevel((uint8_t)parseNumber(nextToken(&args)));
            return true;
        }
        return false;
    }

    void printMenu(Stream &out) override {
        printMenuLine(out, "6bat commands:");
        printMenuLine(out, "  b <0-3>         - battery level");
    }
};

#endif
