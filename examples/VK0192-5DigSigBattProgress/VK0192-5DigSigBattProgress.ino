/**
 * @example SegLCD_VK0192_Raw.ino
 * @brief Example of 4 digits with degree symbol LCD display using HT1621
 */

#include "SegLCD_VK0192_5DigSigBattProgress.h"

#define PIN_CS 27
#define PIN_WR 14
#define PIN_DATA 12
#define PIN_PWR 13 // If you have connected Vcc on GPIO instead power line else -1

SegLCD_VK0192_5DigSigBattProgress lcd(PIN_CS, PIN_DATA, PIN_WR);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Initialize LCD...");

  if (PIN_PWR > -1) {
    pinMode(PIN_PWR, OUTPUT);
    digitalWrite(PIN_PWR, HIGH);
  }

  lcd.init();

  // Clear LCD
  lcd.clear();

  //uint8_t _buffer[2] = {0x00, 0x60};
  //lcd.writeRam(_buffer, sizeof(_buffer), 0);
  lcd.writeDigit7seg(0, '1');
  lcd.writeDigit7seg(1, '2');
  lcd.writeDigit7seg(2, '3');
  lcd.writeDigit7seg(3, '4');
  lcd.writeDigit7seg(4, '5');
  lcd.writeDigit7seg(5, '6');
  lcd.writeDigit7seg(6, '7');
  lcd.writeDigit7seg(7, '8');
  lcd.writeDigit7seg(8, '9');
  lcd.writeDigit7seg(9, '0');
}

void loop() {
  delay(5000);
}
