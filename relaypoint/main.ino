struct sensorRelayPair {
  int relayPin;
  int sensorPin;
  bool state;
  unsigned long updated;
};

sensorRelayPair controls[] = {
  { 52, 53, HIGH, 0}
};

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  for (int i = 0; i < (sizeof(controls) / sizeof(controls[0])); i++) {
    Serial.print("Setting pin mode of ");
    Serial.print(controls[i].sensorPin);
    Serial.print(" as a sensor and ");
    Serial.print(controls[i].relayPin);
    Serial.println(" as relay");
    pinMode(controls[i].sensorPin, INPUT);
    pinMode(controls[i].relayPin, OUTPUT);
    digitalWrite(controls[i].relayPin, HIGH);
  }
}

void burstRelay(int pin, bool state) {
  Serial.print("Bursting relay ");
  Serial.println(pin);
  digitalWrite(pin, !state);
  delay(200);
  digitalWrite(pin, state);
}

void loop() {
  unsigned long t = millis();
  for (int i = 0; i < (sizeof(controls) / sizeof(controls[0])); i++) {
    bool state = digitalRead(controls[i].sensorPin);
    if (state != controls[i].state) {
      if (t - controls[i].updated >= 10000) {
        Serial.print("As state was not matched, bursting relay to state ");
        Serial.println(HIGH ? state == true : LOW);
        burstRelay(controls[i].relayPin, controls[i].state);
        controls[i].updated = millis();
      } else {
        Serial.print("Time was ");
        Serial.print(t);
        Serial.print(" and updated was ");
        Serial.print(controls[i].updated);
        Serial.println(", so not firing");
      }
    }
  }
delay(10);
}