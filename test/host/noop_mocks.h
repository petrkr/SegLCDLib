#ifndef SEGLCDLIB_TEST_NOOP_MOCKS_H
#define SEGLCDLIB_TEST_NOOP_MOCKS_H

#include "SegTransport.h"
#include "SegBacklight.h"

class NoopTransport3Wire : public SegTransport3Wire {
    public:
        void init_cs(uint8_t) override {}
        void set_cs(uint8_t, bool) override {}
        void write(uint16_t, uint8_t) override {}
};

class NoopTransportI2C : public SegTransportI2C {
    public:
        void write(uint8_t, uint8_t) override {}
        void write(uint8_t, uint8_t*, size_t) override {}
};

class NoopBacklight : public SegBacklight {
    public:
        void init(bool) override {}
        void setDigital(bool) override {}
        void setBrightness(uint8_t) override {}
};

#endif
