# Changelog

## v0.0.2-SNAPSHOT
- Note: v0.0.x releases are alpha/prerelease builds. Breaking changes may happen without semantic-versioning major bumps.
- Breaking change: Added Transport layer in order to support other environment than Arduino Wire.h
- Breaking Change: TempHum LCD changed labels C/PERCENT to Units to match other LCDs
- FIX: TempHum: use masked write to prevent other digits when writing units
- FIX: Call clear() from init()
- Transport split: `SegTransport.h` contains abstract transport interfaces, while Arduino transport implementations moved to `SegTransportArduino.h`. Arduino code using `SegTransportI2CArduino` or `SegTransport3WireArduino` must include `SegTransportArduino.h` explicitly. This is part of the v0.0.2 transport breaking change.
- Added supported build macro `SEGLCD_DISABLE_ARDUINO_TRANSPORT` for environments that provide their own transport and do not expose Arduino `Wire.h`.
- Added LCD model build selection with `SEGLCD_DISABLE_ALL_LCDS` and per-model `SEGLCD_ENABLE_*` macros. Documented the concrete enable macro for each LCD model.
- Improve project structure and documentation
- Updated documentation
- Added CI tests
- Added Github templates, first preview version
- Added Xygax SEG_I2C LCD support


## v0.0.1
- Initial library release
- Abstract API for segment LCD displays
- Base support for extensible controller and display implementations
