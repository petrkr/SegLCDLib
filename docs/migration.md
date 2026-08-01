# Migration Guide

This guide documents user-visible changes required when upgrading between SegLCDLib versions.

## v0.0.1 to v0.0.2

Migration checklist:

- [Transport layer](#transport-layer)
- [PCx85 raw class split](#pcx85-raw-class-split)
- [Labels renamed to units](#labels-renamed-to-units)
- [Direct driver includes](#direct-driver-includes)
- [Custom LCD implementations](#custom-lcd-implementations)
- [Backlight initialization](#backlight-initialization)

### Transport layer {#transport-layer}

LCD constructors no longer take Arduino `Wire` or GPIO pins directly. Create a transport object and pass it to the LCD class.

Old:

```cpp
#include "SegLCD_PCF85176_OneDigit.h"
#include <Wire.h>

SegLCD_PCF85176_OneDigit lcd(Wire);
```

New:

```cpp
#include "SegTransportArduino.h"
#include "SegLCD_PCF85176_OneDigit.h"
#include <Wire.h>

SegTransportI2CArduino transport(Wire);
SegLCD_PCF85176_OneDigit lcd(transport);
```

For 3-wire controllers:

```cpp
#include "SegTransportArduino.h"
#include "SegLCD_HT1621_4SegDegree.h"

SegTransport3WireArduino transport(6, 5);  // DATA, WR
SegLCD_HT1621_4SegDegree lcd(transport, 7);  // CS
```

This affects all existing `v0.0.1` LCD constructors:

- PCx85 I2C LCDs changed from `TwoWire&` to `SegTransportI2C&`
- HT1621/HT1622/VK0192 LCDs changed from direct GPIO pins to `SegTransport3Wire&` plus chip-select

### PCx85 raw class split {#pcx85-raw-class-split}

The generic `SegLCD_PCx85_Raw` class was replaced by controller-specific raw classes.

Old:

```cpp
#include "SegLCD_PCx85_Raw.h"

SegLCD_PCx85_Raw lcd(Wire);
```

New for PCF85176/PCF8576:

```cpp
#include "SegTransportArduino.h"
#include "SegLCD_PCF85176_Raw.h"

SegTransportI2CArduino transport(Wire);
SegLCD_PCF85176_Raw lcd(transport);
```

Use `SegLCD_PCF85134_Raw` for PCF85134 displays.

### Labels renamed to units {#labels-renamed-to-units}

Physical measurement indicators were renamed from labels to units.

| v0.0.1 | v0.0.2 |
|--------|--------|
| `LabelFlags` | `UnitFlags` |
| `setLabels(...)` | `setUnits(...)` |
| `clearLabels(...)` | `clearUnits(...)` |
| `LABEL_*` unit constants | `UNIT_*` constants |

Affected LCD classes:

- `SegLCD_PCF85176_TempHumidity`
- `SegLCD_PCF85176_6DigitSignalBatteryProgress`
- `SegLCD_PCF85176_T1T2Lcd`
- `SegLCD_PCF8576_4Seg6SegMaintSegBatUnits`
- `SegLCD_VK0192_5DigSigBattProgress` for unit indicators only

Example:

```cpp
// Old
lcd.setLabels(SegLCD_PCF85176_TempHumidity::LABEL_DEGREE_C |
              SegLCD_PCF85176_TempHumidity::LABEL_PROC);

// New
lcd.setUnits(SegLCD_PCF85176_TempHumidity::UNIT_DEGREE_C |
             SegLCD_PCF85176_TempHumidity::UNIT_PERCENT);
```

Display text labels remain labels. For example, `SegLCD_PCF85176_T1T2Lcd::setT1T2Labels(...)` and HT1621 LCM label APIs are unchanged.

### Direct driver includes {#direct-driver-includes}

`SegDriver_PCF85176` and `SegDriver_PCF8576` moved out of `SegDriver_PCx85.h` into their own headers.

If user code includes controller drivers directly, update includes:

```cpp
#include "SegDriver_PCF85176.h"
#include "SegDriver_PCF8576.h"
```

Regular LCD model headers already include the correct controller header.

### Custom LCD implementations {#custom-lcd-implementations}

Custom LCD classes derived from library drivers must be updated for the transport layer.

- I2C custom drivers should inherit from the specific controller driver, such as `SegDriver_PCF85176`.
- 3-wire custom drivers should inherit from the specific controller driver, such as `SegDriver_HT1621`, `SegDriver_HT1622`, or `SegDriver_VK0192`.
- Old global controller macros such as `MAX_ADDRESS` / `MAX_HW_ADDRESS` were replaced by class-scoped constants.

### Backlight initialization {#backlight-initialization}

`initBacklight()` no longer takes a raw GPIO pin number. Create a `SegBacklight` implementation and pass it in instead. The active-high flag moved to the backlight implementation's constructor.

Old:

```cpp
lcd.initBacklight(7, SegLCDLib::BACKLIGHT_DIGITAL, true);
```

New:

```cpp
#include "SegBacklightArduino.h"

SegBacklightArduino backlight(7);  // activeHigh defaults to true
lcd.initBacklight(&backlight, SegLCDLib::BACKLIGHT_DIGITAL);
```

If `initBacklight()` was called conditionally based on a "no backlight" sentinel pin value, keep the `SegBacklightArduino` instance around only when a real pin is configured (see `examples/HT1622/10Digit16SegmentLCD` for a static-pin example, or `examples/MenuControl/MenuControl.cpp` for a dynamically-configured pin using `new`/`delete`).
