#include <SegDriver_HT1622.h>


SegDriver_HT1622::SegDriver_HT1622(SegTransport3Wire& transport, uint8_t chipselect)
    : SegDriver_3Wire(transport, chipselect) {}
