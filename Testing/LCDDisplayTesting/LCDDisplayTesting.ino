#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd1(0x27, 16, 2);

bool display = true;

void setup() {
  // put your setup code here, to run once:
  lcd1.init();
  lcd1.clear();
  lcd1.backlight();

}

void loop() {
  // put your main code here, to run repeatedly
}
