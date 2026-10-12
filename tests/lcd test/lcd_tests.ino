// 04_lcd_test.ino
// 16x2 I2C LCD test

//samthing here, need i donwlaod ung mga libraries na to

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    lcd.init();
    lcd.backlight();

    lcd.setCursor(0, 0);
    lcd.print("system");

    lcd.setCursor(0, 1);
    lcd.print("lcd test?? okay");
}
//basta may output galing sa arduino oks na
void loop() {
}