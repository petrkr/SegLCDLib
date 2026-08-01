#include <SegDriver_VK0192.h>


SegDriver_VK0192::SegDriver_VK0192(SegTransport3Wire& transport, uint8_t chipselect)
    : SegDriver_3Wire(transport, chipselect) {}
