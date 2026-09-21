//MPU6050 and gyroscope test
//need nyo muna download or import tong mga libraries na to sa arduino uno before nyo magamit
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void setup() {
    Serial.begin(9600);

    if (!mpu.begin()) {
        Serial.println("MPU6050 not found.");
        
        while (1) {
            delay(10);
        }
    }

    Serial.println("MPU6050 found.");

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    delay(100);
}

void loop() {
    sensors_event_t acceleration;
    sensors_event_t gyro;
    sensors_event_t temperature;

    mpu.getEvent(&acceleration, &gyro, &temperature);

    Serial.print("Acceleration X: ");
    Serial.print(acceleration.acceleration.x);

    Serial.print(" | Y: ");
    Serial.print(acceleration.acceleration.y);

    Serial.print(" | Z: ");
    Serial.print(acceleration.acceleration.z);

    Serial.println(" m/s^2");

    delay(100);
}

/* ano gustong result? 

basta may results or output, kasi ung ano dito is if nbakaka pag communicate ung arduino
with the mpu-6050, tas ano itsura sa sensor data.