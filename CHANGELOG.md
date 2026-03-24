# Changelog

## v0.0.2-SNAPSHOT
- Breaking change: Added Transport layer in order to support other environment than Arduino Wire.h
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
