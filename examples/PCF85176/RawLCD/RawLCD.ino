/**
 * @example PCF85176/RawLCD/RawLCD.ino
 * @brief Example of RAW LCD display using PCx85
 */

#include "SegLCD_PCx85_Raw.h"
#include "SegTransport.h"

#include <Wire.h>

SegTransportI2CArduino bus(Wire);
SegLCD_PCx85_Raw lcd(bus);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Initilalize I2C...");
  #if defined(ARDUINO_ARCH_AVR)
    Wire.begin();
  #else
    Wire.begin(1, 2);
  #endif

  Serial.println("Initialize LCD...");

  // Demo for static one segment LCD display
  lcd.init(MODE_DRIVE_14, MODE_BIAS_13);

  // Clear LCD
  lcd.clear();

  delay(1000);

  // Write at address 0 one byte
  lcd.writeRam((uint8_t)0b10000000, 0);

  delay(1000);

  // Write at address 8 one byte
  lcd.writeRam((uint8_t)0b01000000, 2);

  // Write at address 8 one byte
  lcd.writeRam((uint8_t)0b01000000, 4);

  // Write at address 8 one byte
  lcd.writeRam((uint8_t)0b01000000, 6);

  delay(1000);

  // Write few bytes at address 0
  uint8_t data[2] = { 0b11111111, 0b11111111 };
  lcd.writeRam(data, sizeof(data), 0);
}


void loop() {

}
