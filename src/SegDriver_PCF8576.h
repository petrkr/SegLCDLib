#ifndef SEGDRIVER_PCF8576_H
#define SEGDRIVER_PCF8576_H

#include <SegDriver_PCF85176.h>

/**
 * @brief PCF8576 LCD segment driver.
 *
 * PCF8576 is protocol-compatible with PCF85176 and is superseded by PCF85176.
 * This class only provides a chip-specific type name.
 */
class SegDriver_PCF8576 : public SegDriver_PCF85176 {
    public:
        using SegDriver_PCF85176::SegDriver_PCF85176; // inherit constructors
};

#endif
