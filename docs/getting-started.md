# Getting Started with SegLCDLib

## Requirements

- **Arduino IDE**: Version 1.8.13 or later
- **Hardware**: Arduino-compatible microcontroller (AVR, ARM, ESP32, etc.)
- **Wire Library**: Built-in; used for I2C controllers (included with Arduino IDE)
- **Display Module**: One of the [supported LCD displays](supported-lcds.md)

## Installation

### Option 1: Arduino Library Manager (Recommended)
1. Open Arduino IDE
2. Go to **Sketch** → **Include Library** → **Manage Libraries...**
3. Search for `SegLCDLib`
4. Click **Install**

### Option 2: Manual Installation
1. Download or clone: https://github.com/petrkr/SegLCDLib
2. Copy the `SegLCDLib` folder to your Arduino libraries folder:
   - **Windows**: `Documents\Arduino\libraries`
   - **macOS**: `~/Documents/Arduino/libraries`
   - **Linux**: `~/Arduino/libraries`
3. Restart Arduino IDE

## First Project: 4-Digit Display with Degree Symbol (HT1621)

This example uses the **4-digit LCD with degree symbol** (integrated HT1621 controller). This is a ready-made module commonly found on AliExpress, no soldering needed.

### Wiring

Connect 3 GPIO pins and power to your Arduino:

| Module Pin | Arduino Pin |
|-----------|-------------|
| CLK | Pin 5 |
| DATA | Pin 6 |
| CS | Pin 7 |
| VCC | 3.3V |
| GND | GND |

**Note:** Adjust pins 5, 6, 7 if they conflict with other components.

### Code

```cpp
#include "SegLCD_HT1621_4SegDegree.h"

// Create 3-wire transport (DATA, WR) and LCD (CS)
SegTransport3WireArduino transport(6, 5);
SegLCD_HT1621_4SegDegree lcd(transport, 7);

void setup() {
  lcd.init();           // Initialize display
}

void loop() {
  int temp = 23;        // Temperature in Celsius

  lcd.setCursor(0, 0);  // Move to first digit
  lcd.print(temp);      // Print 23 on display

  lcd.showDegree();     // Show degree symbol (°)

  delay(1000);
}
```

### Running

1. Select your board: **Tools** → **Board** → Choose your Arduino
2. Select COM port: **Tools** → **Port**
3. Upload: **Sketch** → **Upload** (Ctrl+U)
4. Display should show "23°" with degree symbol

## Choosing Your LCD Display

Use this table to find the right display for your project:

| Your Need | Controller | LCD Module | Wiring |
|-----------|-----------|-----------|---------|
| **Simplest** | PCF85176 | RAW LCD | I2C only |
| **Show digits** | PCF85176 | 6-digit with battery/signal | I2C, 4 pins |
| **Temperature** | PCF85176 | Temp/Humidity | I2C, 4 pins |
| **Small display** | HT1621 | 4-digit with degree | 3 pins |
| **Many digits** | HT1621 | 6-digit with battery | 3 pins |
| **16-segment** | HT1622 | 10-digit 16-segment | 3 pins |
| **Advanced** | VK0192 | 5-digit with battery | 3 pins |

For detailed specifications and purchase links, see [Supported LCDs](supported-lcds.md).

## Basic API Reference

### Display Control

```cpp
lcd.init();           // Initialize display and clear
lcd.clear();          // Turn off all segments
lcd.on();             // Turn display on
lcd.off();            // Turn display off
lcd.home();           // Set cursor to 0,0
```

### Positioning and Output

```cpp
lcd.setCursor(row, col);  // Move cursor to row/column
lcd.print(value);         // Output number or string
lcd.write(byte);          // Output single character/segment byte
```

### Features (display-dependent)

```cpp
lcd.setBatteryLevel(level);  // If supported by the display
lcd.setSignalLevel(bars);    // If supported by the display
lcd.setDegreeSymbol(true);   // If supported by the display
```

See [Architecture](architecture.md) for class hierarchy and full [API Reference](https://petrkr.github.io/SegLCDLib/) for complete method documentation.

## Common Issues

**Display not showing:**
- Verify I2C address: Use example sketch `PCF85176/RawLCD` to scan addresses
- Check wiring: SDA/SCL for I2C or DATA/WR/CS for 3-wire, plus VCC/GND
- Confirm board selection and COM port in Arduino IDE

**Wrong I2C address:**
- PCF85176 I2C address is set by SA0 pin: 0x38 (SA0=0) or 0x39 (SA0=1)
- A0-A2 are subaddresses in the protocol, not I2C address pins
- HT1621/HT1622/VK0192 use 3-wire serial (no I2C address)

**Library not found:**
- Restart Arduino IDE after installation
- Verify installation path: Check `Arduino/libraries/SegLCDLib/` exists

## Next Steps

- Explore [examples](https://github.com/petrkr/SegLCDLib/tree/master/examples) for your LCD type
- Read [Architecture](architecture.md) to understand design
- See [Adding New LCD](adding-new-lcd.md) to create custom display support
- Full [API Reference](https://petrkr.github.io/SegLCDLib/) for all available methods

## Resources

- **GitHub**: https://github.com/petrkr/SegLCDLib
- **Issues/Support**: https://github.com/petrkr/SegLCDLib/issues
- **LCD API Spec**: https://playground.arduino.cc/Code/LCDAPI/
