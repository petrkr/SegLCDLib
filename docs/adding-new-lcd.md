# Adding New LCD Display Support

Guide for implementing support for a new segment LCD display not yet in SegLCDLib.

---

## Overview

Adding a display involves 5 steps:

1. **Identify the controller** (PCF85176, HT1621, HT1622, or VK0192)
2. **Test with RAW LCD class** (prototype segment mapping)
3. **Create dedicated LCD class** (implement display-specific driver)
4. **Create example sketch** (demonstrate usage)
5. **Document the display** (add to [supported-lcds.md](supported-lcds.md))

Estimated time: 1-2 hours for first-time implementation.

---

## Step 1: Identify the Controller

### Examine the LCD Module

Look for markings on the LCD module itself:

```
LCD Module (bottom view):
┌─────────────────┐
│ COB             │  ← Integrated controller (HT1621, HT1622, VK0192)
│ HT1621 marked   │
└─────────────────┘
```

**Common scenarios:**

| Controller Marked | Type | Pins | Notes |
|------------------|------|------|-------|
| PCF85176 (separate IC) | I2C | 2 (SDA, SCL) | See separate IC on PCB |
| PCF8576 (separate IC) | I2C | 2 (SDA, SCL) | Variant of PCF85176 |
| None (COB) | Usually 3-wire | 3 (CLK, DATA, CS) | Integrated in module |
| HT1621 | 3-wire serial | 3 | Standard 6-digit, small |
| HT1622 | 3-wire serial | 3 | Larger RAM, 10+ digits |
| VK0192 | 3-wire serial | 3 | Irregular addressing |

### Determine I2C Address (if PCF85176)

Use the standard I2C scanner:

```cpp
#include <Wire.h>

void setup() {
    Serial.begin(9600);
    Wire.begin();
}

void loop() {
    for (uint8_t addr = 0x00; addr < 0xFF; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
            Serial.print("Device found at 0x");
            Serial.println(addr, HEX);
        }
    }
    delay(5000);
}
```

**Expected I2C addresses for PCF85176:** 0x38 (SA0=0) or 0x39 (SA0=1)

### Document Pinout

Create a pinout table:

```
LCD Module Pinout (My Display):
Pin  Name      Connect To
1    VCC       3.3V/5V
2    GND       GND
3    SDA       Arduino A4 (I2C)
4    SCL       Arduino A5 (I2C)
```

---

## Step 2: Test with RAW LCD Class

Use the appropriate RAW LCD class to prototype segment mapping without writing a full driver.

### For PCF85176 (I2C)

```cpp
#include <Wire.h>
#include "SegLCD_PCF85176_Raw.h"

SegLCD_PCF85176_Raw lcd(Wire);  // Default I2C address 0x38

void setup() {
    Wire.begin();
    Serial.begin(9600);
    lcd.init();
    Serial.println("RAW LCD initialized. Send segment data via Serial.");
}

void loop() {
    if (Serial.available()) {
        // Read hex values: "00 01 02 03 ..." to set segments
        uint8_t addr = Serial.parseInt();
        uint8_t value = Serial.parseInt();

        // Write raw segment data
        lcd._ramBuffer[addr] = value;
        lcd._writeRam(lcd._ramBuffer[addr], addr * 2);

        Serial.print("Address 0x");
        Serial.print(addr, HEX);
        Serial.print(" = 0x");
        Serial.println(value, HEX);
    }
}
```

**Send commands via Serial Monitor:**
```
0 FF        // Set address 0 to 0xFF (all segments on)
1 01        // Set address 1 to 0x01
```

### For HT1621/1622/VK0192 (3-Wire)

```cpp
#include "SegLCD_HT1621_Raw.h"  // Use for HT1621, HT1622 (same driver)
// OR
#include "SegLCD_VK0192_Raw.h"  // Use for VK0192 (different addressing)

const int CLK = 5, DATA = 6, CS = 7;
SegTransport3WireArduino transport(DATA, CLK);
SegLCD_HT1621_Raw lcd(transport, CS);

void setup() {
    Serial.begin(9600);
    lcd.init();
    Serial.println("RAW LCD ready");
}

void loop() {
    if (Serial.available()) {
        // Input: "00 FF" (address 0x00, value 0xFF)
        uint8_t addr = Serial.parseInt();
        uint8_t value = Serial.parseInt();

        // Write to controller RAM at byte index addr
        // 3-wire drivers use nibble-oriented HW addresses
        lcd._ramBuffer[addr] = value;
        lcd._writeRam(lcd._ramBuffer[addr], addr * 2);

        Serial.print("Address ");
        Serial.print(addr, HEX);
        Serial.print(" = ");
        Serial.println(value, HEX);
    }
}
```

### Mapping Process

1. **Enable all segments:** Send 0xFF to each address in sequence
2. **Observe display:** Note which segments light up for each address
3. **Record mapping:** Create table: Address → Display Position

Example table for 6-digit display:

```
Address  Digit   Segments    Binary   Notes
0x00     1       a,b,c,d,e,f,g  11111110  Standard 7-segment
0x01     1       decimal point  00000001
0x02     1       colon          00000010
0x03     2       a,b,c,d,e,f,g  11111110
0x04     2       decimal point  00000001
...
```

---

## Step 3: Create Dedicated LCD Class

### File Structure

Create two files:

- `src/SegLCD_[CONTROLLER]_[DISPLAY].h` (header)
- `src/SegLCD_[CONTROLLER]_[DISPLAY].cpp` (implementation)

Example for PCF85176 6-digit display:
- `src/SegLCD_PCF85176_MyDisplay.h`
- `src/SegLCD_PCF85176_MyDisplay.cpp`

### Header File Template

```cpp
// File: src/SegLCD_PCF85176_MyDisplay.h
#ifndef SEGLCD_PCF85176_MYDISPLAY_H
#define SEGLCD_PCF85176_MYDISPLAY_H

#include "SegDriver_PCF85176.h"

/**
 * @brief PCF85176 driver for My Custom 6-Digit Display
 *
 * Features:
 * - 6 7-segment digits
 * - Battery indicator
 * - Signal strength
 *
 * @example PCF85176/MyDisplay/MyDisplay.ino
 */
class SegLCD_PCF85176_MyDisplay : public SegDriver_PCF85176 {
public:
    /**
     * @brief Constructor
     * @param i2c I2C bus (Wire)
     * @param address I2C address (0x38 or 0x39 based on SA0 pin)
     * @param subaddress Protocol subaddress (A0-A2 pins, default 0)
     */
    SegLCD_PCF85176_MyDisplay(TwoWire& i2c, uint8_t address = 0x38, uint8_t subaddress = 0);

    /**
     * @brief Initialize display
     */
    void init();

    /**
     * @brief Output character at current cursor position
     * @param ch Character to display (digit 0-9, A-F, etc.)
     * @return Number of characters written
     */
    virtual size_t write(uint8_t ch);

    /**
     * @brief Set battery indicator level
     * @param level 0=off, 1=low, 2=medium, 3=high
     */
    void setBattery(uint8_t level);

    /**
     * @brief Set signal strength indicator
     * @param bars 0-4 (0=no signal, 4=full strength)
     */
    void setSignal(uint8_t bars);

private:
    // 7-segment character map (0-15 for digits/hex)
    static const uint8_t _charMap[256];

    // Map logical digit/segments to physical RAM addresses
    void _mapSegments(uint8_t digit, uint8_t segments);

    // Segment positions for symbols (calculated from mapping)
    static const uint8_t _batteryAddr;
    static const uint8_t _batteryBits[4];
    static const uint8_t _signalAddr;
    static const uint8_t _signalBits[5];
};

#endif
```

### Implementation File Template

```cpp
// File: src/SegLCD_PCF85176_MyDisplay.cpp
#include "SegLCD_PCF85176_MyDisplay.h"

// 7-segment character map (from standard LCD fonts)
const uint8_t SegLCD_PCF85176_MyDisplay::_charMap[256] = {
    // Segment layout: gfedcba (common Arduino convention)
    0x3F,  // '0' = 0b00111111 (segments a,b,c,d,e,f)
    0x06,  // '1' = 0b00000110 (segments b,c)
    0x5B,  // '2' = 0b01011011 (segments a,b,d,e,g)
    0x4F,  // '3' = 0b01001111 (segments a,b,c,d,g)
    0x66,  // '4' = 0b01100110 (segments b,c,f,g)
    0x6D,  // '5' = 0b01101101 (segments a,c,d,f,g)
    0x7D,  // '6' = 0b01111101 (segments a,c,d,e,f,g)
    0x07,  // '7' = 0b00000111 (segments a,b,c)
    0x7F,  // '8' = 0b01111111 (all segments)
    0x6F,  // '9' = 0b01101111 (segments a,b,c,d,f,g)
    // ... add more characters as needed
    0x00   // All unused = off
};

// Battery indicator positions (from your RAW LCD testing)
const uint8_t SegLCD_PCF85176_MyDisplay::_batteryAddr = 0x12;
const uint8_t SegLCD_PCF85176_MyDisplay::_batteryBits[4] = {
    0x00,  // Off
    0x01,  // Low (1 bar)
    0x03,  // Medium (2 bars)
    0x07   // High (3 bars)
};

SegLCD_PCF85176_MyDisplay::SegLCD_PCF85176_MyDisplay(uint8_t i2cAddress)
    : SegDriver_PCF85176(i2cAddress) {
}

void SegLCD_PCF85176_MyDisplay::init() {
    SegDriver_PCF85176::init();  // Call parent init
}

size_t SegLCD_PCF85176_MyDisplay::write(uint8_t ch) {
    if (_cursorCol >= 6) {  // Max 6 digits
        return 0;
    }

    // Get 7-segment pattern for character
    uint8_t segments = _charMap[ch];

    // Map to display RAM
    _mapSegments(_cursorCol, segments);

    // Write to hardware
    _writeRam();

    // Advance cursor
    _cursorCol++;
    return 1;
}

void SegLCD_PCF85176_MyDisplay::_mapSegments(uint8_t digit, uint8_t segments) {
    // Map digit position (0-5) to RAM address(es)
    // Based on your RAW LCD testing results

    // Example mapping (adjust to your actual display):
    // Digit 0: address 0x00 (segments gfedcba)
    // Digit 1: address 0x01
    // etc.

    uint8_t addr = digit;  // Simple linear mapping (yours may differ)

    _ramBuffer[addr] = segments;
    _writeRam(_ramBuffer[addr], addr * 2);
}

void SegLCD_PCF85176_MyDisplay::setBattery(uint8_t level) {
    if (level > 3) level = 3;

    // Clear battery bits, then set new level
    _ramBuffer[_batteryAddr] &= 0xF8;  // Clear bits 0-2
    _ramBuffer[_batteryAddr] |= _batteryBits[level];

    _writeRam(_ramBuffer[_batteryAddr], _batteryAddr * 2);
}

void SegLCD_PCF85176_MyDisplay::setSignal(uint8_t bars) {
    if (bars > 4) bars = 4;

    // Similar to battery (implementation depends on your mapping)
    // ...
}
```

### Key Implementation Notes

1. **Cursor Tracking:** Track `_cursorRow` and `_cursorCol` (inherit from base class)
2. **Character Map:** Populate `_charMap[]` array with 7-segment patterns for each character
3. **Segment Mapping:** Implement `_mapSegments()` based on your RAW LCD testing
4. **Feature Methods:** Add `setBattery()`, `setSignal()`, etc. for display-specific features
5. **RAM Buffering:** Update `_ramBuffer[]`, then sync affected byte(s) with `_writeRam(...)` or `flush(...)`

---

## Step 4: Create Example Sketch

Place in `examples/[CONTROLLER]/[DisplayType]/[DisplayType].ino`

Example for PCF85176 6-digit display:

```cpp
// File: examples/PCF85176/MyDisplay/MyDisplay.ino
#include <Wire.h>
#include "SegLCD_PCF85176_MyDisplay.h"

// Create LCD instance
SegLCD_PCF85176_MyDisplay lcd(Wire);

void setup() {
    Wire.begin();
    lcd.init();
    lcd.clear();

    Serial.begin(9600);
    Serial.println("My Display Example Started");
}

void loop() {
    // Display counter
    static int counter = 0;

    lcd.setCursor(0, 0);
    lcd.print(counter);

    // Show battery level (cycling through 0-3)
    lcd.setBattery(counter % 4);

    // Show signal strength
    lcd.setSignal((counter / 2) % 5);

    counter++;
    if (counter > 999999) counter = 0;

    delay(1000);
}
```

---

## Step 5: Document the Display

### Update docs/supported-lcds.md

Add a section for your display under the appropriate controller:

```markdown
### My Custom Display

**Specifications:**
- **Digits:** 6 7-segment digits
- **Features:** Battery level, Signal strength
- **Wiring:** I2C (SDA, SCL)
- **I2C Address:** 0x38 (SA0=0) or 0x39 (SA0=1)
- **Controller:** PCF85176
- **Purchase:** [Link to AliExpress/store]
- **Example:** `examples/PCF85176/MyDisplay/`

**Code Example:**
\`\`\`cpp
#include "SegLCD_PCF85176_MyDisplay.h"
SegLCD_PCF85176_MyDisplay lcd(Wire);

lcd.init();
lcd.print(123456);
lcd.setBattery(2);
lcd.setSignal(3);
\`\`\`
```

---

## Troubleshooting

### Segments Not Showing

1. **Verify I2C/3-wire communication:**
   - Test with RawLCD class first
   - Check wiring (SDA/SCL for I2C, CLK/DATA/CS for 3-wire)

2. **Check segment mapping:**
   - Raw LCD shows all on at address X
   - Your class shows nothing at digit X
   - → `_mapSegments()` has wrong address

3. **Verify character map:**
   - `write()` receives character but doesn't display
   - Check `_charMap[ch]` has correct pattern
   - Test with `0x3F` (all segments on)

### Display Garbled

1. **Mixed up digit order:**
   - Test each digit individually: `lcd.setCursor(0, 0); lcd.write(0xFF);`
   - Adjust address mapping in `_mapSegments()`

2. **Wrong segment bits:**
   - Use RAW LCD to map each segment
   - Verify `_charMap[]` patterns match expected display

### I2C Address Not Found

1. **Check physical connection:**
   - SDA → Arduino A4 (Uno), pin 20 (Mega)
   - SCL → Arduino A5 (Uno), pin 21 (Mega)
   - VCC, GND connected

2. **Scan for address:**
   - Run I2C scanner (code above)
   - PCF85176 uses 0x38 (SA0=0) or 0x39 (SA0=1)

3. **SA0 pin setting:**
   - Look for SA0 pad on module (determines I2C address)
   - A0, A1, A2 are subaddresses in protocol, not I2C address pins

---

## Best Practices

### RAM Optimization

Segment LCD display RAM is precious. Minimize memory usage:

```cpp
// ✓ GOOD: Store patterns in PROGMEM
const uint8_t _charMap[256] PROGMEM = { ... };

// ✗ BAD: Duplicate character map in every class
uint8_t charMap[] = { ... };
```

### Buffer Management

```cpp
// ✓ GOOD: Batch updates
lcd.print(12345);        // Updates _cursorCol
lcd.setBattery(2);       // Only modifies relevant bits
lcd._writeRam();         // One I2C transaction

// ✗ BAD: Multiple hardware writes
lcd._writeRam();
lcd._writeRam();
lcd._writeRam();
```

### Cursor Tracking

Always track cursor position:

```cpp
size_t write(uint8_t ch) {
    if (_cursorCol >= MAX_DIGITS) return 0;

    _mapSegments(_cursorCol, segments);
    _cursorCol++;  // Important: advance cursor
    return 1;
}
```

---

## Reference

- [Architecture](architecture.md): Class hierarchy and design patterns
- [Controllers](controllers.md): Protocol details
- [Supported LCDs](supported-lcds.md): Existing implementations
- [Getting Started](getting-started.md): Basic usage

## Testing Checklist

- [ ] RAW LCD class maps all segments
- [ ] Pinout documented and verified
- [ ] `_charMap[]` populated with patterns
- [ ] `_mapSegments()` implemented correctly
- [ ] Digits display in correct order
- [ ] `write()` advances cursor
- [ ] Feature methods (battery, signal) work
- [ ] Example sketch runs without errors
- [ ] Documentation added to `supported-lcds.md`

---

## Example Progression

If your display is complex, follow this order:

1. Test RAW LCD: Does hardware work?
2. Implement `init()` and `write()`: Can I display digits?
3. Add features: Battery, signal, symbols
4. Optimize: RAM usage, buffer management
5. Document: Add to supported-lcds.md

Each stage tests one aspect before moving to the next.
