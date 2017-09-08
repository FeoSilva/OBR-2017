/*
   Programacao para o torneio OBR 2017 - Etapa Estadual
   Desenvolvido por Felipe Silva, Felipe Tiozo, Antonio Pereira e Giovannna Diodato;
   Equipe Robonaticos;
   Apoio: Sesi São Paulo / Engenhoteca
   http://facebook.com/robonaticos
*/

#include <QTRSensors.h>
#include <Servo.h>

#include <Encoder.h>

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"

#include "Wire.h"
#include "MPU6050_6Axis_MotionApps20.h"
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

  // i2c setup
  //Wire.begin();
  //TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)

  Serial.begin(115200);

  // SETUP DE TUDO
  ligarMotores(); // ligar motores
  debugEncoder();
  Servo1.attach(servo1);
  Servo1.write(93);
  //Servo2.detach();
  //Servo2.attach(servo2);
  //Servo2.write(180);

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

  Serial.println(F("Alive"));

  // put your setup code here, to run once:

  // MPU6050 init
  IMU_init();

}

void loop() {
  //Serial.println(getYPR(0));
  //MPU6050Connect();
  //lendoMpuGyro();
  lerTodosSensores(SHARPSensor);
  //mover(70, 70);
  //Seguidor(true);
  if (Botao1.readValue() == HIGH) {
    LED2.turnOn();
    delay(500);
    Seguidor(true);
  }
  if (Botao2.readValue() == HIGH) {
    Resgate(true);
    //Verde(DIREITA);
    
    curvaEncoder(60, 100, ESQUERDA);
    //Curva90Graus(ESQUERDA, LIN);
  }
  if (Botao4.readValue() == HIGH) {
    resgate();
    //Curva90Graus(DIREITA, LIN);
  }
  
}

