//B10K potentiometer reading test

const int POT_PIN = A0;

void setup(){
    Serial.begin(9600);

    Serial.println("Potentiometer test started.");
}

void loop(){
    int rawValue = analogRead(POT_PIN);

    int percentage = map(rawValue, 0, 1023, 0, 100);

    Serial.print("Raw: ");
    Serial.print(rawValue);
    Serial.print(" | Percentage: ");
    Serial.print(percentage);
    Serial.println("%");
    
    delay(200);
}


// expected results        raw values
// knob is @ minimum -> approx. 0
// knob is @ middle -> approx. 512
// knob is @ maxiomum -> approx. 1023 

