#!/usr/bin/env python3
"""Generates test/host/compile_all.generated.cpp from src/SegLCD_*.h.

Instantiates every concrete SegLCD_* class against no-op transports and
exercises write(uint8_t)/write(const char*)/write(const char*, size_t) on
each, so a class that hides SegLCDLib's write() overloads (missing
`using SegLCDLib::write;`) fails to compile instead of silently slipping
through. Run this before compiling test/host/compile_all.generated.cpp.
"""

import re
import sys
from pathlib import Path

SRC_DIR = Path(__file__).resolve().parent.parent.parent / "src"
OUT_FILE = Path(__file__).resolve().parent / "compile_all.generated.cpp"

CLASS_RE = re.compile(r"^class\s+(\w+)\s*:\s*public\s+\w+", re.MULTILINE)
CTOR_3WIRE_RE = re.compile(r"^\s*(\w+)\s*\(\s*SegTransport3Wire\s*&\s*\w+\s*,\s*uint8_t\s+\w+\s*\)\s*;")
CTOR_I2C_RE = re.compile(r"^\s*(\w+)\s*\(\s*SegTransportI2C\s*&\s*\w+")


def parse_header(path: Path):
    text = path.read_text()
    class_match = CLASS_RE.search(text)
    if not class_match:
        return None
    cls = class_match.group(1)

    for line in text.splitlines():
        m3 = CTOR_3WIRE_RE.match(line)
        if m3 and m3.group(1) == cls:
            return (cls, path.name, "3wire")
        mi2c = CTOR_I2C_RE.match(line)
        if mi2c and mi2c.group(1) == cls:
            return (cls, path.name, "i2c")

    return None


def main():
    headers = sorted(SRC_DIR.glob("SegLCD_*.h"))
    entries = []
    skipped = []

    for header in headers:
        result = parse_header(header)
        if result is None:
            skipped.append(header.name)
            continue
        entries.append(result)

    if skipped:
        print("ERROR: could not determine constructor for:", file=sys.stderr)
        for name in skipped:
            print(f"  - {name}", file=sys.stderr)
        print(
            "Expected exactly one constructor of the form "
            "'ClassName(SegTransport3Wire& transport, uint8_t chipselect)' or "
            "'ClassName(SegTransportI2C& transport, ...)'. "
            "Update generate_compile_all.py if a new constructor shape was introduced.",
            file=sys.stderr,
        )
        sys.exit(1)

    lines = []
    lines.append("// GENERATED FILE - do not edit by hand.")
    lines.append("// Regenerate with: python3 test/host/generate_compile_all.py")
    lines.append('#include "noop_mocks.h"')
    for _, header, _ in entries:
        lines.append(f'#include "{header[:-2]}.h"')
    lines.append("")
    lines.append("template <typename T>")
    lines.append("static void exerciseWrite(T& lcd) {")
    lines.append("    lcd.write((uint8_t)'A');")
    lines.append('    lcd.write("test");')
    lines.append('    lcd.write("test", 4);')
    lines.append("}")
    lines.append("")
    lines.append("int main() {")
    lines.append("    NoopTransport3Wire t3;")
    lines.append("    NoopTransportI2C ti2c;")
    lines.append("")
    for i, (cls, _, kind) in enumerate(entries):
        transport = "t3, 5" if kind == "3wire" else "ti2c"
        lines.append(f"    {{ {cls} lcd{i}({transport}); exerciseWrite(lcd{i}); }}")
    lines.append("")
    lines.append("    return 0;")
    lines.append("}")
    lines.append("")

    OUT_FILE.write_text("\n".join(lines))
    print(f"Generated {OUT_FILE} covering {len(entries)} LCD classes.")


if __name__ == "__main__":
    main()
