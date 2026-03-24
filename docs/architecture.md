# SegLCDLib Architecture

## Overview

SegLCDLib uses a layered architecture separating concerns between abstract interface, transport, controller protocol handling, and display-specific implementations.

```
┌─────────────────────────────────────────────────────┐
│  User Application (Arduino Sketch)                  │
└──────────────────┬──────────────────────────────────┘
                   │
┌──────────────────▼──────────────────────────────────┐
│  LCD Implementation Layer                           │
│  ├─ SegLCD_PCF85176_TempHumidity                    │
│  ├─ SegLCD_PCF85176_6DigitSignalBatteryProgress     │
│  ├─ SegLCD_HT1621_4SegDegree                        │
│  └─ ... (one class per display variant)             │
└──────────────────┬──────────────────────────────────┘
                   │ inherits
┌──────────────────▼──────────────────────────────────┐
│  SegLCDLib Base Class (SegLCDLib : public Print)    │
│  - Abstract interface (pure virtual methods)        │
│  - LCD API 1.0 spec compliance                      │
│  - Cursor positioning (row, col)                    │
└──────────────────┬──────────────────────────────────┘
                   │ base of drivers
┌──────────────────▼──────────────────────────────────┐
│  Controller Driver Layer                            │
│  ├─ SegDriver_PCx85  (I2C via transport)            │
│  ├─ SegDriver_HT1621 (3-wire via transport)         │
│  ├─ SegDriver_HT1622 (3-wire via transport)         │
│  └─ SegDriver_VK0192 (3-wire via transport)         │
└──────────────────┬──────────────────────────────────┘
                   │ uses
┌──────────────────▼──────────────────────────────────┐
│  Transport Layer                                    │
│  ├─ SegTransportI2C / SegTransportI2CArduino        │
│  └─ SegTransport3Wire / SegTransport3WireArduino    │
└──────────────────┬──────────────────────────────────┘
                   │ wraps
┌──────────────────▼──────────────────────────────────┐
│  Hardware Communication                             │
│  ├─ Wire / TwoWire bus                              │
│  └─ GPIO pins for DATA / WR / CS                    │
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

## Layer 2: Transport Layer

Transports encapsulate low-level bus access and are injected into controller drivers.

### I2C Transport

**File:** `src/SegTransport.h`

```cpp
class SegTransportI2C : public SegTransport {
    virtual void write(uint8_t address, uint8_t data) = 0;
    virtual void write(uint8_t address, uint8_t* data, size_t length) = 0;
};

class SegTransportI2CArduino : public SegTransportI2C {
    SegTransportI2CArduino(TwoWire& i2c);
};
```

### 3-Wire Transport

**File:** `src/SegTransport.h`

```cpp
class SegTransport3Wire : public SegTransport {
    virtual void set_cs(uint8_t chipselect, bool state) = 0;
    virtual void write(uint16_t data, uint8_t bitCount) = 0;
};

class SegTransport3WireArduino : public SegTransport3Wire {
    SegTransport3WireArduino(uint8_t data, uint8_t write, uint8_t read = -1);
};
```

---

## Layer 3: Controller Drivers

Controllers handle protocol-level communication with LCD hardware. Each driver abstracts a specific protocol.

### PCF85176 / PCF8576 (I2C)

**File:** `src/SegDriver_PCx85.h`, `src/SegDriver_PCF85176.h`

```cpp
class SegDriver_PCx85 : public SegLCDLib {
    SegTransportI2C& _transport;

    // I2C address configuration
    uint8_t _address;      // 0x38 (SA0=0) or 0x39 (SA0=1)
    uint8_t _subaddress;   // A0-A2 bits (protocol subaddress)

    // RAM buffering is inherited from SegLCDLib
    // Concrete LCD class allocates required size via _allocateBuffer()
    uint8_t* _ramBuffer;
    size_t _ramBufferSize;

    // Control methods
    void _writeRam();     // Send buffer to controller
    void _writeAddrRam(); // Write with address masking
};
```

**Features:**
- I2C protocol (2 pins: SDA, SCL)
- Uses shared dynamic RAM buffer from `SegLCDLib`
- Sends I2C transactions through `SegTransportI2C`
- I2C address set by SA0 pin (0x38 or 0x39)
- A0-A2 are protocol subaddresses (not I2C address pins)
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
    SegTransport3Wire& _transport;  // 3-wire bus transport
    uint8_t _cs;                    // Chip select pin (GPIO)

    // RAM buffering is inherited from SegLCDLib
    // Concrete LCD class allocates required size via _allocateBuffer()
    uint8_t* _ramBuffer;
    size_t _ramBufferSize;

    // 3-wire serial protocol methods via transport
    void command(uint8_t cmd);
    void _writeRam(uint8_t* data, size_t length, uint8_t address);
};
```

**Features:**
- 3-wire serial protocol: Clock, Data, Chip Select
- Uses shared dynamic RAM buffer from `SegLCDLib`
- Sends DATA/WR/CS signaling through `SegTransport3Wire`
- Typical controller RAM capacity is 16 bytes (HT1621) or 32 bytes (HT1622), but concrete LCD classes may allocate only the used portion
- Configurable BIAS and DRIVE modes
- Integrated into LCD module (no separate IC on board)
- No hardware peripheral required beyond GPIO pins

**Protocol:**
1. Assert CS through transport
2. Send command/address prefix
3. Send data bits MSB first through transport
4. Release CS through transport

---

### VK0192 (3-Wire Serial, Advanced)

**File:** `src/SegDriver_VK0192.h`

```cpp
class SegDriver_VK0192 : public SegLCDLib {
    // Same 3-wire transport model as HT1621
    SegTransport3Wire& _transport;
    uint8_t _cs;

    // RAM buffering is inherited from SegLCDLib
    // Concrete LCD class allocates required size via _allocateBuffer()
    uint8_t* _ramBuffer;
    size_t _ramBufferSize;

    // Same transport and RAM write model as HT1621/HT1622
};
```

**Features:**
- Uses shared dynamic RAM buffer from `SegLCDLib`
- Sends 3-wire signaling through `SegTransport3Wire`
- VK0192 address space is 24×8 bit RAM (48 4-bit addresses 0-47)
- Irregular segment mapping (digits/segments not sequential in memory)
- 4μs minimum pulse width timing
- More complex segment addressing than HT1621/HT1622

---

## Layer 4: LCD Implementations

LCD classes combine a controller driver with display-specific segment mapping and symbols.

### Design Pattern: LCD Inherits from Controller

Unlike typical composition patterns, LCD classes inherit directly from controller drivers:

```cpp
class SegLCD_PCF85176_TempHumidity : public SegDriver_PCF85176 {
    // Display-specific methods
    virtual size_t write(uint8_t ch);  // Output digit
};
```

**Rationale:**
- Controllers are always **integrated into the LCD module** (not separate components)
- Single instance serves both controller and display logic
- Reduces memory overhead (no separate controller instance)
- Simplifies user code (one object to manage)

### Structure of LCD Class

```cpp
class SegLCD_PCF85176_6DigitSignalBatteryProgress : public SegDriver_PCF85176 {
public:
    // Constructor
    SegLCD_PCF85176_6DigitSignalBatteryProgress(SegTransportI2C& transport, uint8_t address = 0x38, uint8_t subaddress = 0);

    // Base methods (pure virtual implementations)
    void init();                              // Init controller + clear
    size_t write(uint8_t ch);                 // Output 7-segment char

    // Feature methods (display-specific)
    void setSignalLevel(uint8_t bars);        // 0-4 bars
    void setBatteryLevel(uint8_t level);      // 0-3 levels
    void setProgress(uint8_t percent);        // 0-100%

private:
    // Segment mapping: maps digit position → RAM addresses/bits
    void _mapSegments(uint8_t digit, uint8_t segments);

    // 7-segment character map
    static const uint8_t _charMap[256];

    // Display layout constants
    static const uint8_t DIGITS = 6;
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
uint8_t* _ramBuffer = nullptr;
size_t _ramBufferSize = 0;

// Concrete LCD class allocates exact size it needs
_allocateBuffer(RAM_SIZE);

// Update display: modify cached RAM, then sync affected byte(s)
_ramBuffer[0] |= 0x01;
_writeRam(_ramBuffer[0], 0);
```

**Benefits:**
- Minimize I2C/3-wire transactions (one bulk write vs. multiple small writes)
- Support masked writes (update specific bits without full byte)
- Reduce latency (batch updates before hardware sync)

### Segment Mapping

Each LCD class implements `_mapSegments()` to convert logical segments to physical RAM addresses:

```cpp
// Example: Turn on segments for digit '5'
// _mapSegments(0b0110111);  // logical segment set for one digit

// Internal mapping:
_ramBuffer[addressForDigit0] |= bitsFor5;
_writeRam(_ramBuffer[addressForDigit0], hwAddressForDigit0);
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
- **I2C?** → Use PCF85176/PCF8576 driver with `SegTransportI2C`
- **3-wire?** → Use HT1621/HT1622/VK0192 driver with `SegTransport3Wire`

### Step 2: Create LCD Class
```cpp
// File: src/SegLCD_PCF85176_MyDisplay.h
class SegLCD_PCF85176_MyDisplay : public SegDriver_PCF85176 {
    virtual size_t write(uint8_t ch);
    void _mapSegments(uint8_t digit, uint8_t segments);
};
```

Instantiate with transport from user code:

```cpp
SegTransportI2CArduino transport(Wire);
SegLCD_PCF85176_MyDisplay lcd(transport);
```

For 3-wire displays:

```cpp
SegTransport3WireArduino transport(dataPin, wrPin);
SegLCD_HT1621_MyDisplay lcd(transport, csPin);
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
| **Transport injected into drivers** | Separate bus access from controller protocol and ease Arduino/non-Arduino backends |
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
