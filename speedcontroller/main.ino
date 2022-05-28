#include <Encoder.h>

struct speedController {
  int controllerToggle1;
  int controllerToggle2;
  int pwm;
  int encoderRight;
  int encoderLeft;
  long state;
  Encoder enc;
};

speedController speedControllers[] = {
  {2, 3, 9, 18, 19, -999, Encoder(18, 19)},
  {4, 5, 10, 16, 17, -999, Encoder(16, 17)},
  {6, 7, 11, 14, 15, -999, Encoder(14, 15)}
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
}

long oldPosition  = -999;

void loop() {
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
  }
}