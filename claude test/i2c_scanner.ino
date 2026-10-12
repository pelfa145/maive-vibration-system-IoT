// I2C scanner - run this FIRST with the LCD and MPU-6050 both wired to A4/A5.
// Expect the LCD at 0x27 or 0x3F, and the MPU-6050 at 0x68 (or 0x69).
#include <Wire.h>

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.println("Scanning I2C bus...");

  int found = 0;
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("Device at 0x");
      Serial.println(addr, HEX);
      found++;
    }
  }
  Serial.print("Done. Devices found: ");
  Serial.println(found);
}

void loop() {}
