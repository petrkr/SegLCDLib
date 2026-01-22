# SegLCDLib MenuControl

Pluginable menu control system for SegLCDLib displays. Supports multiple LCD types with configurable pins and persistent settings on ESP32.

## Features

- **Plugin-based architecture**: Easy to add new LCD types
- **Zero boilerplate**: Plugins only define display-specific logic (~50 lines each)
- **Persistent configuration**: Save/load settings on ESP32 using Preferences
- **Common interface**: All displays support standard LCD operations
- **Interactive settings**: Configure pins and LCD type via serial menu

## Supported Displays

- **T1T2**: PCF85176 I2C controller with temperature/humidity labels
- **VK0192**: 5-digit display with signal/battery/progress indicators

## Usage

### First Time Setup

1. Upload to board (ESP32 recommended for config persistence)
2. Open serial monitor (115200 baud)
3. Entering settings mode automatically
4. Use `help` to see available commands

```
> help
Settings:
  list   - show available displays
  select - select display
  bus <i2c|3wire> - select bus type
  pins   - configure pins
  save   - save config
  init   - initialize LCD
  exit   - return to active mode
```

### Configuration Flow

```
> list              # Show available displays
> bus i2c          # Select I2C bus
> select           # Choose display type (e.g., T1T2)
> pins             # Configure pins
  SDA pin: 21
  SCL pin: 22
  I2C Address: 38
  Backlight pin: -1
> save             # Save to Preferences
> init             # Initialize LCD
> exit             # Exit settings mode
```

### Active Mode

Once initialized, use display-specific commands:

```
> help             # Show all commands
> c                # Clear display
> p hello          # Print text
> w 0 0 Test       # Write at row 0, col 0
> cur 1 2          # Set cursor to row 1, col 2

# T1T2-specific:
> b 3              # Set battery to level 3
> g 4              # Set signal to level 4
> colon 1          # Enable colon symbol
> d 0 2 1          # Set decimal point at row 0, col 2

# VK0192-specific:
> b 2              # Set battery to level 2
> prog 75          # Set progress bar to 75
> ls 0x0001        # Set labels (P label)
```

### Returning to Settings

```
> settings         # Exit active mode, return to settings
```

## Architecture

### Plugin Interface (LCDPlugin.h)

Each plugin must implement:

```cpp
class LCDPlugin {
public:
    virtual const char *name() const = 0;
    virtual SegLCDLib* create(const DisplayConfig &cfg) = 0;
    virtual void destroy(SegLCDLib *lcd) = 0;
    virtual bool handleCommand(SegLCDLib *lcd, const char *cmd, char *args, Stream &out) = 0;
    virtual void printMenu(Stream &out) = 0;
    virtual bool hasDecimal() const { return false; }
    virtual bool hasColon() const { return false; }
};
```

### Configuration Structure

```cpp
struct DisplayConfig {
    uint8_t version = 1;
    uint8_t displayId = 0;    // Which plugin (0=T1T2, 1=VK0192, ...)
    uint8_t busType = BUS_I2C; // I2C or 3-WIRE
    int8_t sda = -1, scl = -1; // I2C pins
    int8_t cs = -1, wr = -1, data = -1; // 3-WIRE pins
    int8_t backlight = -1;
    uint8_t i2cAddr = 0x38, subAddr = 0x00;
};
```

### Adding a New Plugin

1. Create `YourDisplayPlugin.h` in same directory
2. Extend `LCDPlugin` class
3. Implement `create()`, `destroy()`, `handleCommand()`, `printMenu()`
4. Add to plugin registry in `MenuControl.cpp`:

```cpp
static YourDisplayPlugin pluginYourDisplay;
static LCDPlugin *plugins[] = { &pluginT1T2, &pluginVK0192, &pluginYourDisplay };
```

## File Structure

```
MenuControl/
├── MenuControl.ino           # Wrapper (setup/loop)
├── MenuControl.h             # Declarations
├── MenuControl.cpp           # Core implementation
├── LCDPlugin.h               # Plugin interface + helpers
├── T1T2Plugin.h              # T1T2 implementation
├── VK0192Plugin.h            # VK0192 implementation
└── README.md                 # This file
```

## Design Benefits

1. **Minimal code**: Each plugin ~50 lines (vs. 150+ in old UniversalMenuControl)
2. **No boilerplate**: Plugins only define LCD-specific logic
3. **Direct calls**: No virtual wrapper overhead
4. **Persistence**: Works on ESP32 with built-in storage
5. **Clean interface**: Settings menu adapts to available plugins

## Notes

- Config persists on ESP32 using Preferences API
- Non-ESP32 boards fall back to settings-mode-only
- All LCD commands defer to active plugin after common command dispatch
- Decimal/Colon support detected via plugin capabilities
