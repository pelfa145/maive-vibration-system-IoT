// Integrated prototype: pot sets motor speed, LCD shows speed + MPU-6050 data.
// No delay() in loop(), everything runs on millis() timers.
// Assumption: the pot controls speed (forward only). Adjust to the real spec.

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "config.h"

LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);
Adafruit_MPU6050 mpu;

bool mpuOk = false;
int speedPercent = 0;
float accelZ = 0.0;

unsigned long lastSensor = 0;
unsigned long lastLcd = 0;
unsigned long lastSerial = 0;

void setup() {
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  stopMotor();

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  mpuOk = mpu.begin();
  if (!mpuOk) {
    Serial.println("MPU6050 not found.");
    lcd.setCursor(0, 0);
    lcd.print("MPU not found");
    delay(1500);  // fine here, setup only runs once
    lcd.clear();
  }
}

void loop() {
  unsigned long now = millis();

  if (now - lastSensor >= SENSOR_INTERVAL_MS) {
    lastSensor = now;
    readPot();
    readMpu();
    updateMotor();
  }

  if (now - lastLcd >= LCD_INTERVAL_MS) {
    lastLcd = now;
    updateLcd();
  }

  if (now - lastSerial >= SERIAL_INTERVAL_MS) {
    lastSerial = now;
    logSerial();
  }
}

// ---- inputs ----

void readPot() {
  speedPercent = map(analogRead(PIN_POT), 0, 1023, 0, 100);
}

void readMpu() {
  if (!mpuOk) return;
  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);
  accelZ = accel.acceleration.z;
}

// ---- outputs ----

void updateMotor() {
  if (speedPercent < MIN_RUN_PERCENT) {
    stopMotor();
  } else {
    setMotorForward(speedPercent);
  }
}

void setMotorForward(int percent) {
  percent = constrain(percent, 0, 100);
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  analogWrite(PIN_ENA, map(percent, 0, 100, 0, 255));
}

void stopMotor() {
  analogWrite(PIN_ENA, 0);
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
}

void updateLcd() {
  // Overwrite with trailing spaces instead of lcd.clear() to avoid flicker
  lcd.setCursor(0, 0);
  lcd.print("Speed: ");
  lcd.print(speedPercent < MIN_RUN_PERCENT ? 0 : speedPercent);
  lcd.print("%    ");

  lcd.setCursor(0, 1);
  if (mpuOk) {
    lcd.print("Az: ");
    lcd.print(accelZ, 1);
    lcd.print("      ");
  } else {
    lcd.print("MPU offline     ");
  }
}

void logSerial() {
  Serial.print("Speed: ");
  Serial.print(speedPercent);
  Serial.print("% | AccelZ: ");
  Serial.println(accelZ);
}
