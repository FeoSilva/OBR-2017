#include <Arduino.h>

/*
  FUNÇÕES DE VERIFICAÇÃO DO ROBÔ
*/

void checarBateria() {

  if (myCustomVoltimeter.readVoltage() < VOLTAGE_LOW && myCustomVoltimeter.readVoltage() > 6) {
    int t = 0;
    while (myCustomVoltimeter.readVoltage() < VOLTAGE_LOW && myCustomVoltimeter.readVoltage() > 6) {
      Serial.println("Bateria acabando");
      Serial.println("Nivel: " + String(myCustomVoltimeter.readVoltage()) + "V");
      Buzzer.turnOn();
      LED1.turnOn();
      LED2.turnOn();
      LED3.turnOn();
      LED4.turnOn();
      delay(500);
      Buzzer.turnOff();
      LED1.turnOff();
      LED2.turnOff();
      LED3.turnOff();
      LED4.turnOff();
      delay(300);
      Serial.println(myCustomVoltimeter.readVoltage());
    }


  }


}


/*
  void I2CScanner() {
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknow error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
  delay(5000);           // wait 5 seconds for next scan
  }


*/
boolean verificaGap() {
  //if(lerQTR(1) <= branco && lerQTR(2) <= branco && lerQTR(3) <= branco && lerQTR(4) <= branco && lerQTR(5) <= branco && lerQTR(6) <= branco && lerQTR(7) <= branco && lerQTR(8) <= branco){
  if (lerTodosQTR() == 0 || lerTodosQTR() == 7000) {
    return true;
  } else {
    return false;
  }
}

boolean verificaEncruzilhada() {
  if (lerQTR(1) >= preto && lerQTR(2) >= preto && lerQTR(3) >= preto && lerQTR(4) >= preto && lerQTR(5) >= preto && lerQTR(6) >= preto && lerQTR(7) >= preto && lerQTR(8) >= preto) {
    return true;
  } else {
    return false;
  }
}

boolean verificaSilverTap() {
  if (lerQTR(1) >= silver_tape && lerQTR(2) >= silver_tape && lerQTR(3) >= silver_tape && lerQTR(4) >= silver_tape && lerQTR(5) >= silver_tape && lerQTR(6) >= silver_tape && lerQTR(7) >= silver_tape && lerQTR(8) >= silver_tape) {
    return true;
  } else {
    return false;
  }
}


boolean verificaObstaculo() {

  if (lerSharp(2) > 340 && lerSharp(2) < 450) {
    return true;
  } else {
    return false;
  }

}

int verificaT() {
  if (lerQTR(1) >= preto && lerQTR(2) >= preto && lerQTR(3) >= preto && lerQTR(7) <= branco && lerQTR(8) <= branco) {
    return ESQUERDA;
  } else if (lerQTR(6) >= preto && lerQTR(7) >= preto && lerQTR(8) >= preto && lerQTR(1) <= branco && lerQTR(2) <= branco) {
    return DIREITA;
  }
  return IDDLE;

}




int verdeesquerda = 0;
int verdedireita = 0;

int verdeesquerda_somente = 0;
int verdedireita_somente = 0;

boolean verificaVerde(int lado) {

  if (lado == ESQUERDA) {
  
    if ((lerSensorVerde(ESQUERDA, G) > 40) && (lerSensorVerde(ESQUERDA, R) > 70)) {
      verdeesquerda++;
    //} else if ((lerSensorVerde(ESQUERDA, G) > 80) && (lerSensorVerde(ESQUERDA, G) < 150)) {
    //  verdeesquerda++;
    } else {
      verdeesquerda = 0;
    }

    if (verdeesquerda >= 5) {
      return true;
    } else {
      return false;
    }
  }



  if (lado == DIREITA) {
  
    if ((lerSensorVerde(DIREITA, G) > 40 /*&& (lerSensorVerde(DIREITA, G) < 45)*/)) {
      verdedireita++;
    //} else if ((lerSensorVerde(DIREITA, G) > 220 && (lerSensorVerde(DIREITA, G) < 330))) {
    //  verdedireita++;
    } else {
      verdedireita = 0;
    }

    if (verdedireita >= 5) {
      return true;
    } else {
      return false;
    }


  }


}

boolean verificaRampa() {

//  if(lerSharpDigital(3) == 1 && lerSharpDigital(4) == 1 && lendoMpuAccel() > ANGULO_RAMPA) {
  //  return true;
  //}
  return false;

}

boolean verificaRedutor() {

  if (lerBtnRedutor() == 1) {
    return true;
  }
  return false;

}


boolean verificaParede() {
//  if ((lerSharp(1) > 230) && (lerSharp(2) > 220) && (lerSharpDigital(1) == 1) && (lerSharpDigital(2) == 1)) {
  //  return true;
  //}
  return false;
}
boolean verificaAreaResgate() {
  if ((lerSharp(1) > 310) && (lerSharp(2) > 100)) {
    return true;
  }
  return false;
}
boolean verificaVitima() {
  if ((lerSharp(1) > 280) && (lerSharp(2) < 190)) {
   /* if ((lerSharpDigital(1) == 1) && (lerSharpDigital(2) == 0)) {
      return true;
    }*/
    return true;
  }
  return false;
}

boolean verificaVitimaEsquerda() {
//  if (lerSharpDigital(3) == 1) {
  //  return true;
  //}
  return false;
}
boolean verificaVitimaDireita() {
//  if (lerSharpDigital(4) == 1) {
  //  return true;
  //}
  return false;
}
