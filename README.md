# SegLCDLib

Arduino library for segment LCD displays. Provides abstract interface, controller drivers (PCF85176/HT1621/HT1622/VK0192), and implementations for common displays.

## Quick Start

```cpp
#include "SegLCD_HT1621_4SegDegree.h"

// Pins: CLK=5, DATA=6, CS=7 (adjust to your pins)
SegLCD_HT1621_4SegDegree lcd(5, 6, 7);

void setup() {
    pinMode(5, OUTPUT);   // CLK
    pinMode(6, OUTPUT);   // DATA
    pinMode(7, OUTPUT);   // CS
    lcd.init();
}

void loop() {
    lcd.setCursor(0, 0);
    lcd.print(25);        // Display: 25
    delay(1000);
}
```

## Documentation

- **[Getting Started](docs/getting-started.md)**: Installation, first project, LCD selection
- **[Supported LCDs](docs/supported-lcds.md)**: Complete catalog with pinout and examples
- **[Architecture](docs/architecture.md)**: Class hierarchy and design patterns
- **[Controllers](docs/controllers.md)**: Protocol details (I2C, 3-wire serial)
- **[Adding New LCD](docs/adding-new-lcd.md)**: Tutorial for custom displays
- **[API Reference](https://petrkr.github.io/SegLCDLib/)**: Full Doxygen documentation

## Supported Controllers

| Controller | Protocol | Pins | Max Digits |
|-----------|----------|------|-----------|
| PCF85176 | I2C | 2 | 13 |
| PCF8576 | I2C | 2 | 13 |
| HT1621 | 3-wire | 3 | 6 |
| HT1622 | 3-wire | 3 | 10+ |
| VK0192 | 3-wire | 3 | 5 |

## Featured Displays

See [Supported LCDs](docs/supported-lcds.md) for full catalog.

| Display | Controller | Features |
|---------|-----------|----------|
| Temperature/Humidity | PCF85176 | 6-digit, icons |
| 6-Digit Signal/Battery | PCF85176 | Signal bars, battery, progress |
| 4-Digit with Degree | HT1621 | Colon, degree symbol |
| 6-Digit with Battery | HT1621 | Battery indicator |
| 10-Digit 16-Segment | HT1622 | Enhanced segments |
| 5-Digit Signal/Battery | VK0192 | Advanced controller |

## Installation

### Arduino Library Manager
1. **Sketch** → **Include Library** → **Manage Libraries**
2. Search `SegLCDLib`
3. Click **Install**

### Manual
1. Download: https://github.com/petrkr/SegLCDLib
2. Copy to `Arduino/libraries/SegLCDLib`
3. Restart Arduino IDE

## Examples

13 complete examples in `examples/` directory:
- PCF85176: 6 examples (RawLCD, TempHum, 6DigSigBat, etc.)
- HT1621: 3 examples (4DigDeg, 6DigBat, RawLCD)
- HT1622: 2 examples (10Digit16Segment, RawLCD)
- VK0192: 1 example (5DigSigBatProg)
- PCF8576: 1 example (4Seg6SegMaintSegBatUnits)

## Features

- **Abstract Interface**: Consistent LCD API 1.0 (Arduino LCD standard)
- **Multiple Protocols**: I2C (PCF85176) and 3-wire serial (HT1621/1622/VK0192)
- **RAM Efficient**: Buffered writes, minimal memory overhead
- **Print Compatibility**: Inherits from `Print` class for `print()`/`println()`
- **Flexible**: Easy to add support for new displays

## Architecture

```
Application Code
    ↓
SegLCDLib Base (LCD API 1.0)
    ↓
Controller Driver (PCF85176, HT1621, etc.)
    ↓
LCD Implementation (display-specific)
    ↓
Hardware (I2C / 3-wire serial)
```

See [Architecture](docs/architecture.md) for details.

## Support

- **Issues**: https://github.com/petrkr/SegLCDLib/issues
- **Documentation**: https://petrkr.github.io/SegLCDLib/
- **Reference**: https://playground.arduino.cc/Code/LCDAPI/
