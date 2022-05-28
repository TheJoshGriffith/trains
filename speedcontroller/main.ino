#include <Encoder.h>

// PINS
int controllerToggle1 = 2;
int controllerToggle2 = 3;
int pwm = 9;
int encoderRight = 18;
int encoderLeft = 19;

Encoder myEnc(encoderRight, encoderLeft);

void setup() {
  Serial.begin(9600);
  pinMode(controllerToggle1, OUTPUT);
  pinMode(controllerToggle2, OUTPUT);
  pinMode(pwm, OUTPUT);
  digitalWrite(controllerToggle1, LOW);
  digitalWrite(controllerToggle2, HIGH);
  analogWrite(pwm, 255);
}

long oldPosition  = -999;

void loop() {
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    analogWrite(pwm, abs(newPosition));
    if (newPosition < 0 && oldPosition >= 0) {
      digitalWrite(controllerToggle2, LOW);
      digitalWrite(controllerToggle1, HIGH);
    }
    if (newPosition > 0 && oldPosition <= 0) {
      digitalWrite(controllerToggle2, HIGH);
      digitalWrite(controllerToggle1, LOW);
    }
    oldPosition = newPosition;
    Serial.println(newPosition);
  }
}