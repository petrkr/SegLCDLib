/**
 * @example HT1621-RawLCD.ino
 * @brief Example of RAW LCD display using HT1621
 */

#include "SegLCD_HT1621_Raw.h"

#define PIN_CS 6
#define PIN_WR 4
#define PIN_DATA 7

SegLCD_HT1621_Raw lcd(PIN_CS, PIN_DATA, PIN_WR);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Initialize LCD...");

  // Demo for duty 1/4, bias 1/3 LCD display
  lcd.init(MODE_DRIVE_14, MODE_BIAS_13);

  // Send RAW command
  lcd.sendCommand(CMD_SYS_EN);

  // Send LCD ON implemented command
  lcd.on();

  // Clear LCD
  lcd.clear();

  delay(1000);

  // Write at address 0 one byte
  lcd.write((uint8_t)0xFF, 0);
}

uint8_t i=0;
void loop() {

  lcd.write((uint8_t)0xff, i);

  i+=2;

  if (i>31) {
    i = 0;
  }

  delay(1000);
}
