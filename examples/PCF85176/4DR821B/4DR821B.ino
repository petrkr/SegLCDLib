/**
 * @example PCF85176/4DR821B/4DR821B.ino
 * @brief Example of Tesla 4DR821B LCD display using PCF85176
 */

#include "SegLCD_PCF85176_4DR821B.h"

#include <Wire.h>

SegLCD_PCF85176_4DR821B lcd(Wire);

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

  lcd.init();
}


void loop() {
  Serial.println("Testing clock colon");
  lcd.clear();
  lcd.print("12.54");
  delay(1000);

  lcd.setCursor(0, 2);
  lcd.print("23");
  delay(1000);

  Serial.println("Testing decimals");
  lcd.clear();
  lcd.print("1.24");
  delay(1000);

  lcd.home();
  lcd.print("12.4");
  delay(1000);

  lcd.home();
  lcd.print("124.");
  delay(1000);

  Serial.println("Testing column 0 symbols");
  lcd.clear();
  lcd.print("-1234");
  delay(1000);

  lcd.clear();
  lcd.print("+1234");
  delay(1000);

  lcd.home();
  lcd.print(":1234");
  delay(1000);

  lcd.clear();
  lcd.print("+12:34");
  delay(1000);

  Serial.println("Testing symbols");
  lcd.setSymbol(SegLCD_PCF85176_4DR821B::SymbolFlags::SYMBOL_ARROW, true);
  delay(1000);

  lcd.setSymbol(SegLCD_PCF85176_4DR821B::SymbolFlags::SYMBOL_TILDA, true);
  delay(1000);

  lcd.setSymbol(SegLCD_PCF85176_4DR821B::SymbolFlags::SYMBOL_ARROW, false);
  delay(1000);

  lcd.setSymbol(SegLCD_PCF85176_4DR821B::SymbolFlags::SYMBOL_TILDA, false);
  delay(1000);
}
