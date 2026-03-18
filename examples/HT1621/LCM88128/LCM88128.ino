/**
 * @example HT1621/LCM88128/LCM88128.ino
 * @brief Example of LCM88128 LCD display using HT1621
 */

#include "SegLCD_HT1621_LCM88128.h"

#define PIN_CS 9
#define PIN_WR 47
#define PIN_DATA 21

SegLCD_HT1621_LCM88128 lcd(PIN_CS, PIN_DATA, PIN_WR);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Initialize LCD...");

  lcd.init();

  // Clear LCD
  lcd.clear();

  delay(1000);

  lcd.setUnits(SegLCD_HT1621_LCM88128::UNIT_RIGHT_VOLT);
  lcd.setSymbols(SegLCD_HT1621_LCM88128::SYMBOL_LINE_WIND_TO_ACDC);
}

uint8_t i=0;
void loop() {

}
