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
void Salao3(boolean condicao) {

  while (condicao == true) {

    // INICIO ________________________________________________
    mover(75, 60);
    delay(500);
    pararMotores();
    inicioGarra();
    delay(200);
    Curva90Graus(ESQUERDA, OBS);
    mover(forca * -1, forca * -1);
    delay(1000);
    pararMotores();
    delay(100);
    mover(75, 60);
    /*
      if (verificaVitimaDireita() == true) {
      pararMotores();
      Curva90Graus(DIREITA, OBS);
      pararMotores();
      resgate();
      delay(1000);
      Curva90Graus(ESQUERDA, OBS);
      pararMotores();
      mover(75, 60);
      }*/
    delay(1000);
    pararMotores();
    Curva90Graus(DIREITA, OBS);
    pararMotores();
    aux = 1;

    // VERIFICAÇÕES E AÇÕES DA PRIMEIRA RONDA___________________
    while (aux == 1) {
      mover(75, 60);
      if (verificaVitima() == true) {

        pararMotores();
        Curva45Graus(DIREITA, OBS);
        if (verificaVitima() == false) {
          Curva45Graus(ESQUERDA, OBS);
          pararMotores();
          resgate();
        }
        else {
          Curva45Graus(ESQUERDA, OBS);
          pararMotores();
          delay(100);
          Curva90Graus(ESQUERDA, OBS);
          pararMotores();
          delay(100);
          mover(65, 70);
          delay(1400);
          pararMotores();
          delay(100);
          Curva90Graus(ESQUERDA, OBS);
          pararMotores();
          delay(100);
          mover(forca * -1, forca * -1);
          delay(1000);
          pararMotores();
          res = 2;
        }
        mover(75, 60);
      }

      if (verificaVitimaEsquerda() == true) {
        pararMotores();
        /*
          mover(forca * -1, forca * -1);
          delay(200);
          pararMotores();
        */
        Curva90Graus(ESQUERDA, OBS);
        pararMotores();
        resgate();
        Curva90Graus(DIREITA, OBS);
        pararMotores();
        mover(75, 60);
      }
      if (verificaParede() == true) {
        pararMotores();
        Curva90Graus(ESQUERDA, OBS);
        pararMotores();
        delay(100);
        mover(75, 60);
        delay(1500);
        pararMotores();
        delay(100);
        Curva90Graus(ESQUERDA, OBS);
        pararMotores();
        delay(100);
        mover(forca * -1, forca * -1);
        delay(250);
        pararMotores();
        res = 2;
      }

      // VERIFICAÇÕES E AÇÕES DA SEGUNDA RONDA_________________

      while (res == 2) {
        mover(75, 60);
        if (verificaVitima() == true) {
          pararMotores();
          Curva45Graus(DIREITA, OBS);
          if (verificaVitima() == false) {
            Curva45Graus(ESQUERDA, OBS);
            pararMotores();
            resgate();
          } else {
            pararMotores();
            drop();
            res = 6;
          }
          mover(75, 60);
        }
        if (verificaVitimaEsquerda() == true) {
          pararMotores();
          Curva90Graus(ESQUERDA, OBS);
          pararMotores();
          resgate();
          delay(1000);
          Curva90Graus(DIREITA, OBS);
          pararMotores();
          mover(75, 60);
        }
        if (verificaParede() == true) {
          pararMotores();
          Curva90Graus(DIREITA, OBS);
          pararMotores();
          delay(100);
          mover(75, 60);
          res = 3;
        }

        // RESGATE _______________________________________________
        while (res == 3) {
          if (verificaParede() == true) {
            pararMotores();
            Curva90Graus(DIREITA, OBS);
            mover(forca * -1, forca * -1);
            delay(700);
            mover(75, 60);
            res++;
          }
        }
        while ((res >= 4) && (res <= 5)) {
          if (verificaParede() == true) {
            pararMotores();
            Curva90Graus(DIREITA, OBS);
            mover(forca * -1, forca * -1);
            delay(700);
            mover(75, 60);
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
            mover(75, 60);
          }
          if (verificaAreaResgate() == true) {
            pararMotores();
            mover(75 * -1, 60 * -1);
            delay(300);
            pararMotores();
            Curva45Graus(DIREITA, OBS);
            mover(75, 60);
            delay(500);
            pararMotores();
            Curva90Graus(ESQUERDA, OBS);
            mover(75, 60);
            delay(500);
            pararMotores();
            mover(75, 60);
            delay(300);
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
}

/*
  makes the second ward

  true to enable
*/
void Seguidor(boolean condicao) {

  while (condicao == true) {

    /*if (verificaRampa() == true) {
      Serial.println("******************************* RAMPA *****************************");
      pararMotores();
      delay(300);
      Servo1.attach(servo1);
      Servo1.write(40);
      pararMotores();
      delay(600);

      // famoso gás
      mover(150, 150);
      delay(200);
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




      }*/
    // SEGUIDOR DE LINHA P.I.D.
    PID(KP, KI, KD, forcaPID, setPoint);

    //Serial.println("************** PID ***************");
    LED1.turnOn();
    Buzzer.turnOff();
    lerTodosSensores(QTR);


    if (verificaRedutor() == true) {
      Redutor();
    }


    if (verificaEncruzilhada() == true) {
      pararMotores();
      delay(800);
      Serial.println("************** ENCRUZILHADA ***************");


      int e = 0;

      for (int i = 0; i < 10; i++) { // make the calibration take about 10 seconds
        lerTodosSensores(QTR);

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
        Obstaculo(ESQUERDA);

      } else if (obstaculo_count == 2) {
        Obstaculo(ESQUERDA);

        Servo1.attach(servo1);
        Servo1.write(180);
      } else if (obstaculo_count == 3) {
        Desvio(ESQUERDA);
      } else {
        Obstaculo(ESQUERDA);
      }
    }

    if (verificaT() != IDDLE) {
      pararMotores();
      delay(800);
      Serial.println("************** T ***************");


      int t = 0;
      for (int i = 0; i < 10; i++) { // make the calibration take about 10 seconds
        lerTodosSensores(QTR);
      }

      if (t == 0) {
        T(verificaT());
      }
      Serial.println("************** ACABOU O T ***************");

    }

    if (verificaGap() == true) {
      Serial.println("******************************* GAP *****************************");
      while (verificaGap() == true) {
        //lerTodosSensores();
        Gap(ST_PRINCIPAL);
      }
      Serial.println("******************************* ACABOU O GAP *****************************");
    }


  }


};
