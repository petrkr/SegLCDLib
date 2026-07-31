/**
 * @example PCF85134/XygaxSegI2C/XygaxSegI2C.ino
 * @brief Example of Xygax SEG_I2C LCD display using PCF85134
 */

#include "SegTransportArduino.h"
#include "SegLCD_PCF85134_XygaxSegI2C.h"

#include <Wire.h>

SegTransportI2CArduino bus(Wire);
SegLCD_PCF85134_XygaxSegI2C lcd(bus);

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
  lcd.init();

  // Clear LCD
  lcd.clear();

  delay(1000);

  // Write at address 2 one byte
  lcd.print("123.4");
}


void loop() {

}
