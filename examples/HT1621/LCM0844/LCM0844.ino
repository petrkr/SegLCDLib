/**
 * @example HT1621/RawLCD/RawLCD.ino
 * @brief Example of RAW LCD display using HT1621
 */

#include "SegLCD_HT1621_LCM0844.h"

#define PIN_CS 9
#define PIN_WR 47
#define PIN_DATA 21

SegLCD_HT1621_LCM0844 lcd(PIN_CS, PIN_DATA, PIN_WR);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Initialize LCD...");

  lcd.init();

  // Clear LCD
  lcd.clear();

  delay(1000);

  for (int i=0; i<=5; i++)
  {
    lcd.setBatteryLevel(i);
    delay(1000);
  }

  // Write at address 0 one byte
  //lcd.writeRam((uint8_t)0xFF, 0);
  lcd.print("1.2.3456.7.8");
}

uint8_t i=0;
void loop() {

}
