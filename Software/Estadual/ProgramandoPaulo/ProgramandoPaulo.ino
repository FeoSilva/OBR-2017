/*
   Programacao para o torneio OBR 2017 - Etapa Estadual
   Desenvolvido por Felipe Silva, Felipe Tiozo, Antonio Pereira e Giovannna Diodato;
   Equipe Robonaticos;
   Apoio: Sesi São Paulo / Engenhoteca
   http://facebook.com/robonaticos
*/

#include <QTRSensors.h>
#include <Servo.h>

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
#include <Thread.h>
#include "AnalogIn.h"
#include "DigitalIn.h"
#include "DigitalOut.h"
#include <ArduinoSensors.h>
#include <sensors/AnalogVoltage.h>

/// Basic robot definitions
#include "Config.h"

#include "Readings.h" // lê
#include "Verifications.h" // verifica
#include "Actions.h" // age
#include "Controllers.h" // controla


void setup() {

  Serial.begin(115200);

  
  // SETUP DE TUDO
  ligarMotores(); // ligar motores
  Servo1.attach(servo1);
  Servo1.write(180);
  Servo2.attach(servo2);
  Servo2.write(180);

  AlertaDeInicio();
  delay(500);


  Serial.println("Calibration Run");
  Serial.println("Calibration Running...");

  for (int i = 0; i < 100; i++)  // make the calibration take about 10 seconds
  {
    LED4.turnOn();
    qtra.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
    LED4.turnOff();
    delay(10);
  }

  LED4.turnOff();
  Serial.println("Calibration Done!");


  // Mostrar 'Inicializando...'
  Serial.println("Inicializando...");
  delay(800);

  // Initialize IMU
  while (!Serial);
  i2cSetup();

  Serial.println(F("Alive"));
  //MPU6050Connect();


}


void loop() {
  //PID(0.05, 0, 0.5, 50, 3500);
  //mover(100, 100);
  //Serial.println(analogRead(A5));
  //mover(-150, -150);
  //lerTodosSensores();
  lerTodosVerdes();
  //Seguidor(true);
  //Serial.println(analogRead(A3));
}
