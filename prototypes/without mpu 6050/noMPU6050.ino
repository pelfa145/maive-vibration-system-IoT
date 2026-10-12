
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Optional MPU-6050
Adafruit_MPU6050 mpu;
bool mpuAvailable = false;

// Pins
const int POT_PIN = A0;
const int MOTOR_PWM_PIN = 5;

// PWM levels
const int LEVEL_1_PWM = 64;
const int LEVEL_2_PWM = 128;
const int LEVEL_3_PWM = 255;

int currentLevel = 0;
int currentPWM = 0;

unsigned long lastLCDUpdate = 0;
unsigned long lastSensorUpdate = 0;
unsigned long lastSerialUpdate = 0;

float accelerationMagnitude = 0.0;

void setup() {
  pinMode(MOTOR_PWM_PIN, OUTPUT);
  analogWrite(MOTOR_PWM_PIN, 0);

  Serial.begin(9600);
  Wire.begin();

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Maize Vibration");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");

  // Detect the MPU, but do not stop if it is absent.
  mpuAvailable = mpu.begin();

  if (mpuAvailable) {
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.println("MPU-6050 detected.");
  } else {
    Serial.println("MPU-6050 not detected. Test mode.");
  }

  delay(1200);
  lcd.clear();
}

void loop() {
  unsigned long now = millis();

  // Read the potentiometer and choose a motor level.
  int potValue = analogRead(POT_PIN);

  if (potValue <= 4) {
    currentLevel = 0;
    currentPWM = 0;
  } else if (potValue <= 340) {
    currentLevel = 1;
    currentPWM = LEVEL_1_PWM;
  } else if (potValue <= 682) {
    currentLevel = 2;
    currentPWM = LEVEL_2_PWM;
  } else {
    currentLevel = 3;
    currentPWM = LEVEL_3_PWM;
  }

  // Update motor control.
  analogWrite(MOTOR_PWM_PIN, currentPWM);

  // Read acceleration only when the MPU is available.
  if (mpuAvailable && now - lastSensorUpdate >= 100) {
    lastSensorUpdate = now;

    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);

    accelerationMagnitude = sqrt(
      accel.acceleration.x * accel.acceleration.x +
      accel.acceleration.y * accel.acceleration.y +
      accel.acceleration.z * accel.acceleration.z
    );
  }

  // Update the LCD.
  if (now - lastLCDUpdate >= 250) {
    lastLCDUpdate = now;

    lcd.setCursor(0, 0);

    if (currentLevel == 0) {
      lcd.print("Motor: OFF      ");
    } else {
      lcd.print("Level: ");
      lcd.print(currentLevel);
      lcd.print(" PWM:");
      lcd.print(currentPWM);
      lcd.print("   ");
    }

    lcd.setCursor(0, 1);

    if (mpuAvailable) {
      lcd.print("Accel:");
      lcd.print(accelerationMagnitude, 1);
      lcd.print(" m/s2 ");
    } else {
      lcd.print("MPU: Not plugged");
    }
  }

  // Serial diagnostics.
  if (now - lastSerialUpdate >= 500) {
    lastSerialUpdate = now;

    Serial.print("Pot=");
    Serial.print(potValue);
    Serial.print(" | Level=");
    Serial.print(currentLevel);
    Serial.print(" | PWM=");
    Serial.print(currentPWM);
    Serial.print(" | MPU=");

    if (mpuAvailable) {
      Serial.print(accelerationMagnitude);
      Serial.println(" m/s^2");
    } else {
      Serial.println("Unavailable");
    }
  }
}
