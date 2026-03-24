#ifndef HT1621_LCM88128_PLUGIN_H
#define HT1621_LCM88128_PLUGIN_H

#include "LCDPlugin.h"
#include "SegLCD_HT1621_LCM88128.h"

class HT1621_LCM88128Plugin : public LCDPlugin {
private:
    SegTransport3WireArduino *_transport = nullptr;

public:
    const char *name() const override { return "lcm88128"; }

    SegLCDLib* create(const DisplayConfig &cfg) override {
        if (cfg.cs < 0 || cfg.wr < 0 || cfg.data < 0) {
            Serial.println("Error: CS/WR/DATA pins required");
            return nullptr;
        }

        _transport = new SegTransport3WireArduino(cfg.data, cfg.wr);
        auto *lcd = new SegLCD_HT1621_LCM88128(*_transport, cfg.cs);
        initPowerPin(cfg.power);
        lcd->init();
        lcd->setAutoFlush(true);
        lcd->clear();
        return lcd;
    }

    void destroy(SegLCDLib *lcd) override {
        delete static_cast<SegLCD_HT1621_LCM88128*>(lcd);
        delete _transport;
        _transport = nullptr;
    }

    bool handleCommand(SegLCDLib *lcdBase, const char *cmd, char *args, Stream &out) override {
        auto *lcd = static_cast<SegLCD_HT1621_LCM88128*>(lcdBase);

        if (strcmp(cmd, "b") == 0) {
            lcd->setBatteryLevel((uint8_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "load") == 0) {
            lcd->setLoadLevel((uint8_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "pv") == 0) {
            lcd->setPVLevel((uint8_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "us") == 0) {
            lcd->setUnits((uint32_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "uc") == 0) {
            lcd->clearUnits((uint32_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "ls") == 0) {
            lcd->setLabels((uint32_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "lc") == 0) {
            lcd->clearLabels((uint32_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "ss") == 0) {
            lcd->setSymbols((uint32_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "sc") == 0) {
            lcd->clearSymbols((uint32_t)parseNumber(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "clock") == 0) {
            lcd->setClockSymbol(parseBool(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "maint") == 0) {
            lcd->setMaintenanceSymbol(parseBool(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "warn") == 0) {
            lcd->setWarningSymbol(parseBool(nextToken(&args)));
            return true;
        }
        if (strcmp(cmd, "mute") == 0) {
            lcd->setMuteSymbol(parseBool(nextToken(&args)));
            return true;
        }
        return false;
    }

    void printMenu(Stream &out) override {
        printMenuLine(out, "lcm88128 commands:");
        printMenuLine(out, "  b <0-5>            - battery level");
        printMenuLine(out, "  load <0-5>         - load level");
        printMenuLine(out, "  pv <0-4>           - PV panel level");
        printMenuLine(out, "  us/uc <mask>       - set/clear units");
        printMenuLine(out, "  ls/lc <mask>       - set/clear labels");
        printMenuLine(out, "  ss/sc <mask>       - set/clear symbols");
        printMenuLine(out, "  clock <0|1>        - clock symbol");
        printMenuLine(out, "  maint <0|1>        - maintenance symbol");
        printMenuLine(out, "  warn <0|1>         - warning symbol");
        printMenuLine(out, "  mute <0|1>         - mute symbol");
    }
};

#endif
