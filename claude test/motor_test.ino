// L298N + DC motor test: speed steps, then reverse.
// Wiring: ENA -> D5 (PWM, remove the ENA jumper), IN1 -> D7, IN2 -> D8.
// Keep the motor free-spinning for this test.

const int ENA = 5;
const int IN1 = 7;
const int IN2 = 8;

const int SPEEDS[] = {25, 50, 75, 100};
const int SPEED_COUNT = sizeof(SPEEDS) / sizeof(SPEEDS[0]);

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  stopMotor();

  Serial.begin(9600);
  Serial.println("Motor test start");
}

void loop() {
  // Forward, stepping up. If it only hums at 25%, that's below the
  // motor's stall duty (normal), not a wiring problem.
  for (int i = 0; i < SPEED_COUNT; i++) {
    Serial.print("Forward: ");
    Serial.print(SPEEDS[i]);
    Serial.println("%");
    setMotor(SPEEDS[i], true);
    delay(3000);
  }

  // Always stop before changing direction
  Serial.println("Stop");
  stopMotor();
  delay(2000);

  Serial.println("Reverse: 50%");
  setMotor(50, false);
  delay(3000);

  Serial.println("Stop");
  stopMotor();
  delay(2000);
}

void setMotor(int percent, bool forward) {
  percent = constrain(percent, 0, 100);
  digitalWrite(IN1, forward ? HIGH : LOW);
  digitalWrite(IN2, forward ? LOW : HIGH);
  analogWrite(ENA, map(percent, 0, 100, 0, 255));
}

void stopMotor() {
  analogWrite(ENA, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}
