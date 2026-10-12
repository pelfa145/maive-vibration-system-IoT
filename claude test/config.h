// Shared settings for the integrated prototype. Change pins/addresses here only.
#pragma once

// L298N
const int PIN_ENA = 5;   // PWM
const int PIN_IN1 = 7;
const int PIN_IN2 = 8;

// Potentiometer
const int PIN_POT = A0;

// I2C LCD (confirm with i2c_scanner)
const uint8_t LCD_ADDR = 0x27;

// Below this duty the motor usually just hums, so treat it as "off"
const int MIN_RUN_PERCENT = 15;

// Non-blocking timing (ms)
const unsigned long SENSOR_INTERVAL_MS = 50;
const unsigned long LCD_INTERVAL_MS    = 250;
const unsigned long SERIAL_INTERVAL_MS = 500;
