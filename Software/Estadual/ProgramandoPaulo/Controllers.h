#include <Arduino.h>
#include <QTRSensors.h>


/*
  FUNÇÕES de Controles DO ROBÔ
*/

int aux = 0;
int res = 0;

/*
  makes the third ward

  true to enable
*/

void Resgate(boolean condicao) {

  while (condicao == true) {

    // INICIO ________________________________________________

    mover(forca, forca);
    delay(300);
    pararMotores();
    //inicioGarra();
    delay(200);
    Curva90Graus(ESQUERDA, OBS);
    mover((forca + 20) * -1, (forca + 20) * -1);
    delay(1000);
    pararMotores();
    delay(100);
    andarCM(5, 60);
    pararMotores();
    delay(500);
    //Curva90Graus(DIREITA, OBS);
    curvaEncoder(45, 80, DIREITA);
    pararMotores();
    aux = 1;

    // VERIFICAÇÕES E AÇÕES DA PRIMEIRA RONDA___________________
    while (aux == 1) {
      mover(68, 60);
      if (verificaVitima() == true) {
        pararMotores();
        curvaEncoder(45, forca, DIREITA);
        pararMotores();
        delay(300);
        if (confirmaVitima() == true) {
          delay(300);
          curvaEncoder(45, forca, ESQUERDA);
          pararMotores();
          delay(300);
          resgate();
        } else {
          base = 1;
          curvaEncoder(45, forca, ESQUERDA);
          pararMotores();
          delay(100);
          Curva90Graus(ESQUERDA, OBS);
          pararMotores();
          delay(200);
          andarCM(40, 60);
          pararMotores();
          delay(100);
          Curva90Graus(ESQUERDA, OBS);
          pararMotores();
          delay(100);
          mover((forca + 20) * -1, (forca + 20) * -1);
          delay(2000);
          pararMotores();
          res = 2;
        }
        mover(68, 60);
      }

      if (verificaVitimaEsquerda() == true) {
        pararMotores();
        mover(forca * -1, forca * -1);
        delay(200);
        pararMotores();
        Curva90Graus(ESQUERDA, OBS);
        pararMotores();
        resgate();
        delay(200);
        Curva90Graus(DIREITA, OBS);
        pararMotores();
        mover(68, 60);
      }
      if (verificaParede() == true) {
        pararMotores();
        Curva90Graus(ESQUERDA, OBS);
        pararMotores();
        delay(100);
        andarCM(35, 60);
        pararMotores();
        if (verificaAreaResgate() == false) {
          base = 2;
          andarCM(10, 60);
        }
        delay(100);
        Curva90Graus(ESQUERDA, OBS);
        pararMotores();
        delay(100);
        mover(forca * -1, forca * -1);
        delay(1000);
        pararMotores();
        mover(forca, forca);
        delay(500);
        res = 2;
      }


      // VERIFICAÇÕES E AÇÕES DA SEGUNDA RONDA_________________

      while (res == 2) {
        mover(68, 60);
        if (verificaVitima() == true) {
          pararMotores();
          curvaEncoder(45, forca, DIREITA);
          pararMotores();
          delay(300);
          if (confirmaVitima() == true) {
            delay(300);
            curvaEncoder(45, forca, ESQUERDA);
            pararMotores();
            delay(300);
            resgate();
          } else {
            pararMotores();
            mover(75 * -1, 60 * -1);
            delay(300);
            pararMotores();
            //Curva45Graus(DIREITA, OBS);
            mover(forca, forca * -1);
            delay(410);
            mover(forca, forca);
            delay(500);
            pararMotores();
            Curva90Graus(ESQUERDA, OBS);
            mover(forca, forca);
            delay(500);
            pararMotores();
            mover(forca, forca);
            delay(300);
            pararMotores();
            andarCM(5, -60);
            mover(150, 150);
            drop();
            res = 6;
          }
          mover(68, 60);
        }

        if (verificaVitimaEsquerda() == true) {
          pararMotores();
          mover(forca * -1, forca * -1);
          delay(200);
          pararMotores();
          Curva90Graus(ESQUERDA, OBS);
          pararMotores();
          resgate();
          delay(200);
          Curva90Graus(DIREITA, OBS);
          pararMotores();
          mover(68, 60);
        }
        if (verificaVitimaDireita() == true) {
          pararMotores();
          mover(forca * -1, forca * -1);
          delay(200);
          pararMotores();
          Curva90Graus(DIREITA, OBS);
          pararMotores();
          resgate();
          delay(200);
          Curva90Graus(ESQUERDA, OBS);
          pararMotores();
          mover(68, 60);
        }
        if (verificaParede() == true) {
          pararMotores();
          delay(500);
          andarCM(8, forca * -1);
          pararMotores();
          delay(500);
          Curva90Graus(DIREITA, OBS);
          pararMotores();
          delay(100);
          mover(68, 60);
          res = 3;
        }

        // RESGATE _______________________________________________
        while (res == 3) {
          mover(68, 60);
          if (verificaParede() == true) {
            Curva90Graus(DIREITA, OBS);
            mover((forca + 20) * -1, (forca + 20) * -1);
            delay(1000);
            pararMotores();
            res++;
          }
          if (verificaAreaResgate() == true) {
            pararMotores();
            mover(75 * -1, 60 * -1);
            delay(300);
            pararMotores();
            //Curva45Graus(DIREITA, OBS);
            mover(forca, forca * -1);
            delay(410);
            mover(forca, forca);
            delay(500);
            pararMotores();
            Curva90Graus(ESQUERDA, OBS);
            mover(forca, forca);
            delay(500);
            pararMotores();
            mover(forca, forca);
            delay(300);
            pararMotores();
            andarCM(5, -60);
            mover(150, 150);
            drop();
            res = 6;
          }
        }
      }
      while ((res == 4)) {
        mover(68, 60);
        if (verificaParede() == true) {
          pararMotores();
          Curva90Graus(DIREITA, OBS);
          mover((forca + 20) * -1, (forca + 20) * -1);
          delay(700);
          res++;
        }
        if (verificaVitimaDireita() == true) {
          pararMotores();
          Curva90Graus(DIREITA, OBS);
          pararMotores();
          resgate();
          delay(1000);
          Curva90Graus(ESQUERDA, OBS);
          pararMotores();
          mover(60, 60);
        }
        if (verificaAreaResgate() == true && base >= 1) {
          pararMotores();
          delay(300);
          resgate();
        }
        if (verificaAreaResgate() == true && base == 0) {
          pararMotores();
          mover(75 * -1, 60 * -1);
          delay(300);
          pararMotores();
          //Curva45Graus(DIREITA, OBS);
          mover(forca, forca * -1);
          delay(410);
          mover(forca, forca);
          delay(500);
          pararMotores();
          Curva90Graus(ESQUERDA, OBS);
          mover(forca, forca);
          delay(500);
          pararMotores();
          mover(forca, forca);
          delay(300);
          pararMotores();
          andarCM(5, -60);
          mover(150, 150);
          drop();
          res = 6;
        }
      }
      while ((res == 5)) {
        mover(68, 60);
        if (verificaVitimaDireita() == true) {
          pararMotores();
          Curva90Graus(DIREITA, OBS);
          pararMotores();
          resgate();
          delay(1000);
          Curva90Graus(ESQUERDA, OBS);
          pararMotores();
        }
        if (verificaAreaResgate() == true) {
          pararMotores();
          mover(75 * -1, 60 * -1);
          delay(300);
          pararMotores();
          //Curva45Graus(DIREITA, OBS);
          mover(forca, forca * -1);
          delay(410);
          mover(forca, forca);
          delay(500);
          pararMotores();
          Curva90Graus(ESQUERDA, OBS);
          mover(forca, forca);
          delay(500);
          pararMotores();
          mover(forca, forca);
          delay(300);
          pararMotores();
          andarCM(5, -60);
          mover(150, 150);
          drop();
          res = 6;
        }
      }
      if (res >= 6) {
        while (1) {
          pararMotores();
        }
      }

    }
    condicao = false;

  }
}



void Rampa(boolean condicao) {

  Serial.println("******************************* RAMPA *****************************");
  LED2.turnOff();
  while (condicao == true) {
    LED3.turnOn();

    pararMotores();
    delay(300);
    Servo1.attach(servo1);
    Servo1.write(160);
    pararMotores();
    delay(600);

    // famoso gás
    mover(150, 150);
    delay(200);
    Buzzer.turnOn();
    //while (verificaSilverTap() == false && verificaEncruzilhada() == false) {
    while (lerSharp(1) > 100) {

      Serial.println(lerSharp(1));
      PID(0.8, 0, 0.0, 255, 3500);

      if (verificaGap() == true) {
        Serial.println("******************************* GAP *****************************");
        while (verificaGap() == true && lerSharp(1) > 100) {
          Gap(ST_RAMPA);
        }
      }
    }
    pararMotores();
    delay(1000);
    andarCM(5, forca);

    LED3.turnOff();

    Buzzer.turnOff();
    condicao = false;
    Resgate(true);


  }

}


/*
  makes the second ward

  true to enable
*/
void Seguidor(boolean condicao) {

  while (condicao == true) {


    if (verificaRampa() == true) {
      condicao = false;
      Rampa(true);
    }
    // SEGUIDOR DE LINHA P.I.D.
    PID(KP, KI, KD, forcaPID, setPoint);

    //Serial.println("************** PID ***************");

    LED2.turnOn();
    //Buzzer.turnOff();
    lerTodosSensores(ALLSensor);


    if (verificaEncruzilhada() == true) {
      pararMotores();
      delay(500);
      Serial.println("************** ENCRUZILHADA ***************");


      int e = 0;

      for (int i = 0; i < 10; i++) { // make the calibration take about 10 seconds
        //Serial.println(analogRead(A15));
        lerTodosSensores(ALLSensor);
        if (verificaVerde(ESQUERDA) == true && e == 0) {
          Serial.println("******************************* VERDE ESQUERDA *****************************");
          Verde(ESQUERDA);
          e = 1;
        } else if (verificaVerde(DIREITA) == true && e == 0) {
          Serial.println("******************************* VERDE DIREITA *****************************");
          Verde(DIREITA);
          e = 1;
        }

      }

      if (e == 0) {
        while (verificaEncruzilhada() == true) {
          Encruzilhada();
        }
      }
      Serial.println("************** ACABOU A ENCRUZILHADA ***************");
    }



    if (verificaObstaculo() == true) {
      obstaculo_count++;
      Serial.println("************** OBSTÁCULO ***************");
      if (obstaculo_count == 1) {
        Obstaculo(DIREITA);
      } else if (obstaculo_count == 2) {
        Obstaculo(ESQUERDA);
      } else if (obstaculo_count == 3) {
        Desvio(ESQUERDA);
      } else {
        Obstaculo(ESQUERDA);
      }
    }



    if (verificaT() != IDDLE) {
      pararMotores();
      delay(500);

      if (verificaT() == DIREITA) {
        Serial.println("************** T - DIREITA ***************");
      } else if (verificaT() == ESQUERDA) {
        Serial.println("************** T - ESQUERDA ***************");
      }


      int t = 0;
      for (int i = 0; i < 10; i++) { // make the calibration take about 10 seconds


        lerTodosSensores(ALLSensor);
        if (verificaVerde(ESQUERDA) == true && t == 0) {
          Serial.println("******************************* VERDE ESQUERDA *****************************");
          Verde(ESQUERDA);
          t = 1;
        } else if (verificaVerde(DIREITA) == true && t == 0) {
          Serial.println("******************************* VERDE DIREITA *****************************");
          Verde(DIREITA);
          t = 1;
        }
      }

      if (t == 0) {
        T(verificaT());
      }
      Serial.println("************** ACABOU O T ***************");


    }

    if (verificaGap() == true) {
      encoderEsquerdo.write(0);
      Serial.println("******************************* GAP *****************************");
      int g = 0;

      while (verificaGap() == true && g == 0) {
        //lerTodosSensores();
        Gap(ST_PRINCIPAL);
        if (encoderEsquerdo.read() >= 1500) {
          g = 1;
        }
      }
      pararMotores();
      if (g == 1) {
        while (verificaGap() == true) {
          mover(forca * -1, forca * -1);
        }
      }
      Serial.println("******************************* ACABOU O GAP *****************************");
    }


  }

};
