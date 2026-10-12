
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>

//arduino uno + mosfet

// pin assignments
const int POT_PIN = A0;
const int MOTOR_PWM_PIN = 5;


LiquidCrystal_I2C lcd(0x27, 16, 2);


Adafruit_MPU6050 mpu;

// PWM settings: initial values for testing
const int LEVEL_1_PWM = 64;
const int LEVEL_2_PWM = 128;
const int LEVEL_3_PWM = 255;

// potentiometer thresholds
const int OFF_THRESHOLD = 4;
const int LEVEL_1_MAX = 340;
const int LEVEL_2_MAX = 682;

// update intervals
const unsigned long SENSOR_INTERVAL = 100;
const unsigned long DISPLAY_INTERVAL = 250;
const unsigned long SERIAL_INTERVAL = 500;

unsigned long lastSensorRead = 0;
unsigned long lastDisplayUpdate = 0;
unsigned long lastSerialUpdate = 0;

// system state
int potValue = 0;
int selectedLevel = 0;  // 0 = OFF
int currentPWM = 0;

float accelX = 0.0;
float accelY = 0.0;
float accelZ = 0.0;
float accelerationMagnitude = 0.0;



void setup() {
    pinMode(MOTOR_PWM_PIN, OUTPUT);

    // motor starts at off when initiating 
    analogWrite(MOTOR_PWM_PIN, 0);

    Serial.begin(9600);
    Wire.begin();

    lcd.init();
    lcd.backlight();
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Maize Vibration");
    lcd.setCursor(0, 1);
    lcd.print("Initializing...");

    delay(1000);

    // initialize accelerometer
    if (!mpu.begin()) {
        analogWrite(MOTOR_PWM_PIN, 0);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("MPU6050 ERROR");
        lcd.setCursor(0, 1);
        lcd.print("Motor disabled");

        Serial.println("ERROR: MPU6050 not detected.");
        Serial.println("Motor remains OFF.");

        // fail safe
        while (true) {
            analogWrite(MOTOR_PWM_PIN, 0);
            delay(100);
        }
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("System Ready");
    lcd.setCursor(0, 1);
    lcd.print("Motor OFF");

    Serial.println("Maize Vibration System");
    Serial.println("Turn knob to select a level.");
    Serial.println("Minimum position = OFF.");

    delay(1000);
}



void loop() {
    unsigned long now = millis();

    readPotentiometer();
    updateMotor();

    if (now - lastSensorRead >= SENSOR_INTERVAL) {
        lastSensorRead = now;
        readAccelerometer();
    }

    if (now - lastDisplayUpdate >= DISPLAY_INTERVAL) {
        lastDisplayUpdate = now;
        updateLCD();
    }

    if (now - lastSerialUpdate >= SERIAL_INTERVAL) {
        lastSerialUpdate = now;
        printDiagnostics();
    }
}


void readPotentiometer() {
    potValue = analogRead(POT_PIN);

    if (potValue <= OFF_THRESHOLD) {
        selectedLevel = 0;
    }
    else if (potValue <= LEVEL_1_MAX) {
        selectedLevel = 1;
    }
    else if (potValue <= LEVEL_2_MAX) {
        selectedLevel = 2;
    }
    else {
        selectedLevel = 3;
    }
}


void updateMotor() {
    switch (selectedLevel) {
        case 0:
            currentPWM = 0;
            break;

        case 1:
            currentPWM = LEVEL_1_PWM;
            break;

        case 2:
            currentPWM = LEVEL_2_PWM;
            break;

        case 3:
            currentPWM = LEVEL_3_PWM;
            break;

        default:
            currentPWM = 0;
            break;
    }

    analogWrite(MOTOR_PWM_PIN, currentPWM);
}


void readAccelerometer() {
    sensors_event_t acceleration;
    sensors_event_t rotation;
    sensors_event_t temperature;

    mpu.getEvent(
        &acceleration,
        &rotation,
        &temperature
    );

    accelX = acceleration.acceleration.x;
    accelY = acceleration.acceleration.y;
    accelZ = acceleration.acceleration.z;

    accelerationMagnitude = sqrt(
        accelX * accelX +
        accelY * accelY +
        accelZ * accelZ
    );
}


void updateLCD() {
    lcd.setCursor(0, 0);

    if (selectedLevel == 0) {
        lcd.print("Motor: OFF      ");
    }
    else {
        lcd.print("Level: ");
        lcd.print(selectedLevel);
        lcd.print(" PWM:");
        lcd.print(currentPWM);
        lcd.print("   ");
    }

    lcd.setCursor(0, 1);
    lcd.print("Accel:");
    lcd.print(accelerationMagnitude, 1);
    lcd.print(" m/s2  ");
}


void printDiagnostics() {
    Serial.print("Pot: ");
    Serial.print(potValue);

    Serial.print(" | Level: ");
    Serial.print(selectedLevel == 0 ? "OFF" :
                 selectedLevel == 1 ? "LOW" :
                 selectedLevel == 2 ? "MEDIUM" : "HIGH");

    Serial.print(" | PWM: ");
    Serial.print(currentPWM);

    Serial.print(" | Accel X: ");
    Serial.print(accelX, 2);

    Serial.print(" Y: ");
    Serial.print(accelY, 2);

    Serial.print(" Z: ");
    Serial.print(accelZ, 2);

    Serial.print(" | Magnitude: ");
    Serial.print(accelerationMagnitude, 2);

    Serial.println(" m/s^2");
}
