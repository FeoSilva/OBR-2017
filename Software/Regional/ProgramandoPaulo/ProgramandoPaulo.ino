/*
   Programacao para o torneio OBR 2017
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
  ligarSharpDigital();
  ligarSensoresCor();
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

  //qtra.read(sensorValues);

  // Se a bateria estiver baixa, leds piscando, beep no buzzer e mensagem na tela "Bateria Baixa"
  //checarBateria();

  lerTodosSensores();
  //delay(100);
  //Serial.println(lerSharp(2));
  //Serial.println(lerQTRVerde(3));
  //PID(KP, KI, KD, 35, 3500);
  //Serial.println(lerSensorVerde(DIREITA));
  //Serial.println(lerSharpDigital(4));
  //mover(120,120);
  /*Serial.print(lerSharp(1));
    Serial.print(" !! ");
    Serial.print(lerSharp(2));
    Serial.print(" !! ");
    Serial.print(lerSharpDigital(1));
    Serial.print(" !! ");
    Serial.println(lerSharpDigital(4));
  */
  //Serial.println(lerSharp(2));

  if (Botao1.readValue() == HIGH) {
    LED2.turnOn();
    delay(1000);
    Seguidor(true);
    LED2.turnOff();
  }
  if (Botao2.readValue() == HIGH) {
    Serial.println("******************************* RAMPA *****************************");
    pararMotores();
    delay(1000);
    Servo1.attach(servo1);
    Servo1.write(40);
    pararMotores();
    delay(600);
    
    Buzzer.turnOn();
    while (verificaSilverTap() == false && verificaEncruzilhada() == false) {

      //Serial.println(lendoMpuAccel());
      PID(0.15, 0, 0, 230, 3500);

      if (verificaRedutor() == true) {
        Servo1.attach(servo1);
        Servo1.write(90);
        pararMotores();
        delay(500);
        mover(255, 255);
        delay(700);
        Servo1.attach(servo1);
        Servo1.write(40);
      }
      if (verificaGap() == true) {
        Serial.println("******************************* GAP *****************************");
        while (verificaGap() == true) {
          Gap(ST_RAMPA);
        }
      }
    }

    mover(255, 255);
    delay(300);
    mover(255, 255);
    Servo1.attach(servo1);
    Servo1.write(180);
    delay(100);

    Buzzer.turnOff();
    while (true) {
      Salao3(true);
    }
  }
  if (Botao3.readValue() == HIGH) {

  }

  if (Botao4.readValue() == HIGH) {
    LED2.turnOn();
    LED3.turnOn();
    LED4.turnOn();
    delay(1000);
    Salao3(true);

  }



}
