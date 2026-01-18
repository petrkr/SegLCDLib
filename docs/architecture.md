# SegLCDLib Architecture

## Overview

SegLCDLib uses a layered architecture separating concerns between abstract interface, controller protocol handling, and display-specific implementations.

```
┌─────────────────────────────────────────────────────┐
│  User Application (Arduino Sketch)                  │
└──────────────────┬──────────────────────────────────┘
                   │
┌──────────────────▼──────────────────────────────────┐
│  SegLCDLib Base Class (SegLCDLib : public Print)    │
│  - Abstract interface (pure virtual methods)         │
│  - LCD API 1.0 spec compliance                       │
│  - Cursor positioning (row, col)                     │
└──────────────────┬──────────────────────────────────┘
                   │
┌──────────────────▼──────────────────────────────────┐
│  Controller Driver Layer                            │
│  ├─ SegDriver_PCx85  (I2C)                          │
│  ├─ SegDriver_HT1621 (3-wire serial)                │
│  ├─ SegDriver_HT1622 (3-wire serial)                │
│  └─ SegDriver_VK0192 (3-wire serial)                │
└──────────────────┬──────────────────────────────────┘
                   │
┌──────────────────▼──────────────────────────────────┐
│  LCD Implementation Layer                           │
│  ├─ SegLCD_PCF85176_TempHum                         │
│  ├─ SegLCD_PCF85176_6DigSigBattProgress             │
│  ├─ SegLCD_HT1621_4SegDegree                        │
│  └─ ... (one class per display variant)             │
└──────────────────┬──────────────────────────────────┘
                   │
┌──────────────────▼──────────────────────────────────┐
│  Hardware Communication                             │
│  ├─ I2C Bus (Wire library for PCF85176)             │
│  └─ 3-Wire Serial (GPIO for HT1621/1622/VK0192)     │
└─────────────────────────────────────────────────────┘
```

---

## Layer 1: Abstract Base Class

**File:** `src/SegLCDLib.h`

Defines the common interface for all LCD implementations:

```cpp
class SegLCDLib : public Print {
    virtual void init() = 0;          // Initialize display
    virtual void clear() = 0;         // Turn off all segments
    virtual void on() = 0;            // Display on
    virtual void off() = 0;           // Display off
    virtual void setCursor(uint8_t row, uint8_t col) = 0;
    virtual size_t write(uint8_t ch) = 0;  // Print character
    virtual void command(uint8_t cmd) = 0; // Raw command
};
```

### Key Features

- **Inherits from `Print`**: Enables `print()`, `println()`, `write()` from Arduino standard library
- **LCD API 1.0 Spec**: Follows Arduino's LCD API specification (https://playground.arduino.cc/Code/LCDAPI/)
- **Cursor System**: Row/column addressing for positioning output
  - Row 0: First display row (for simple LCDs, the only row)
  - Column: Digit position within that row
  - Example: T1T2 LCD has Row 0 (clock), Row 1 (T1 temps), Row 2 (T2 temps)

### Enumerations

```cpp
enum ModeDrive {        // Display refresh pattern
    MODE_DRIVE_STATIC,  // 1/1 duty (single digit, rare)
    MODE_DRIVE_12,      // 1/2 multiplex (2 digits)
    MODE_DRIVE_13,      // 1/3 multiplex (3 digits)
    MODE_DRIVE_14       // 1/4 multiplex (4 digits)
};

enum ModeBias {         // Voltage bias (sets contrast)
    MODE_BIAS_13,       // 1/3 bias
    MODE_BIAS_12        // 1/2 bias
};

enum BacklightMode {    // GPIO backlight control
    BACKLIGHT_DIGITAL,  // On/off only
    BACKLIGHT_PWM       // PWM brightness (0-255)
};
```

---

## Layer 2: Controller Drivers

Controllers handle protocol-level communication with LCD hardware. Each driver abstracts a specific protocol.

### PCF85176 / PCF8576 (I2C)

**File:** `src/SegDriver_PCx85.h`, `src/SegDriver_PCF85176.h`

```cpp
class SegDriver_PCx85 : public SegLCDLib {
    // I2C address configuration
    uint8_t _address;    // 0x38 + address pins (A0-A2)

    // RAM buffering (minimize I2C transactions)
    uint8_t _ramBuffer[40];  // PCF85176 has 39 bytes RAM

    // Control methods
    void _writeRam();     // Send buffer to controller
    void _writeAddrRam(); // Write with address masking
};
```

**Features:**
- I2C protocol (2 pins: SDA, SCL)
- 40-byte RAM for segment data
- Address configurable via A0-A2 pins (0x38-0x3F range)
- Multiplexed display support (1/3 or 1/4 duty)
- Bias and drive mode configuration

**Subclass Chain:**
- `SegDriver_PCx85`: Base with generic configuration
- `SegDriver_PCF85176`: Inherits, sets PCF85176 specifics
- `SegDriver_PCF8576`: Thin wrapper (nearly identical to PCF85176)

---

### HT1621 / HT1622 (3-Wire Serial)

**Files:** `src/SegDriver_HT1621.h`, `src/SegDriver_HT1622.h`

```cpp
class SegDriver_HT1621 : public SegLCDLib {
    uint8_t _pinCLK;    // Clock pin (GPIO)
    uint8_t _pinDATA;   // Data pin (GPIO)
    uint8_t _pinCS;     // Chip select pin (GPIO)

    // RAM buffering
    uint8_t _ramBuffer[16];  // HT1621 has 16 bytes RAM

    // 3-wire serial protocol methods
    void _writeCommand(uint8_t cmd);
    void _writeData(uint16_t addr, uint8_t data);
    void _writeBit(uint8_t bit);
};
```

**Features:**
- 3-wire serial protocol: Clock, Data, Chip Select
- 16-byte RAM (HT1621) or 32-byte RAM (HT1622)
- Configurable BIAS and DRIVE modes
- Integrated into LCD module (no separate IC on board)
- GPIO-based (no special hardware needed)

**Protocol:**
1. Set CS low
2. Send 9-bit command/address
3. Send data bits MSB first
4. Set CS high

---

### VK0192 (3-Wire Serial, Advanced)

**File:** `src/SegDriver_VK0192.h`

```cpp
class SegDriver_VK0192 : public SegLCDLib {
    // Same 3-wire pins as HT1621
    uint8_t _pinCLK, _pinDATA, _pinCS;

    // 24 bytes × 8 bits RAM (48 4-bit addresses)
    uint8_t _ramBuffer[24];

    // Irregular addressing (non-sequential segment mapping)
    void _mapSegmentToAddress(uint8_t digit, uint8_t segment,
                              uint8_t& addr, uint8_t& bit);
};
```

**Features:**
- 24×8 bit RAM (48 4-bit addresses 0-47)
- Irregular segment mapping (digits/segments not sequential in memory)
- 4μs minimum pulse width timing
- More complex segment addressing than HT1621/HT1622

---

## Layer 3: LCD Implementations

LCD classes combine a controller driver with display-specific segment mapping and symbols.

### Design Pattern: LCD Inherits from Controller

Unlike typical composition patterns, LCD classes inherit directly from controller drivers:

```cpp
class SegLCD_PCF85176_TempHum : public SegDriver_PCF85176 {
    // Display-specific methods
    virtual size_t write(uint8_t ch);  // Output digit

    // Symbol control
    void setTempSymbol(bool on);
    void setHumiditySymbol(bool on);

private:
    // Segment-to-RAM address mapping
    void _mapSegments(uint8_t digit, uint8_t segments);
};
```

**Rationale:**
- Controllers are always **integrated into the LCD module** (not separate components)
- Single instance serves both controller and display logic
- Reduces memory overhead (no separate controller instance)
- Simplifies user code (one object to manage)

### Structure of LCD Class

```cpp
class SegLCD_PCF85176_6DigSigBattProgress : public SegDriver_PCF85176 {
public:
    // Constructor
    SegLCD_PCF85176_6DigSigBattProgress(uint8_t i2cAddress = 0x38);

    // Base methods (pure virtual implementations)
    void init();                              // Init controller + clear
    size_t write(uint8_t ch);                 // Output 7-segment char

    // Feature methods (display-specific)
    void setSignal(uint8_t bars);             // 0-4 bars
    void setBattery(uint8_t level);           // 0-3 levels
    void setProgress(uint8_t percent);        // 0-100%

private:
    // Segment mapping: maps digit position → RAM addresses/bits
    void _mapSegments(uint8_t digit, uint8_t segments);

    // 7-segment character map
    static const uint8_t _charMap[256];

    // Display layout constants
    static const uint8_t MAX_DIGITS = 6;
};
```

---

## Cursor and Positioning System

SegLCDLib uses row/column addressing inherited from LCD API:

```cpp
setCursor(row, col);    // Move output position
write(character);       // Output at current position, advance
```

### Row/Column Semantics

For most displays (single row):
- `setCursor(0, 0)`: Start of display
- `setCursor(0, 1)`: 2nd digit
- `setCursor(1, x)`: Invalid (most displays have no row 1)

For multi-row displays (e.g., T1T2 LCD):
- `setCursor(0, 0)`: Clock display, digit 1
- `setCursor(1, 0)`: T1 temperature, digit 1
- `setCursor(2, 0)`: T2 temperature, digit 1

---

## Memory Management

### RAM Buffering

All drivers maintain an in-memory buffer mirroring controller RAM:

```cpp
uint8_t _ramBuffer[40];  // PCF85176 has 39 bytes

// Update display: modify buffer, then write to hardware
_ramBuffer[0] |= 0x01;    // Set segment bit
_writeRam();              // Send entire buffer to controller
```

**Benefits:**
- Minimize I2C/3-wire transactions (one bulk write vs. multiple small writes)
- Support masked writes (update specific bits without full byte)
- Reduce latency (batch updates before hardware sync)

### Segment Mapping

Each LCD class implements `_mapSegments()` to convert logical segments to physical RAM addresses:

```cpp
// Example: Turn on segments for digit '5'
// _mapSegments(0, 0b0110111);  // RAM address 0, bits 0-6

// Internal mapping:
_ramBuffer[addressForDigit0] |= bitsFor5;
_writeRam();
```

Mapping varies by:
- **Controller**: Different RAM layouts (PCF vs HT vs VK)
- **Display**: Segment wiring (which output pin connects to which segment)

---

## Initialization Flow

When user calls `lcd.init()`:

```cpp
1. Controller initialization (BIAS, DRIVE modes)
2. Enable display (turn on voltage supply)
3. Clear RAM buffer (all zeros)
4. Write buffer to hardware
5. Reset cursor to (0, 0)
```

---

## Adding New Displays

### Step 1: Identify Controller
- **I2C?** → Use PCF85176/PCF8576 driver
- **3-wire?** → Use HT1621/HT1622/VK0192 driver

### Step 2: Create LCD Class
```cpp
// File: src/SegLCD_PCF85176_MyDisplay.h
class SegLCD_PCF85176_MyDisplay : public SegDriver_PCF85176 {
    virtual size_t write(uint8_t ch);
    void _mapSegments(uint8_t digit, uint8_t segments);
};
```

### Step 3: Implement Segment Mapping
- Use RAW LCD variant to test physical wiring
- Record which output pins light which segments
- Implement `_mapSegments()` to convert logical→physical

### Step 4: Add Feature Methods
- Display-specific symbols (battery, signal, degree, etc.)
- Implement `write()` to output characters correctly

See [Adding New LCD](adding-new-lcd.md) for detailed tutorial.

---

## Datasheet References

- **PCF85176**: I2C 40×8 segment LCD controller
- **HT1621/1622**: 3-wire serial LCD drivers
- **VK0192**: 3-wire LCD driver with irregular addressing

---

## Key Design Decisions

| Decision | Rationale |
|----------|-----------|
| **LCD inherits controller** | Controllers integrated in LCD modules, not external |
| **RAM buffering** | Minimize I2C/serial transactions, batch updates |
| **Row/column addressing** | Follow Arduino LCD API standard for consistency |
| **Print inheritance** | Enable standard `print()` and `println()` operations |
| **Base driver per protocol** | Reduce code duplication (PCF, HT1621, HT1622, VK0192 each once) |

---

## Related Documentation

- [Getting Started](getting-started.md): Quick start guide
- [Supported LCDs](supported-lcds.md): LCD catalog with examples
- [Controllers](controllers.md): Protocol and hardware details
- [Adding New LCD](adding-new-lcd.md): Tutorial for custom displays
