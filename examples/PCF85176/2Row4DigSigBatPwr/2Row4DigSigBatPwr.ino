/**
 * @example PCF85176/RawLCD/RawLCD.ino
 * @brief Example of RAW LCD display using PCx85
 */

#include "SegLCD_PCF85176_2Row4DigSigBatPwr.h"

#include <Wire.h>

SegLCD_PCF85176_2Row4DigSigBatPwr lcd(Wire);

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

  // Write at address 0 one byte
  lcd.print("1.2.34");
  lcd.print("123.4");

  lcd.setSignalLevel(1);
  lcd.setBatteryLevel(2);
  lcd.setPowerSymbol(true);
}


void loop() {

}
