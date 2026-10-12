// 16x2 I2C LCD test.
// Libraries: "LiquidCrystal I2C" (install via Library Manager).
// Blank screen? Run i2c_scanner first, then change LCD_ADDR (0x27 or 0x3F),
// and try turning the contrast screw on the backpack.

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const uint8_t LCD_ADDR = 0x27;
LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);

void setup() {
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("LCD test");
  lcd.setCursor(0, 1);
  lcd.print("Display OK");
}

void loop() {}
