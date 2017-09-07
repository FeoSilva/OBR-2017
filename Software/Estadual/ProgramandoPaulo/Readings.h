#include <Arduino.h>
#include <Wire.h>


/*
  FUNÇÕES de leitura DO ROBÔ
*/

/*int lerQTR(int pino) {
  pino = pino - 1;
  qtra.read(sensorValues);
  return sensorValues[pino];
  }*/

/*
  get DMP from MPU
*/



bool IMU_read() {
  // if programming failed, don't try to do anything
  if (!dmpReady) return;

  // wait for MPU interrupt or extra packet(s) available
  while (!mpuInterrupt && fifoCount < packetSize) {
    // other program behavior stuff here
    // .
    // .
    // .
    // if you are really paranoid you can frequently test in between other
    // stuff to see if mpuInterrupt is true, and if so, "break;" from the
    // while() loop to immediately process the MPU data
    // .
    // .
    // .
  }

  // reset interrupt flag and get INT_STATUS byte
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  // get current FIFO count
  fifoCount = mpu.getFIFOCount();

  // check for overflow (this should never happen unless our code is too inefficient)
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    // reset so we can continue cleanly
    mpu.resetFIFO();
    Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
  } else if (mpuIntStatus & 0x02) {
    // wait for correct available data length, should be a VERY short wait
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

    // read a packet from FIFO
    mpu.getFIFOBytes(fifoBuffer, packetSize);

    // track FIFO count here in case there is > 1 packet available
    // (this lets us immediately read more without waiting for an interrupt)
    fifoCount -= packetSize;

    // display Euler angles in degrees
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
  }
}
float getYPR(int n) {
  IMU_read();
  if(n == 0)
    return (ypr[n] * (180.0 / M_PI)) + 180;

  return ypr[n] * (180.0 / M_PI);
}

/*
  read a green sensor value
*/
int lerVerde(int lado) {


  // LER SENSOR VERDE DA ESQUERDA
  if (lado == ESQUERDA) {
    return analogRead(sensorVerdeEsquerda);
  }


  // LER SENSOR VERDE DA DIREITA
  if (lado == DIREITA) {
    return analogRead(sensorVerdeDireita);
  }

}


/*
  read all QTR sensors at once
*/
int lerTodosQTR() {
  unsigned int position = qtra.readLine(sensorValues);
  return position;
}



/*
  read a QTR sensor value
*/
unsigned int lerQTR(int pino) {

  //qtra.read(sensorValues);
  return analogRead(QTR[pino]);
  //return sensorValues[pino-1];

}
/*
  read a QTR sensor value
*/
int lerSharp(int pino) {
  return analogRead(Sharp[pino]);
}

/*
  sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
*/


/*

   Read sensors selected

   PARAMETERS FOR lerTodosSensores

   "QTRSensor" for "QTR sensors"
   "VERDESensor" for "sensors of green"
   "SHARPSensor" for "Sharp sensors"
   "ALLSensor" for "All sensors"
*/
void lerTodosSensores(int sensorType) {
  switch (sensorType) {
    case MPU:
      IMU_read();
      Serial.print("Yaw: ");
      Serial.print(getYPR(0));

      Serial.print(" Pitch: ");
      Serial.print(getYPR(1));

      Serial.print(" Roll: ");
      Serial.print(getYPR(2));
      Serial.println();
      break;
    case QTRSensor:
      Serial.print("QTR: ");
      Serial.print(lerQTR(1));
      Serial.print(" ");
      Serial.print(lerQTR(2));
      Serial.print(" ");
      Serial.print(lerQTR(3));
      Serial.print(" ");
      Serial.print(lerQTR(4));
      Serial.print(" ");
      Serial.print(lerQTR(5));
      Serial.print(" ");
      Serial.print(lerQTR(6));
      Serial.print(" ");
      Serial.print(lerQTR(7));
      Serial.print(" ");
      Serial.print(lerQTR(8));
      Serial.print(" / ");
      Serial.print(lerTodosQTR());
      Serial.println();
      break;
    case VERDESensor:
      Serial.print(" Verde Esquerda: ");
      Serial.print(lerVerde(ESQUERDA));
      Serial.print(" || Verde Direita: ");
      Serial.print(lerVerde(DIREITA));
      Serial.println();
      break;
    case SHARPSensor:
      Serial.print(" --- Sharp E: ");
      Serial.print(lerSharp(1));
      Serial.print(" | FC: ");
      Serial.print(lerSharp(2));
      Serial.print(" | D: ");
      Serial.print(lerSharp(3));
      Serial.print(" | FB: ");
      Serial.print(lerSharp(4));
      Serial.print(" | FE: ");
      Serial.print(lerSharp(5));
      Serial.print(" | FD: ");
      Serial.print(lerSharp(6));
      Serial.println();
      break;
    case ALLSensor:
      Serial.print("QTR: ");
      Serial.print(lerQTR(1));
      Serial.print(" ");
      Serial.print(lerQTR(2));
      Serial.print(" ");
      Serial.print(lerQTR(3));
      Serial.print(" ");
      Serial.print(lerQTR(4));
      Serial.print(" ");
      Serial.print(lerQTR(5));
      Serial.print(" ");
      Serial.print(lerQTR(6));
      Serial.print(" ");
      Serial.print(lerQTR(7));
      Serial.print(" ");
      Serial.print(lerQTR(8));
      Serial.print(" / ");
      Serial.print(lerTodosQTR());
      Serial.print(" Verde Esquerda: ");
      Serial.print(lerVerde(ESQUERDA));
      Serial.print(" || Verde Direita: ");
      Serial.print(lerVerde(DIREITA));
      Serial.print(" --- Sharp E: ");
      Serial.print(lerSharp(1));
      Serial.print(" | FC: ");
      Serial.print(lerSharp(2));
      Serial.print(" | D: ");
      Serial.print(lerSharp(3));
      Serial.print(" | FB: ");
      Serial.print(lerSharp(4));
      Serial.print(" | FE: ");
      Serial.print(lerSharp(5));
      Serial.print(" | FD: ");
      Serial.print(lerSharp(6));
      Serial.println();
      break;
  }
}
