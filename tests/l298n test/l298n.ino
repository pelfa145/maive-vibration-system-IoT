// 05_l298n_test.ino
// L298N direction-control test

const int ENA = 5;
const int IN1 = 7;
const int IN2 = 8;

void setup() {
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);

    Serial.begin(9600);

    stopMotor();

    Serial.println("L298N test started.");
}

void loop() {
    Serial.println("Direction: FORWARD");

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 128);

    delay(3000);

    Serial.println("STOP");

    stopMotor();

    delay(2000);

    Serial.println("Direction: REVERSE");

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, 128);

    delay(3000);

    Serial.println("STOP");

    stopMotor();

    delay(2000);
}

void stopMotor() {
    analogWrite(ENA, 0);

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
}