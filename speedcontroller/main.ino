#include <Encoder.h>

struct SpeedController {
  int controllerToggle1;
  int controllerToggle2;
  int pwm;
  int encoderRight;
  int encoderLeft;
  long state;
  Encoder enc;
}

speedControllers controllers[] = {
  {2, 3, 9, 18, 19, -999},
  {4, 5, 9, 18, 19, -999},
  {6, 7, 9, 18, 19, -999}
}

// PINS
int controllerToggle1 = 2;
int controllerToggle2 = 3;
int pwm = 9;
int encoderRight = 18;
int encoderLeft = 19;

Encoder myEnc(encoderRight, encoderLeft);

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
}

long oldPosition  = -999;

void loop() {
  for (int i = 0; i < (sizeof(speedControllers) / sizeof(speedControllers[0])); i++) {
    long newPos = 
  }
  
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