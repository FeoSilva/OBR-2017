#include <Arduino.h>
#include <Servo.h>
#include <QTRSensors.h>


//FUNÇÕES de Ação DO ROBÔ

/*
   Stop all motors
*/
void pararMotores() {
  analogWrite(MotorEsquerdoFrente, 0);
  analogWrite(MotorEsquerdoTras, 0);

  /*** direita ***/
  analogWrite(MotorDireitoFrente, 0);
  analogWrite(MotorDireitoTras, 0);
}


/*
  Engine control

  left-to-right engine power, respectively

*/
void mover(int a, int b) {
  if (a > 0) {
    //a=a*2.55;
    //a=a+compensacao;
    analogWrite(MotorEsquerdoFrente, a);
    analogWrite(MotorEsquerdoTras, 0);
  }
  else if (a == 0) {
    analogWrite(MotorEsquerdoFrente, 0);
    analogWrite(MotorEsquerdoTras, 0);
  }
  else {

    //a=a*-2.55;
    analogWrite(MotorEsquerdoFrente, 0);
    analogWrite(MotorEsquerdoTras, abs(a));
  }
  if (b > 0) {
    // b=b*2.55;
    analogWrite(MotorDireitoFrente, b);
    analogWrite(MotorDireitoTras, 0);
  }
  else if (b == 0) {
    analogWrite(MotorDireitoFrente, 0);
    analogWrite(MotorDireitoTras, 0);
  }
  else {
    //b=b*-2.55;
    analogWrite(MotorDireitoFrente, 0);
    analogWrite(MotorDireitoTras, abs(b));
  }
};

/*
  move the robot by pulses of encoder
*/
void moverEncoder(int pulsos, int forcaEsquerda, int forcaDireita) {
  encoderEsquerdo.write(0); // reseta o encoder
  mover(forcaEsquerda, forcaDireita);
  while (encoderEsquerdo.read() <= pulsos) {
  }
  pararMotores();
}

/*
  convert for cm to encoder pulses and make the robo walk that
*/
void andarCM (int distanciaCM, int forcaCM) {
  float cmConvertidos = (distanciaCM / circunferenciaRoda) * pulsosPorRotacao;
  moverEncoder(cmConvertidos, forcaCM, forcaCM);
}
// fazer curvas com encoder
void curvaEncoder(int graus, int forcaCurvaEncoder, int sentido) {
  float grausConvertido = ((distanciaEntreEixos / raioRoda) * graus) * 9.8611;

  if (sentido == DIREITA) {
    moverEncoder(grausConvertido, forcaCurvaEncoder, forcaCurvaEncoder * -1);
  }
}

/*
  Encolder test
*/
void debugEncoder() {
  mover(100, 100);
  delay(200);
  pararMotores();
  Serial.print("Encoder Esquerdo: ");
  Serial.println(encoderEsquerdo.read());
  if (encoderEsquerdo.read() <= 20) {
    while (1) {
      //Buzzer.turnOn();
      LED1.turnOn();
      LED2.turnOn();
      LED3.turnOn();
      LED4.turnOn();
      delay(200);
      Buzzer.turnOff();
      LED1.turnOff();
      LED2.turnOff();
      LED3.turnOff();
      LED4.turnOff();
      delay(200);
    }
  }
  encoderEsquerdo.write(0);
}

/*
  warning that the robot was started
*/
void AlertaDeInicio() {
  // Acender Todos LEDs
  // Dar Beep no buzzer
  Buzzer.turnOn();
  LED1.turnOn();
  LED2.turnOn();
  LED3.turnOn();
  LED4.turnOn();
  delay(25);
  Buzzer.turnOff();
  LED1.turnOff();
  LED2.turnOff();
  LED3.turnOff();
  LED4.turnOff();
  delay(50);
  Buzzer.turnOn();
  LED1.turnOn();
  LED2.turnOn();
  LED3.turnOn();
  LED4.turnOn();
  delay(25);
  Buzzer.turnOff();
  LED2.turnOff();
  LED3.turnOff();
  LED4.turnOff();


}
/*
  warning that the robot has passed a T
*/
void AlertaT() {
  LED4.turnOn();
  delay(150);
  LED4.turnOff();
  delay(300);
  LED4.turnOn();
  delay(150);
  LED4.turnOff();
  delay(300);
}

int lastError = 0;

/*
  Line follower PID
*/

void PID (double kP, double kI, double kD, double tP, int media) {

  //int erro = lerTodosQTR() - media;
  int erro = lerTodosQTR() - media;

  //int error = map(media, 0, 7000, -100, 100);

  //int desaceleracao = map((error - nova_media), -100, 100, 1, 0);

  P = kP * erro;
  I = I + (erro * kI);
  D = kD * (erro - lastError);

  ganho = P + I + D;

  lastError = erro;

  motorB = tP + ganho;
  motorC = tP - ganho;

  //Serial.print(motorB);
  //Serial.print(" / ");
  //Serial.print(motorC);
  //Serial.println();
  mover(motorB, motorC);

  //Serial.println(analogRead(A15));

};

/*void Curva90Graus(int lado) {

  if(lado == ESQUERDA) {

      pararMotores();
      delay(250);
      mover(forca, forca);
      delay(250);
      mover(forca_Curva * -1, forca_Curva);
      delay(650);
      mover(forca * -1, forca * -1);
      delay(250);

  }
  if(lado == DIREITA) {

      pararMotores();
      delay(300);
      mover(forca, forca);
      delay(250);
      mover(forca_Curva, forca_Curva * -1);
      delay(600);
      mover(forca * -1, forca * -1);
      delay(250);
  }

  }*/

/*
  makes 90 degree curve
*/

void Curva90Graus(int lado, int tipo) {
  MPU6050Connect();

  // robo anda para frente antes de fazer a curva
  if (lado == ESQUERDA && tipo == LIN) {

    pararMotores();
    delay(250);
    mover(forca, forca);
    delay(270);
    pararMotores();

  } else if (lado == DIREITA && tipo == LIN) {

    pararMotores();
    delay(250);
    mover(forca, forca);
    delay(270);
    pararMotores();
  }

  // inicializa a mpu

  // faz a curva dependendo do lado passado no parametro
  if (lado == ESQUERDA) {
    Yaw = 0;

    LED4.turnOn();

    // Gira até o angulo do giroscopio for maior que o solicitado
    if (tipo == OBS) {
      while (lendoMpuGyro() >= angulo_curva_esquerda_obs * -1) {
        mover(forca_Curva * -1, forca_Curva);
      }
    }
    if (tipo == LIN) {
      while (lendoMpuGyro() >= angulo_curva_esquerda * -1) {
        mover(forca_Curva * -1, forca_Curva);
      }
    }

    LED4.turnOff();

    if (tipo == LIN) {
      // andada para tras depois de fazer a curva
      pararMotores();
      mover(forca * -1, forca * -1);
      delay(100);
      pararMotores();
    }

  }
  else if (lado == DIREITA) {
    Yaw = 0;

    LED4.turnOn();

    // Gira pra direita ate o angular for menor que o solicitado
    if (tipo == OBS) {
      while (lendoMpuGyro() <= angulo_curva_direita_obs) {
        mover(forca_Curva, forca_Curva * -1);
      }
    }
    if (tipo == LIN) {
      while (lendoMpuGyro() <= angulo_curva_direita) {
        mover(forca_Curva, forca_Curva * -1);
      }
    }

    LED4.turnOff();

    if (tipo == LIN) {
      // andada para tras depois de fazer a curva
      pararMotores();
      mover(forca * -1, forca * -1);
      delay(100);
      pararMotores();
    }
  }

  pararMotores();


}
/*
  makes T curve
*/
void T(int lado) {


  // anda um pouco para frente para verificar se é T ou 90 graus
  mover(forca_Baixa, forca_Baixa);
  delay(150);
  // robo para

  pararMotores();
  delay(1000);

  // verifica se esta tudo branco, se sim, ele volta e faz a curva
  if (lerTodosQTR() == 0 || lerTodosQTR() == 7000) {

    mover(forca_Baixa * -1, forca_Baixa * -1);
    delay(200);
    pararMotores();
    delay(500);

    if (lado == ESQUERDA) {
      LED3.turnOn();
      Curva90Graus(ESQUERDA, LIN);
      LED3.turnOff();
    }
    if (lado == DIREITA) {
      LED4.turnOn();
      Curva90Graus(DIREITA, LIN);
      LED4.turnOff();
    }


  }
}
/*
  makes GAP
*/
void Gap(int estado) {
  LED1.turnOff();
  if (estado == ST_PRINCIPAL) {
    mover(forca_Baixa, forca_Baixa);
  } else if (estado == ST_RAMPA) {
    mover(forcaRampa, forcaRampa);
  }
  LED1.turnOn();
}
/*
  makes crossroads
*/
void Encruzilhada() {
  LED4.turnOn();
  mover(forca, forca);
  LED4.turnOff();
}
/*
  makes curve of green
*/
boolean Verde(int lado) {

  if (lado == ESQUERDA) {

    LED3.turnOn();
    Buzzer.turnOn();

    Curva90Graus(ESQUERDA, LIN);

    Buzzer.turnOff();
    LED3.turnOff();
  }

  if (lado == DIREITA) {

    LED4.turnOn();
    Buzzer.turnOn();

    Curva90Graus(DIREITA, LIN);

    Buzzer.turnOff();
    LED4.turnOff();
  }

}
/*
  makes reductor
*/
void Redutor() {

  Buzzer.turnOn();
  pararMotores();
  delay(300);
  mover(forca * -1, forca * -1);
  delay(200);
  unsigned long tempo_redutor = millis();
  mover(forca_Redutor, forca_Redutor);
  delay(300);
  while (millis() <= tempo_redutor + 700) {
    PID(KP_redutor, KI_redutor, KD_redutor, forca_Redutor, setPoint);
  }
  pararMotores();
  Buzzer.turnOff();
}

/*
  makes 45 degrees curve
*/
void Curva45Graus(int lado, int tipo) {
  MPU6050Connect();

  // robo anda para frente antes de fazer a curva
  if (lado == ESQUERDA && tipo == LIN) {

    pararMotores();
    delay(250);
    mover(forca, forca);
    delay(270);
    pararMotores();

  } else if (lado == DIREITA && tipo == LIN) {

    pararMotores();
    delay(250);
    mover(forca, forca);
    delay(270);
    pararMotores();
  }

  // inicializa a mpu

  // faz a curva dependendo do lado passado no parametro
  if (lado == ESQUERDA) {
    Yaw = 0;

    LED4.turnOn();

    // Gira até o angulo do giroscopio for maior que o solicitado
    if (tipo == OBS) {
      while (lendoMpuGyro() >= angulo_curva_esquerda_obs2 * -1) {
        mover(forca_Curva * -1, forca_Curva);
      }
    }
    if (tipo == LIN) {
      while (lendoMpuGyro() >= angulo_curva_esquerda * -1) {
        mover(forca_Curva * -1, forca_Curva);
      }
    }

    LED4.turnOff();

    if (tipo == LIN) {
      // andada para tras depois de fazer a curva
      pararMotores();
      mover(forca * -1, forca * -1);
      delay(200);
      pararMotores();
    }

  }
  else if (lado == DIREITA) {
    Yaw = 0;

    LED4.turnOn();

    // Gira pra direita ate o angular for menor que o solicitado
    if (tipo == OBS) {
      while (lendoMpuGyro() <= angulo_curva_direita_obs2) {
        mover(forca_Curva, forca_Curva * -1);
      }
    }
    if (tipo == LIN) {
      while (lendoMpuGyro() <= angulo_curva_direita) {
        mover(forca_Curva, forca_Curva * -1);
      }
    }

    LED4.turnOff();

    if (tipo == LIN) {
      // andada para tras depois de fazer a curva
      pararMotores();
      mover(forca * -1, forca * -1);
      delay(200);
      pararMotores();
    }
  }

  pararMotores();


}
/*
  makes obstacle
*/
void Obstaculo(int lado) {
  // Primeria curva ===================================================
  pararMotores();
  delay(300);
  Servo1.attach(servo1);
  Servo1.write(135);

  mover(forca_Baixa * -1, forca_Baixa * -1);
  delay(100);
  pararMotores();

  if (lado == ESQUERDA) {
    Curva90Graus(ESQUERDA, OBS);
  }
  if (lado == DIREITA) {
    Curva90Graus(DIREITA, OBS);
  }

  // Anda pra frente até NÃO encontrar mais objeto
  if (lado == ESQUERDA) {
    //    while (lerSharpDigital(4) == 1) {
    //    LED4.turnOn();
    //  mover(forca, forca);
    //}
    LED4.turnOff();
  }

  if (lado == DIREITA) {
    //    while (lerSharpDigital(3) == 1) {
    //    LED4.turnOn();
    //  mover(forca, forca);
    //}
    LED4.turnOff();
  }
  pararMotores();
  delay(300);

  // Anda mais um pouco para fazer a segunda curva ===================

  mover(forca_Baixa, forca_Baixa);
  delay(750);
  pararMotores();


  // Segunda curva ===================================================
  if (lado == ESQUERDA) {
    Curva90Graus(DIREITA, OBS);
  }
  if (lado == DIREITA) {
    Curva90Graus(ESQUERDA, OBS);
  }

  // Anda até encontrar objeto =======================================

  pararMotores();
  delay(300);

  if (lado == ESQUERDA) {
    LED4.turnOn();
    //    while (lerSharpDigital(4) == 0) {
    //    mover(forca_Baixa, forca_Baixa);
    //}
    LED4.turnOff();
    pararMotores();
    delay(300);
    //    while (lerSharpDigital(4) == 1) {
    //    mover(forca_Baixa, forca_Baixa);
    //}
    LED4.turnOff();
  }
  if (lado == DIREITA) {
    LED4.turnOn();
    //    while (lerSharpDigital(3) == 0) {
    //    mover(forca, forca);
    // }
    LED4.turnOff();
    pararMotores();
    delay(300);
    //while (lerSharpDigital(3) == 1) {
    //  mover(forca, forca);
    // }
    LED4.turnOff();
  }

  pararMotores();
  delay(300);

  // Anda pouco mais para fazer a terceira curva ====================

  mover(forca_Baixa, forca_Baixa);
  delay(450);
  pararMotores();
  delay(300);


  // Terceira curva
  if (lado == ESQUERDA) {
    Curva90Graus(DIREITA, OBS);
  }
  if (lado == DIREITA) {
    Curva90Graus(ESQUERDA, OBS);
  }

  // Anda até encontrar obstaculo ======================================

  pararMotores();
  delay(300);

  /*if (lado == ESQUERDA) {
    LED4.turnOn();
    while (lerSharpDigital(4) == 0) {
      mover(forca_Baixa, forca_Baixa);
    }
    LED4.turnOff();
    pararMotores();
    delay(300);
    while (lerSharpDigital(4) == 1) {
      mover(forca_Baixa, forca_Baixa);
    }
    LED4.turnOff();
    }
    if (lado == DIREITA) {
    LED4.turnOn();
    while (lerSharpDigital(3) == 0) {
      mover(forca, forca);
    }
    LED4.turnOff();
    pararMotores();
    delay(300);
    while (lerSharpDigital(3) == 1) {
      mover(forca, forca);
    }
    LED4.turnOff();
    }
  */
  mover(forca_Baixa, forca_Baixa);
  delay(1000);
  // Anda mais um pouco para a quarta curva ===========================
  pararMotores();
  delay(300);


  if (lado == ESQUERDA) {
    mover(forca_Baixa * -1, forca_Baixa * -1);
    delay(50);
    Curva90Graus(ESQUERDA, OBS);
  }
  if (lado == DIREITA) {
    mover(forca_Baixa * -1, forca_Baixa * -1);
    delay(100);
    Curva90Graus(DIREITA, OBS);
  }

  Servo1.attach(servo1);
  Servo1.write(180);
  /*
    pararMotores();
    delay(300);

    // vai para trás para se alinhar no objeto BEM devargazinho
    mover(forca_Baixa * -1, forca_Baixa * -1);
    delay(200);
    pararMotores();

    if (verificaGap() == true) {

      mover(forca_Curva * -1, forca_Curva);
      delay(700);
      if (verificaGap() == true) {
        mover(forca_Curva , forca_Curva * -1);
        delay(800);
      }
    }*/
}

/*
  makes obstacle optmized
*/
void Desvio(int lado) {

  mover(forca_Baixa * -1, forca_Baixa * -1);
  delay(400);

  int forca_inicial = 30;

  mover(-40, 80);
  delay(800);

  mover(50, 80);
  delay(1700);

  mover(110, -55);
  delay(2100);

  while (verificaGap() == true) {
    mover(forca_Baixa, forca_Baixa);
  }
  while (1) {
    pararMotores();
  }
}
/*
  init the claw
*/
void inicioGarra() {
  Servo1.attach(servo1);
  Servo1.write(40);
  Servo1.write(70);
  Servo1.write(120);
  /*
    int pos = 0;
    for (pos = 0; pos <= 90; pos -= 1) {
    Servo1.write(pos);
    delay(15);
    }*/
}
/*
  makes rescue
*/
void resgate() {
  Servo1.write(0);
  delay(1000);
  Servo1.write(45);
  delay(500);
  Servo1.write(0);
  delay(500);
  Servo1.write(45);
  delay(500);
  Servo1.write(65);
  delay(500);
  Servo1.write(90);
  delay(500);
  Servo1.write(120);
  delay(1000);
  /*
    int pos = 0;
    for (pos = 180; pos >= 0; pos -= 1) {
    Servo1.write(pos);
    delay(10);
    }
    for (pos = 0; pos <= 90; pos += 1) {
    Servo1.write(pos);
    delay(10);
    }
  */
}

/*
  makes the drop of victim
*/
void drop() {
  Servo1.write(80);
  Servo2.write(135);
  delay(100);
  Servo2.write(90);
  delay(1000);
  Servo2.write(45);
  delay(100);
  Servo2.write(0);
}
