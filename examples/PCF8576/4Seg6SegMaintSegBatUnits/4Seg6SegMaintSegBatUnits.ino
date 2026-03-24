/**
 * @example PCF8576/4Seg6SegMaintSegBatUnits/4Seg6SegMaintSegBatUnits.ino
 * @brief Example for 4+6 digit LCD with maintenance, battery, signal, units
 */

#include "SegLCD_PCF8576_4Seg6SegMaintSegBatUnits.h"
#include "SegTransport.h"
#include <Wire.h>

SegTransportI2CArduino bus(Wire);
SegLCD_PCF8576_4Seg6SegMaintSegBatUnits lcd(bus);

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("Initialize i2c");
  #if defined(ARDUINO_ARCH_AVR)
    Wire.begin();
  #else
    Wire.begin(1, 2);
  #endif

  Serial.println("Initialize backlight");
  lcd.initBacklight(10);
  lcd.setBacklight(true);

  Serial.println("Initialize LCD");
  lcd.init();
  lcd.clear();

  Serial.println("Demo");
  lcd.setSignalLevel(3);
  lcd.setBatteryLevel(2);
  lcd.setMaintenanceSymbol(true);
  lcd.setDegreeSymbol(true);

  lcd.setCursor(0, 0);
  lcd.print("17:37");

  delay(1000);

  lcd.setCursor(0, 0);
  lcd.print("18");

  delay(1000);

  lcd.setCursor(1, 2);
  lcd.print("17.37");

  delay(1000);

  lcd.setCursor(1, 2);
  lcd.print("18");

  delay(1000);

  lcd.setCursor(1, 0);
  lcd.print("T1:-3.41");


  //lcd.setAutoFlush(false);    // Disable autoflush

}

static unsigned long lastUpdate = 0;
static float value = 123.4;

void loop() {
   if (millis() - lastUpdate >= 100) {
       lastUpdate = millis();

       lcd.setCursor(0, 0);
       lcd.print(value);

       value += 0.1;
       if (value > 999.9) value = 0.0;
   }
}
