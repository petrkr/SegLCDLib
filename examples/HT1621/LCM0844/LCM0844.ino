/**
 * @example HT1621/LCM0844/LCM0844.ino
 * @brief Example of LCM0844 LCD display using HT1621
 */

#include "SegLCD_HT1621_LCM0844.h"

#define PIN_CS 9
#define PIN_WR 47
#define PIN_DATA 21

SegTransport3WireArduino transport(PIN_DATA, PIN_WR);
SegLCD_HT1621_LCM0844 lcd(transport, PIN_CS);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Initialize LCD...");

  lcd.init();

  // Clear LCD
  lcd.clear();

  delay(1000);

  lcd.setLabels(SegLCD_HT1621_LCM0844::LABEL_BATT_AGM);
  lcd.setSymbols(SegLCD_HT1621_LCM0844::SYMBOL_MUTE);

  delay(2000);
  lcd.clearLabels(SegLCD_HT1621_LCM0844::LABEL_BATT_AGM);
  lcd.clearSymbols(SegLCD_HT1621_LCM0844::SYMBOL_MUTE);

  for (int i=0; i<=5; i++)
  {
    lcd.setLoadLevel(i);
    delay(500);
  }

  // Write at address 0 one byte
  //lcd.writeRam((uint8_t)0xFF, 0);
  lcd.print("1.2.3456.7.8");
}

uint8_t i=0;
void loop() {

}
