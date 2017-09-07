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



int took;
int outOfSyncs;
int interruptStatus;
uint8_t packet[64];
bool IMU_read() {
  static int fifoCount = 0;

  // Check for interrupt
  if (!imuInterrupted && fifoCount < 42)
    return false;

  // Clear interrupt flag
  imuInterrupted = false;

  // Check for new Packet
  delayMicroseconds(500);
  interruptStatus = mpu.getIntStatus();
  delayMicroseconds(50);
  fifoCount = mpu.getFIFOCount();

  if ((interruptStatus & 0x10) || fifoCount == 1024 || fifoCount % 42 > 0) {
    // reset so we can continue cleanly
    delayMicroseconds(50);
    mpu.resetFIFO();
    delayMicroseconds(50);
    fifoCount = 0;
    return false;
  }

  // Check if new data available
  bool run = (interruptStatus & 0x02);

  // Don't read if not ready
  if (!run)
    return false;

  unsigned long start = millis();

  // read a packet from FIFO
  delayMicroseconds(50);
  mpu.getFIFOBytes(packet, 42);
  
  fifoCount -= 42;

  // Convert and save state to Object
  mpu.dmpGetQuaternion(&q, packet);
  mpu.dmpGetGravity(&gravity, &q);
  mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

  newImuData = true;

  took = millis() - start;
}
float getYPR(int n) {

  if (n == 0) {
    float grau = (ypr[n] * (180.0 / M_PI)) + 180;
    if (grau == 180.0 || grau == 360.0) {
      return 0.0;
    } else {
      return grau;
    }
  }
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
