#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <QTRSensors.h>
#include "variables.h"

//config millis interseccion
unsigned long contIni = 0;
const unsigned long delayMillis = 1000;
bool contState = false;
int direccionGiro = 0; //0 nada, 1 izq, 2 der

// macros
#define N(sensor) ((sensor) >= umbral)
#define B(sensor) ((sensor) < umbral)
/*
  #define INTERSECCION (N(S0) && N(S1) && N(S2) && N(S3) && N(S4) && N(S5) && N(S6) && N(S7))
  #define GAP (B(S0) && B(S1) && B(S2) && B(S3) && B(S4) && B(S5) && B(S6) && B(S7))
*/



// Crea una instancia del sensor
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

QTRSensors sigueLineas;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

const int frecuencia = 5000;
const int resolucion = 8;

int umbral = 3000, vel = 75, delayCurvaD = 770, delayCurvaI = 880, rectaBloque = 1000;

void setup() {
  Serial.begin(9600);
  sigueLineas.setTypeAnalog();
  sigueLineas.setSensorPins((const uint8_t[])
  {
    36, 39, 34, 35, 32, 33, 25, 26
  }, SensorCount);
  sigueLineas.setEmitterPin(27);
  motorSetup();

  Serial.println("Iniciando...");

  // Inicia la comunicación I2C con el sensor
  if (!lox.begin()) {
    Serial.println(F("¡Error de inicio de VL53L0X! Verifica las conexiones."));
    while (1); // Detiene el programa si no se puede inicializar el sensor
  }

  Serial.println(F("VL53L0X iniciado correctamente."));

  while (digitalRead(BOTON) == 0) {
  }

}

void loop() {
  laser();
  
  if (millis() - contIni >= delayMillis) {
    // Ya pasó el tiempo de interseccion
    contState = false;
  }

  sigueLineas.read(sensorValues);

  int S0 = sensorValues[0];
  int S1 = sensorValues[1];
  int S2 = sensorValues[2];
  int S3 = sensorValues[3];
  int S4 = sensorValues[4];
  int S5 = sensorValues[5];
  int S6 = sensorValues[6];
  int S7 = sensorValues[7];

  if (N(S0) && N(S1) && N(S2) && N(S3) && N(S4) && B(S5) && B(S6) && B(S7)) {
    if (contState == true)
    {
      direccionGiro = 1;
      Motor(vel, vel);
    }
    else
    {
      delay(150);
      Motor(-vel, vel);
      delay(delayCurvaI);
    }
  }
  else if (B(S0) && B(S1) && B(S2) && N(S3) && N(S4) && N(S5) && N(S6) && N(S7)) {
    if (contState == true)
    {
      direccionGiro = 2;
      Motor(vel, vel);
    }
    else
    {
      delay(150);
      Motor(vel, -vel);
      delay(delayCurvaD);
    }
  }
  else if (N(S0) && N(S1) && N(S2) && N(S3) && N(S4) && N(S5) && N(S6) && N(S7)) {
    if (contState != true)
    {
      contIni = millis();
      contState = true;
      Motor(vel, vel);
    }
    else
    {
      if (direccionGiro == 1)
      {
        delay(150);
        Motor(-vel, vel);
        delay(delayCurvaI);
        direccionGiro = 0;
      }
      else if (direccionGiro == 2)
      {
        delay(150);
        Motor(vel, -vel);
        delay(delayCurvaD);
        direccionGiro = 0;
      }
      else if (direccionGiro == 0)
      {
        Motor((vel * 0.80), (vel * 0.80));
      }
    }
  }
  else if (B(S0) && B(S1) && B(S2) && B(S3) && B(S4) && B(S5) && B(S6) && B(S7)) {
    Motor(vel, vel);
    delay(100);
  }

  //seguidor
  else if (N(S0) && N(S1) && B(S2) && B(S3) && B(S4) && B(S5) && B(S6) && B(S7)) {
    Motor((vel * 0.10), (vel * 0.90));
  }
  else if (B(S0) && N(S1) && N(S2) && B(S3) && B(S4) && B(S5) && B(S6) && B(S7)) {
    Motor((vel * 0.20), (vel * 0.80));
  }
  else if (B(S0) && B(S1) && N(S2) && N(S3) && B(S4) && B(S5) && B(S6) && B(S7)) {
    Motor((vel * 0.20), (vel * 0.70));
  }
  else if (B(S0) && B(S1) && B(S2) && N(S3) && N(S4) && B(S5) && B(S6) && B(S7)) {
    Motor(vel, vel);
  }
  else if (B(S0) && B(S1) && B(S2) && B(S3) && N(S4) && N(S5) && B(S6) && B(S7)) {
    Motor((vel * 0.70), (vel * 0.20));
  }
  else if (N(S0) && N(S1) && B(S2) && B(S3) && B(S4) && N(S5) && N(S6) && B(S7)) {
    Motor((vel * 0.80), (vel * 0.20));
  }
  else if (B(S0) && B(S1) && B(S2) && B(S3) && B(S4) && B(S5) && N(S6) && N(S7)) {
    Motor((vel * 0.90), (vel * 0.10));
  }
}
