# Changelog

## v0.0.2-SNAPSHOT
- Breaking change: Added Transport layer in order to support other environment than Arduino Wire.h
- Transport split: `SegTransport.h` contains abstract transport interfaces, while Arduino transport implementations moved to `SegTransportArduino.h`. Arduino code using `SegTransportI2CArduino` or `SegTransport3WireArduino` must include `SegTransportArduino.h` explicitly. This is part of the v0.0.2 transport breaking change.
- Breaking Change: TempHum LCD changed labels C/PERCENT to Units to match other LCDs
- FIX: TempHum: use masked write to prevent other digits when writing units
- Improve project structure and documentation
- Updated documentation
- Added CI tests
- Added Github templates, first preview version
- FIX: Call clear() from init()


## v0.0.1
- Initial library release
- Abstract API for segment LCD displays
- Base support for extensible controller and display implementations
