#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd1(0x27, 20, 4);

void setup() {
  // put your setup code here, to run once:
  lcd1.init();
  lcd1.clear();
  lcd1.backlight();
  lcd1.setCursor(0,0);
  lcd1.print("Check");
}

void loop() {
  // put your main code here, to run repeatedly
}
