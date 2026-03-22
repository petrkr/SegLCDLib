## Summary

<!-- Short description of the change -->

## Change Type

- [ ] New LCD implementation
- [ ] New controller/driver support
- [ ] Bug fix
- [ ] Refactor/cleanup
- [ ] Documentation/examples only

## New LCD Checklist

Complete only if this PR adds a new `SegLCD_*` class.

- [ ] Controller type is identified and stated in the PR description
- [ ] Segment mapping was verified on real hardware
- [ ] `src/SegLCD_[CONTROLLER]_[DISPLAY].h` was added
- [ ] `src/SegLCD_[CONTROLLER]_[DISPLAY].cpp` was added
- [ ] Example sketch was added under `examples/`
- [ ] Documentation was updated (`docs/adding-new-lcd.md` or `docs/supported-lcds.md`)
- [ ] PR description includes what was actually tested on hardware

## New Driver Checklist

Complete only if this PR adds or changes a low-level `SegDriver_*` implementation.

- [ ] Init sequence and RAM addressing were validated
- [ ] RAW access or another minimal validation path exists
- [ ] At least one example or integration path uses the driver
- [ ] Any protocol-specific limitations are documented

## Hardware Tested

<!-- Example: HT1621 + LCM0844 on ESP32-C3, real hardware -->

## Notes

<!-- Anything reviewers should focus on -->
