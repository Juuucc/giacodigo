#include "variables.h"

void motorSetup() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  pinMode(LED, OUTPUT);
  pinMode(BOTON, INPUT);
  
  ledcSetup(0, frecuencia, resolucion);
  ledcSetup(1, frecuencia, resolucion);
  ledcAttachPin(PWMA, 0);
  ledcAttachPin(PWMB, 1);
}
void motorI(int vel){
  if (vel >= 0) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  } else {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    vel *= -1;
  }
  
  ledcWrite(0, vel); 
}

void motorD(int vel) {
  if (vel >= 0) {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
  } else {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    vel *= -1;
  }
  ledcWrite(1, vel);
}

void Motor(int velIzq, int velDer){
  motorI(velIzq);
  motorD(velDer);
}
