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


void MPUMath() {
  mpu.dmpGetQuaternion(&q, fifoBuffer);
  mpu.dmpGetGravity(&gravity, &q);
  mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
  Yaw = (ypr[0] * 180 / M_PI);
  Pitch = (ypr[1] * 180 / M_PI);
  Roll = (ypr[2] * 180 / M_PI);
}


void GetDMP() { // Best version I have made so far
  // Serial.println(F("FIFO interrupt at:"));
  // Serial.println(micros());
  mpuInterrupt = false;
  FifoAlive = 1;
  fifoCount = mpu.getFIFOCount();
  /*
    fifoCount is a 16-bit unsigned value. Indicates the number of bytes stored in the FIFO buffer.
    This number is in turn the number of bytes that can be read from the FIFO buffer and it is
    directly proportional to the number of samples available given the set of sensor data bound
    to be stored in the FIFO
  */

  // PacketSize = 42; refference in MPU6050_6Axis_MotionApps20.h Line 527
  // FIFO Buffer Size = 1024;
  uint16_t MaxPackets = 20;// 20*42=840 leaving us with  2 Packets (out of a total of 24 packets) left before we overflow.
  // If we overflow the entire FIFO buffer will be corrupt and we must discard it!

  // At this point in the code FIFO Packets should be at 1 99% of the time if not we need to look to see where we are skipping samples.
  if ((fifoCount % packetSize) || (fifoCount > (packetSize * MaxPackets)) || (fifoCount < packetSize)) { // we have failed Reset and wait till next time!
    digitalWrite(LED_PIN, LOW); // lets turn off the blinking light so we can see we are failing.
    Serial.println(F("Reset FIFO"));
    if (fifoCount % packetSize) Serial.print(F("\t Packet corruption")); // fifoCount / packetSize returns a remainder... Not good! This should never happen if all is well.
    Serial.print(F("\tfifoCount ")); Serial.print(fifoCount);
    Serial.print(F("\tpacketSize ")); Serial.print(packetSize);

    mpuIntStatus = mpu.getIntStatus(); // reads MPU6050_RA_INT_STATUS       0x3A
    Serial.print(F("\tMPU Int Status ")); Serial.print(mpuIntStatus , BIN);
    // MPU6050_RA_INT_STATUS       0x3A
    //
    // Bit7, Bit6, Bit5, Bit4          , Bit3       , Bit2, Bit1, Bit0
    // ----, ----, ----, FIFO_OFLOW_INT, I2C_MST_INT, ----, ----, DATA_RDY_INT

    /*
      Bit4 FIFO_OFLOW_INT: This bit automatically sets to 1 when a FIFO buffer overflow interrupt has been generated.
      Bit3 I2C_MST_INT: This bit automatically sets to 1 when an I2C Master interrupt has been generated. For a list of I2C Master interrupts, please refer to Register 54.
      Bit1 DATA_RDY_INT This bit automatically sets to 1 when a Data Ready interrupt is generated.
    */
    if (mpuIntStatus & B10000) { //FIFO_OFLOW_INT
      Serial.print(F("\tFIFO buffer overflow interrupt "));
    }
    if (mpuIntStatus & B1000) { //I2C_MST_INT
      Serial.print(F("\tSlave I2c Device Status Int "));
    }
    if (mpuIntStatus & B1) { //DATA_RDY_INT
      Serial.print(F("\tData Ready interrupt "));
    }
    Serial.println();
    //I2C_MST_STATUS
    //PASS_THROUGH, I2C_SLV4_DONE,I2C_LOST_ARB,I2C_SLV4_NACK,I2C_SLV3_NACK,I2C_SLV2_NACK,I2C_SLV1_NACK,I2C_SLV0_NACK,
    mpu.resetFIFO();// clear the buffer and start over
    mpu.getIntStatus(); // make sure status is cleared we will read it again.
  } else {
    while (fifoCount  >= packetSize) { // Get the packets until we have the latest!
      if (fifoCount < packetSize) break; // Something is left over and we don't want it!!!
      mpu.getFIFOBytes(fifoBuffer, packetSize); // lets do the magic and get the data
      fifoCount -= packetSize;
    }
    MPUMath(); // <<<<<<<<<<<<<<<<<<<<<<<<<<<< On success MPUMath() <<<<<<<<<<<<<<<<<<<
    digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // Blink the Light
    if (fifoCount > 0) mpu.resetFIFO(); // clean up any leftovers Should never happen! but lets start fresh if we need to. this should never happen.
  }
}


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

void lerTodosVerdes(){

  Serial.print(lerVerde(ESQUERDA));
  Serial.print(" || ");
  Serial.println(lerVerde(DIREITA));
  
}


int lerTodosQTR() {
  unsigned int position = qtra.readLine(sensorValues);
  return position;
}




unsigned int lerQTR(int pino) {

  //qtra.read(sensorValues);
  return analogRead(QTR[pino]);
  //return sensorValues[pino-1];

}

int lerSharp(int pino) {
  return analogRead(Sharp[pino]);
}


int lerBtnRedutor() {
  return digitalRead(BTN_REDUTOR);
}
// sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
double lendoMpuAccel() {

  if (mpuInterrupt) { // wait for MPU interrupt or extra packet(s) available
    GetDMP(); // Gets the MPU Data and canculates angles
  }
  //*****************************************************************************************************************************************************************************
  //************************************              Put any code you want to use the values that come from your MPU6050 here               ************************************
  //*****************************************************************************************************************************************************************************

  static long QTimer = millis();
  if ((long)( millis() - QTimer ) >= 100) {
    QTimer = millis();
    // Serial.print(F("\t Yaw")); Serial.print(Yaw);
    //Serial.print(F("\t Pitch ")); Serial.print(Pitch);
    // Serial.print(F("\t Roll ")); Serial.print(Roll);
    // Serial.println();
  }
  return Roll;

}
double lendoMpuGyro() {

  if (mpuInterrupt) { // wait for MPU interrupt or extra packet(s) available
    GetDMP(); // Gets the MPU Data and canculates angles
  }
  //*****************************************************************************************************************************************************************************
  //************************************              Put any code you want to use the values that come from your MPU6050 here               ************************************
  //*****************************************************************************************************************************************************************************

  static long QTimer = millis();
  if ((long)( millis() - QTimer ) >= 100) {
    QTimer = millis();
    //Serial.print(F("\t Yaw")); Serial.print(Yaw);
    //Serial.print(F("\t Pitch ")); Serial.print(Pitch);
    //Serial.print(F("\t Roll ")); Serial.print(Roll);
    //Serial.println();
  }
  return Yaw;

}


void lerTodosSensores() {
  
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
  Serial.print(" --- E: ");
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
  //delay(250);
}

