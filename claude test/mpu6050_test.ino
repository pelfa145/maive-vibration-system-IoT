// MPU-6050 test: accelerometer + gyro + temperature.
// Libraries: "Adafruit MPU6050" and "Adafruit Unified Sensor" (Library Manager).
// Pass = "MPU6050 found." and values that change when you tilt/rotate the board.
// Flat on the table: accel Z ~ 9.8 m/s^2, gyro ~ 0 rad/s.

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(9600);

  if (!mpu.begin()) {
    Serial.println("MPU6050 not found. Check VCC/GND/SDA/SCL.");
    while (1) delay(10);
  }
  Serial.println("MPU6050 found.");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(100);
}

void loop() {
  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

  Serial.print("Accel (m/s^2) X: ");
  Serial.print(accel.acceleration.x);
  Serial.print(" Y: ");
  Serial.print(accel.acceleration.y);
  Serial.print(" Z: ");
  Serial.print(accel.acceleration.z);

  Serial.print(" | Gyro (rad/s) X: ");
  Serial.print(gyro.gyro.x);
  Serial.print(" Y: ");
  Serial.print(gyro.gyro.y);
  Serial.print(" Z: ");
  Serial.print(gyro.gyro.z);

  Serial.print(" | Temp: ");
  Serial.print(temp.temperature);
  Serial.println(" C");

  delay(100);
}
