//L298N and dc motor test

//assuming naka connect ang motor sa OUT1/OUT2 sa L298N and ginagamit ung ENA for PWM speed control.


const int ENA = 5;
const int IN1 = 7;
const int IN2 = 8;

void setup(){
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);


    stopMotor();

    Serial.begin(9600);
    Serial.println("Motor test start");
}

void loop(){
    Serial.println("Motor: 25%");
    setMotorSpeed(25);
    delay(3000);

    Serial.println("Motor: 50%");
    setMotorSpeed(50);
    delay(3000);

    Serial.println("Motor: 75%");
    setMotorSpeed(75);
    delay(3000);

    Serial.println("Motor: 100%");
    setMotorSpeed(100);
    delay(3000);

    Serial.println("Motor: stop");
    stopMotor();
    delay(3000);

}
//functions

void setMotorSpeed(int percentage){
    percentage = constrain(percentage, 0, 100);

    int pwm = map(percentage, 0, 100, 0, 255);

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    analogWrite(ENA, pwm);

}
void stopMotor(){
    analogWrite(ENA, 0);

    digitalWrite(IN1, LOW);
    digitalWrite(IN1, LOW);
}