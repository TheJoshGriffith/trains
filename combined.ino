#include <Encoder.h>

struct speedController {
  int controllerToggle1; // pin to control the output polarity of the L298N
  int controllerToggle2; // pin to control the output polarity of the L298N
  int pwm; // pin to control the duty cycle of the L298N
  int encoderRight; // pin connected to the clockwise output of the encoder
  int encoderLeft; // pin connected to the anticlockwise output of the encoder
  long state;
  Encoder enc;
};

struct sensorRelayPair {
  int relayPin; // pin to control the relay
  int sensorPin; // pin to acquire data from the sensor
  bool state;
  unsigned long updated;
};

speedController speedControllers[] = {
  {2, 3, 9, 18, 19, -999, Encoder(18, 19)},
  {4, 5, 10, 16, 17, -999, Encoder(16, 17)},
  {6, 7, 11, 14, 15, -999, Encoder(14, 15)}
};

sensorRelayPair controls[] = {
  { 52, 53, HIGH, 0}
};

void setup() {
  // Communications
  Serial.begin(9600);
  
  // Controllers
  for (int i = 0; i < (sizeof(speedControllers) / sizeof(speedControllers[0])); i++) {
    pinMode(speedControllers[i].pwm, OUTPUT);
    pinMode(speedControllers[i].controllerToggle1, OUTPUT);
    pinMode(speedControllers[i].controllerToggle2, OUTPUT);
    
    digitalWrite(speedControllers[i].controllerToggle1, LOW);
    digitalWrite(speedControllers[i].controllerToggle2, HIGH);
    analogWrite(speedControllers[i].pwm, 255);
  }

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
  // iterate the sensor relay pairs
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
  };
  // iterate the speed controllers
  for (int i = 0; i < (sizeof(speedControllers) / sizeof(speedControllers[0])); i++) {
    long newPosition = speedControllers[i].enc.read();
    if (newPosition != speedControllers[0].state) {
      analogWrite(speedControllers[0].pwm, abs(newPosition));
      if (newPosition < 0 && speedControllers[0].state >= 0) {
        digitalWrite(speedControllers[0].controllerToggle2, LOW);
        digitalWrite(speedControllers[0].controllerToggle1, HIGH);
      }
      if (newPosition > 0 && speedControllers[0].state <= 0) {
        digitalWrite(speedControllers[0].controllerToggle2, HIGH);
        digitalWrite(speedControllers[0].controllerToggle1, LOW);
      }
      speedControllers[0].state = newPosition;
      Serial.println(newPosition);
    }
  };
}