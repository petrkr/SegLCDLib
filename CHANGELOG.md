# Changelog

## v0.0.2-SNAPSHOT
- Note: v0.0.x releases are alpha/prerelease builds. Breaking changes may happen without semantic-versioning major bumps.
- Migration notes: [v0.0.1 to v0.0.2](docs/migration.md#v001-to-v002)
- Breaking change: Added Transport layer in order to support other environment than Arduino Wire.h
- Breaking change: LCD constructors now take `SegTransportI2C` or `SegTransport3Wire` instead of Arduino `Wire` or direct GPIO pins.
- Breaking change: `initBacklight()` now takes a `SegBacklight*` implementation instead of a raw GPIO pin number, mode, and active-high flag. Arduino users pass a `SegBacklightArduino` instance from `SegBacklightArduino.h`. This also introduces the new `SegBacklight` abstract interface (implemented by `SegBacklightArduino`, with methods `setDigital()`/`setBrightness()`).
- Added support for non-Arduino frameworks (ESP-IDF, RP2040 SDK, STM32...): `SegLCDLib` no longer requires the Arduino framework outside of Arduino builds, and gains a `write(const char*)` / `write(const char*, size_t)` API available identically everywhere.
- Breaking change: measurement label APIs were renamed from `setLabels`/`clearLabels` and `LABEL_*` to `setUnits`/`clearUnits` and `UNIT_*` on affected LCDs.
- Breaking change: `SegLCD_PCx85_Raw` was split into `SegLCD_PCF85176_Raw` and `SegLCD_PCF85134_Raw`.
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
- Internal cleanup: unified PCx85 default I2C address constant as `DEFAULT_PCX85_I2C_ADDRESS`


## v0.0.1
- Initial library release
- Abstract API for segment LCD displays
- Base support for extensible controller and display implementations
