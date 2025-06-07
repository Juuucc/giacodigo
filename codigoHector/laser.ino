#include <Wire.h>
#include <Adafruit_VL53L0X.h>

#include "variables.h"

void laser() {
  VL53L0X_RangingMeasurementData_t measure; //Declara la variable para almacenar los datos de la medición

  //Realiza la medición de distancia y almacena los resultados en la variable 'measure'
  lox.rangingTest(&measure, false);

  //Verifica si la medición es válida
  if (measure.RangeStatus != 4) {  // Si el estado es diferente de 4, la medición es válida
    Serial.print("Distancia: ");
    Serial.print(measure.RangeMilliMeter); // Imprime la distancia medida en milímetros
    Serial.println(" mm");
    if (measure.RangeMilliMeter <= 115) {
      delay(150);
      Motor(vel, -vel);
      delay(delayCurvaI);
      
      Motor(vel, vel);
      delay(rectaBloque*0.7);

      delay(150);
      Motor(-(vel * 1.25), (vel * 1.25));
      delay(delayCurvaD);

      Motor(vel, vel);
      delay(rectaBloque*1.8);

      delay(150);
      Motor(-(vel * 1.20), (vel * 1.20));
      delay(delayCurvaD);

      Motor(vel, vel);
      delay(rectaBloque*0.7);

      delay(150);
      Motor(vel, -vel);
      delay(delayCurvaI);
    }
  } else {
    Serial.println("Fuera de rango"); // Imprime un mensaje si la medición está fuera de rango
  }
  // Espera 0.1 segundos entre lecturas para evitar sobrecargar el sistema

}