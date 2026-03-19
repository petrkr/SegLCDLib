/**
 * @example HT1621/LCM59011/LCM59011.ino
 * @brief Quick example of LCM59011 LCD display using HT1621
 */

#include "SegLCD_HT1621_LCM59011.h"

#define PIN_CS 9
#define PIN_WR 47
#define PIN_DATA 21

SegLCD_HT1621_LCM59011 lcd(PIN_CS, PIN_DATA, PIN_WR);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Initialize LCD...");

  lcd.init();
  lcd.clear();

  lcd.print("12:3");
  lcd.setSignalLevel(4);
  lcd.setBatteryLevel(5);
  lcd.setLabels(SegLCD_HT1621_LCM59011::LABEL_MCB);
  lcd.setUnits(SegLCD_HT1621_LCM59011::UNIT_PERCENT);
  lcd.setSymbols(
    SegLCD_HT1621_LCM59011::SYMBOL_ECO |
    SegLCD_HT1621_LCM59011::SYMBOL_BLUETOOTH |
    SegLCD_HT1621_LCM59011::SYMBOL_OUTLINE
  );
}

void loop() {
}
