// B10K potentiometer test.
// Expected: knob at minimum ~ 0, middle ~ 512, maximum ~ 1023.

const int POT_PIN = A0;

void setup() {
  Serial.begin(9600);
  Serial.println("Potentiometer test started.");
}

void loop() {
  int raw = analogRead(POT_PIN);
  int percent = map(raw, 0, 1023, 0, 100);

  Serial.print("Raw: ");
  Serial.print(raw);
  Serial.print(" | Percentage: ");
  Serial.print(percent);
  Serial.println("%");

  delay(200);
}
