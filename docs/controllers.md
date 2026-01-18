# LCD Controllers

Detailed specifications for each supported LCD controller. For quick selection, see [Supported LCDs](supported-lcds.md).

---

## PCF85176 / PCF8576 (I2C)

**Driver:** `SegDriver_PCx85`, `SegDriver_PCF85176`, `SegDriver_PCF8576`

**Note:** PCF8576 is an older variant of PCF85176 with identical functionality. SegLCDLib treats them the same.

### Physical Characteristics

| Property | Value |
|----------|-------|
| **Protocol** | I2C (2-wire) |
| **I2C Addresses** | 0x38-0x3F (8 addresses, A0-A2 pins) |
| **RAM** | 40 bytes (39 usable) |
| **Max Segments** | 320 (40 bytes × 8 bits) |
| **Power** | 3.3V or 5V (controller dependent) |

### I2C Addressing

The I2C address is determined by address pins A0, A2, A2 on the controller:

```
Base address: 0x38
A2  A1  A0  →  Address
0   0   0   →  0x38 (default)
0   0   1   →  0x39
0   1   0   →  0x3A
0   1   1   →  0x3B
1   0   0   →  0x3C
1   0   1   →  0x3D
1   1   0   →  0x3E
1   1   1   →  0x3F
```

**Finding Address:**
Use the `PCF85176/RawLCD` example to scan I2C bus:
```cpp
#include "SegLCD_PCF85176_Raw.h"

void setup() {
    Wire.begin();
    Serial.begin(9600);

    // Scan addresses 0x38-0x3F
    for (uint8_t addr = 0x38; addr <= 0x3F; addr++) {
        SegLCD_PCF85176_Raw lcd(addr);
        lcd.init();
        Serial.println(addr, HEX);  // Found address
    }
}
```

### Communication Protocol

**Initialization Sequence:**
1. I2C START condition
2. Address byte (0x38 + address bits)
3. Control register byte (function setting, bias, drive mode)
4. RAM data bytes (0-39)
5. I2C STOP condition

**Command Structure:**
```
Byte 0: 0x00 - Static mode (no multiplexing)
        0x01 - 1/2 duty
        0x02 - 1/3 duty
        0x04 - 1/4 duty
        + 0x00-0x30 for bias/contrast (varies by device)
```

### Functional Modes

#### Drive Mode (Display Refresh)

Controls how the LCD multiplexer refreshes segments:

| Mode | Duty | Use Case |
|------|------|----------|
| Static (1/1) | 100% of segments on at once | Single digit displays (rare) |
| 1/2 Duty | 2 digit rows | Multi-digit displays, simpler |
| 1/3 Duty | 3 digit rows | 6-digit displays (common) |
| 1/4 Duty | 4 digit rows | 4-digit displays |

Higher multiplexing requires more RAM but reduces pin count on LCD module.

#### Bias Mode

Voltage bias between LCD common and segment pins (determines contrast):

| Mode | Voltage Divider | Typical Use |
|------|-----------------|-------------|
| 1/2 Bias | VDD/2 | Older LCD displays |
| 1/3 Bias | VDD/3 | Modern LCD displays (typical) |

**Note:** Only affects display when in multiplexed mode (1/2, 1/3, 1/4 duty). Static mode ignores bias.

### RAM Layout

```
Byte Index  Content              Segments
─────────────────────────────────────────
0-2         Digit 1 (3 bytes)    0-23
3-5         Digit 2 (3 bytes)    24-47
...
36-38       Digit 13 (3 bytes)   288-311
Byte 39     Unused
```

Each byte controls 8 segments:
```
Byte Layout:
Bit 7  6  5  4  3  2  1  0
          ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓
Segment a b c d e f g H (7-segment notation)
```

### Wiring (Arduino → LCD Module)

```
Arduino        PCF85176 Module
─────────────────────────────
SDA (A4)   →   SDA (pin 18)
SCL (A5)   →   SCL (pin 19)
3.3V/5V    →   VCC
GND        →   GND
```

**Note:** I2C pull-ups (4.7kΩ) typically already on LCD module.

### Code Example

```cpp
#include <Wire.h>
#include "SegLCD_PCF85176_TempHum.h"

void setup() {
    Wire.begin();
    SegLCD_PCF85176_TempHum lcd(0x38);  // Default address
    lcd.init();
}

void loop() {
    lcd.setCursor(0, 0);
    lcd.print(23);      // Temperature
}
```

---

## HT1621 (3-Wire Serial)

**Driver:** `SegDriver_HT1621`

### Physical Characteristics

| Property | Value |
|----------|-------|
| **Protocol** | 3-wire serial (bit-bang GPIO) |
| **RAM** | 16 bytes |
| **Max Segments** | 128 (16 × 8) |
| **Clock Speed** | 32 kHz (typical) |
| **Power** | 3.3V typical, 5V tolerant |
| **Integration** | Usually integrated into LCD module (COB) |

### 3-Wire Protocol

Three GPIO pins control communication:

```
CLK  (Clock)  - Synchronization signal
DATA (Data)   - Serial data line
CS   (Select) - Chip select (active low)
```

**Bit Timing:**
```
CLK Cycle:
    ┌──────┐        ┌──────┐
    │      │        │      │
────┘      └────────┘      └────
    ↑         ↑     ↑         ↑
    Data      Read  Data      Read
    Setup     at    Setup     at
             Rising           Rising
```

**Minimum clock period:** 100μs (32 kHz max)

### Command Structure

**Three command types:**

1. **System Control Command**
   ```
   9 bits: 100_0xxxxx
           └─ SYS_EN  (0=disable, 1=enable)
           └─ SYS_DIS (0=display on, 1=off)
           └─ LCD_ON  (1=LCD enabled)
   ```

2. **Drive/Bias Mode Command**
   ```
   9 bits: 101_xxyzzz
           └─ RC (1/2 vs 1/3 bias)
           └─ Duty (1/2 to 1/4 multiplexing)
           └─ Drive frequency
   ```

3. **Data Write Command**
   ```
   9 bits: 101_AAAAA  (address, 32 addresses for 16 bytes)
   8 bits: Data byte
   ```

### RAM Layout

```
Address  0-3    4-7     8-11    12-15   16-19   20-31
Content  Digit1 Digit2  Digit3  Digit4  Digit5  Digit6
Bits     0-31   32-63   64-95   96-127  128-159 160-191
```

Each address holds 8 segments for one digit.

### Write Sequence

```cpp
// Example: Write 0xAA to address 0x00
CS_LOW();
WriteCommand(0x140);      // Data write command (9 bits)
WriteData(0x00);          // Address (5 bits)
WriteData(0xAA);          // Data (8 bits)
CS_HIGH();
```

### Wiring (Arduino → HT1621 LCD Module)

```
Arduino    HT1621 Module
────────────────────────
Pin 5  →   CLK
Pin 6  →   DATA
Pin 7  →   CS
3.3V   →   VCC
GND    →   GND
```

**Note:** Verify module voltage (3.3V or 5V); most are 3.3V.

### Initialization Code

```cpp
const int CLK = 5, DATA = 6, CS = 7;
SegLCD_HT1621_4SegDegree lcd(CLK, DATA, CS);

void setup() {
    // Set GPIO pins to outputs
    pinMode(CLK, OUTPUT);
    pinMode(DATA, OUTPUT);
    pinMode(CS, OUTPUT);

    lcd.init();  // Initialize controller and clear display
}
```

---

## HT1622 (3-Wire Serial, Enhanced)

**Driver:** `SegDriver_HT1622`

### Physical Characteristics

| Property | Value |
|----------|-------|
| **Protocol** | 3-wire serial (enhanced timing) |
| **RAM** | 32 bytes |
| **Max Segments** | 256 (32 × 8) |
| **Clock Speed** | 32 kHz (typical) |
| **Min Pulse Width** | 4μs (vs 3.33μs for HT1621) |
| **Power** | 3.3V typical |
| **Integration** | Usually integrated into LCD module |

### Differences from HT1621

| Feature | HT1621 | HT1622 |
|---------|--------|--------|
| **RAM** | 16 bytes | 32 bytes |
| **Max Digits** | 6 (8 segments/digit) | 10-16 (depends on layout) |
| **Segment Type** | 7-segment | 7 or 16-segment |
| **Pulse Width** | 3.33μs (typical) | 4μs (minimum) |

### Timing Requirements

HT1622 requires more precise timing:

```
Clock Period:  ≥ 32μs (31.25 kHz max)
Low Pulse:     ≥ 4μs
High Pulse:    ≥ 4μs
```

Bit-banging code must respect 4μs minimum:

```cpp
void writebit(uint8_t bit) {
    digitalWrite(CLK, LOW);
    digitalWrite(DATA, bit ? HIGH : LOW);
    delayMicroseconds(4);
    digitalWrite(CLK, HIGH);
    delayMicroseconds(4);
}
```

### RAM Layout

```
Address  0-7    8-15    16-23   24-31
Content  Digit1 Digit2  Digit3  ...
Bits     0-63   64-127  128-191 192-255
```

Greater RAM supports larger segment displays (16-segment alphanumeric).

### Wiring

Same as HT1621 (3 GPIO pins: CLK, DATA, CS).

---

## VK0192 (3-Wire Serial, Advanced)

**Driver:** `SegDriver_VK0192`

### Physical Characteristics

| Property | Value |
|----------|-------|
| **Protocol** | 3-wire serial (irregular addressing) |
| **RAM** | 24 bytes (48 4-bit addresses) |
| **Max Segments** | 192 (24 × 8, but addressed as 4-bit) |
| **Clock Speed** | 250 kHz (faster than HT1621) |
| **Min Pulse Width** | 4μs (critical timing) |
| **Power** | 3.3V typical |
| **Address Mode** | 4-bit addressing (vs 8-bit for HT1621) |

### Key Difference: Irregular Addressing

Unlike HT1621 (sequential), VK0192 has **non-sequential segment mapping**:

```
Display Layout:      VK0192 RAM Addresses
┌─────────────┐
│ Digit 1 2 3 │      Digit 1: Addresses 0, 1,  8, 9
│ Signal ●●●● │      Digit 2: Addresses 2, 3, 10, 11
│ Battery  ▓▓ │      Digit 3: Addresses 4, 5, 12, 13
│ Progress ▓▓ │      Symbols: Addresses 14-47 (scattered)
└─────────────┘
```

Each digit spans **non-sequential addresses** (splits between lower and upper halves).

### 4-Bit Addressing

```
Data Format:
Byte = [ Address High (4 bits) | Data (4 bits) ]

Example:
Address 0x12 = 0x1 << 4 | data
             = 0x10 | data
```

### Timing Requirements (Strict)

```
Clock Period:  ≥ 4μs (250 kHz max)
Pulse Width:   Exactly 4μs (critical)
Setup/Hold:     1μs minimum
```

**Important:** Timing must be precise. Use `delayMicroseconds()` carefully:

```cpp
void writeBit(uint8_t bit) {
    digitalWrite(CLK, LOW);
    digitalWrite(DATA, bit ? HIGH : LOW);
    delayMicroseconds(4);   // Exact timing
    digitalWrite(CLK, HIGH);
    delayMicroseconds(4);   // Exact timing
}
```

### RAM Layout and Addressing

```
Byte Address  4-bit Addresses  Content
0             0, 1             Digit 1 upper, Digit 2 upper
1             (data bits)
...
12            24, 25           Signal strength segments
...
23            46, 47           Progress indicator
```

Segment mapping is **display-specific** due to irregular addressing.

### Wiring

Same as HT1621/HT1622 (3 GPIO pins).

### Segment Mapping Challenge

VK0192 requires careful address mapping because digits don't occupy sequential RAM:

```cpp
class SegLCD_VK0192_5SegSigBatProg : public SegDriver_VK0192 {
    void _mapSegmentToAddress(uint8_t digit, uint8_t segment,
                              uint8_t& addr, uint8_t& bit) {
        // Complex mapping: digit 0 spans addresses 0, 1, 8, 9
        if (digit == 0) {
            if (segment < 4) addr = 0;      // Lower nibble
            else            addr = 8;       // Upper nibble
        }
        // ... similar for digits 1, 2, 3, 4
    }
};
```

---

## Quick Comparison

| Feature | PCF85176 | HT1621 | HT1622 | VK0192 |
|---------|----------|--------|--------|--------|
| **Protocol** | I2C | 3-wire | 3-wire | 3-wire |
| **Pins** | 2 (+ power) | 3 | 3 | 3 |
| **RAM** | 40 bytes | 16 bytes | 32 bytes | 24 bytes |
| **Max Digits** | 13 | 6 | 10+ | 5 |
| **Addressing** | Sequential | Sequential | Sequential | Irregular |
| **Timing Critical** | No | Moderate | High | Very High |
| **Integration** | Separate IC | Integrated | Integrated | Integrated |
| **Typical Cost** | $$$ | $$ | $$ | $$ |

---

## Selection Guide

**Choose PCF85176 if:**
- Need many digits (10+)
- Want I2C (only 2 pins needed)
- Have 3.3V/5V available for I2C pull-ups
- Working with pre-made module

**Choose HT1621 if:**
- Need 4-6 digits
- Have GPIO pins available
- Want simple 3-wire protocol
- Module is integrated (no separate IC)

**Choose HT1622 if:**
- Need 10+ digits
- Want 16-segment capability
- Can handle stricter timing
- Module is already purchased

**Choose VK0192 if:**
- Working with specific VK0192 module
- Can handle complex addressing
- Need precise timing control
- Display already purchased/committed

---

## Protocol Comparison: Write Sequence

### PCF85176 (I2C)
```
START → ADDR → CONTROL → [DATA₀-₃₉] → STOP
```
Single transaction, all data at once.

### HT1621 (3-Wire)
```
CS ↓ → CMD(9b) → ADDR(5b) → DATA(8b) → CS ↑
```
Bit-banged protocol, address then data.

### VK0192 (3-Wire)
```
CS ↓ → CMD(9b) → NIBBLE_ADDR(4b) | DATA(4b) → CS ↑
```
4-bit addressing, must repeat for each nibble.

---

## Reference

- PCF85176 Datasheet: NXP Semiconductors
- HT1621/HT1622 Datasheet: Holtek
- VK0192 Datasheet: VK Inc.
- [Getting Started](getting-started.md)
- [Supported LCDs](supported-lcds.md)
- [Architecture](architecture.md)
