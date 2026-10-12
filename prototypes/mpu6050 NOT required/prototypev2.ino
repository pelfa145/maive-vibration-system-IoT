
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>

//maize project badum tsk
LiquidCrystal_I2C lcd(0x27, 16, 2);

// initialize vibration sensor pero optional pa
Adafruit_MPU6050 mpu;
bool mpuAvailable = false;

// pin assignments
const int POT_PIN = A0;
const int MOTOR_PWM_PIN = 5;

// motor speed settings; this is what determines kung how strong each level
const int LEVEL_1_PWM = 64;
const int LEVEL_2_PWM = 128;
const int LEVEL_3_PWM = 255;

// current operating state
int currentLevel = 0;
int currentPWM = 0;
int potValue = 0;

float accelerationMagnitude = 0.0;

// timing
unsigned long lastLCDUpdate = 0;
unsigned long lastSensorUpdate = 0;
unsigned long lastSerialUpdate = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  pinMode(MOTOR_PWM_PIN, OUTPUT);

  // ensure motor is off during startup
  analogWrite(MOTOR_PWM_PIN, 0);

  // initialize lcd display
  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Maize Vibration");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");

  // try initializing the vibration sensor
  // tutuloy ung program kung d ma initialize or mahanap ung vibration sensor
  mpuAvailable = mpu.begin();

  if (mpuAvailable) {
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    Serial.println("MPU-6050 detected.");
  } else {
    Serial.println("MPU-6050 absent.");
    Serial.println("Continuing without sensor.");
  }

  delay(1000);
  lcd.clear();

  Serial.println("Maize vibration system ready.");
}

void loop() {
  unsigned long now = millis();

//POTENTIOMETER READ!! TAGA BASA SA INPUT
  potValue = analogRead(POT_PIN);

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

  // control motor driver
  analogWrite(MOTOR_PWM_PIN, currentPWM);

// read mpu6050 if meron
  if (mpuAvailable &&
      now - lastSensorUpdate >= 100) {

    lastSensorUpdate = now;

    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;

    mpu.getEvent(&accel, &gyro, &temp);

    //asked ai for help dto, apparently para sa mpu6050 measuring stuff including the math stuff
    //idk clarify ko lang later
    accelerationMagnitude = sqrt(
      accel.acceleration.x * accel.acceleration.x +
      accel.acceleration.y * accel.acceleration.y +
      accel.acceleration.z * accel.acceleration.z
    );
  }

// update lcd if state changes
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
      lcd.print(" ");
    }

    lcd.setCursor(0, 1);

    if (mpuAvailable) {
      lcd.print("Accel:");
      lcd.print(accelerationMagnitude, 1);
      lcd.print(" m/s2  ");
    } else {
      lcd.print("MPU: Not plugged");
    }
  }

  //serial monitor diagnostics, set baud or serial to 9600
  if (now - lastSerialUpdate >= 500) {
    lastSerialUpdate = now;

    Serial.print("Potentiometer: ");
    Serial.print(potValue);

    Serial.print(" | Level: ");
    Serial.print(currentLevel);

    Serial.print(" | PWM: ");
    Serial.print(currentPWM);

    Serial.print(" | MPU-6050: ");

    if (mpuAvailable) {
      Serial.print(accelerationMagnitude, 2);
      Serial.println(" m/s^2");
    } else {
      Serial.println("Unavailable");
    }
  }
}



