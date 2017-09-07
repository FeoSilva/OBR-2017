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
    //Curva90Graus(ESQUERDA, OBS);
    mover(forca * -1, forca);
    delay(700);
    mover(forca * -1, forca * -1);
    delay(1000);
    pararMotores();
    delay(100);
    andarCM(5, 60);
    pararMotores();
    //Curva90Graus(DIREITA, OBS);
    mover(forca, forca * -1);
    delay(820);
    pararMotores();
    aux = 1;

    // VERIFICAÇÕES E AÇÕES DA PRIMEIRA RONDA___________________
    while (aux == 1) {
      mover(60, 60);
      if (verificaVitima() == true) {
        pararMotores();
        resgate();
        pararMotores();
        delay(200);
        /*
          pararMotores();

          //Curva45Graus(DIREITA, OBS);
          mover(forca, forca * -1);
          delay(410);
          if (verificaVitima() == false) {
          //Curva45Graus(ESQUERDA, OBS);
          mover(forca * -1, forca);
          delay(350);
          pararMotores();
          resgate();
          }
          else {
          //Curva45Graus(ESQUERDA, OBS);
          mover(forca * -1, forca);
          delay(350);
          resgate();
        }
        else {
          Curva45Graus(ESQUERDA, OBS);

          pararMotores();
          delay(100);
          //Curva90Graus(ESQUERDA, OBS);
          mover(forca * -1, forca);
          delay(700);
          pararMotores();
          delay(100);
          mover(forca, forca);
          delay(1400);
          pararMotores();
          delay(100);
          //Curva90Graus(ESQUERDA, OBS);
          mover(forca * -1, forca);
          delay(700);
          pararMotores();
          delay(100);
          mover(forca * -1, forca * -1);
          delay(1000);
          pararMotores();
          res = 2;
          }*/
        mover(60, 60);
      }

      if (verificaVitimaEsquerda() == true) {
        pararMotores();
        //Curva90Graus(ESQUERDA, OBS);
        mover(forca * -1, forca);
        delay(750);
        pararMotores();

        if (verificaVitima == true) {
          resgate();
          delay(200);
        }
        //Curva90Graus(DIREITA, OBS);
        mover(forca, forca * -1);
        delay(820);
        pararMotores();
        mover(60, 60);
      }
      if (verificaParede() == true) {
        pararMotores();
        //Curva90Graus(ESQUERDA, OBS);
        mover(forca * -1, forca);
        delay(700);
        pararMotores();
        delay(100);
        mover(forca, forca);
        delay(1000);
        pararMotores();
        delay(100);
        //Curva90Graus(ESQUERDA, OBS);
        mover(forca * -1, forca);
        delay(700);
        pararMotores();
        delay(100);
        mover(forca * -1, forca * -1);
        delay(300);
        pararMotores();
        res = 2;
      }


      // VERIFICAÇÕES E AÇÕES DA SEGUNDA RONDA_________________

      while (res == 2) {
        mover(60, 60);
        if (verificaVitima() == true) {
          pararMotores();

          resgate();
          pararMotores();
          delay(200);
          mover(60, 60);

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
        /*
          if (verificaVitimaEsquerda() == true) {
          pararMotores();
          Curva90Graus(ESQUERDA, OBS);
          pararMotores();
          resgate();
          delay(1000);
          Curva90Graus(DIREITA, OBS);
          pararMotores();
          mover(60, 60);
          }
        */
        if (verificaParede() == true) {
          pararMotores();
          //Curva90Graus(DIREITA, OBS);
          mover(forca, forca * -1);
          delay(820);
          pararMotores();
          delay(100);
          mover(60, 60);
          res = 3;
        }

        // RESGATE _______________________________________________
        while (res == 3) {
          if (verificaParede() == true) {
            pararMotores();
            //Curva90Graus(DIREITA, OBS);
            mover(forca, forca * -1);
            delay(820);
            mover(forca * -1, forca * -1);
            delay(700);
            mover(60, 60);
            res++;
          }
        }
        while ((res >= 4) && (res <= 5)) {
          if (verificaParede() == true) {
            pararMotores();
            //Curva90Graus(DIREITA, OBS);
            mover(forca, forca * -1);
            delay(820);
            mover(forca * -1, forca * -1);
            delay(700);
            mover(60, 60);
            res++;
          }
          /*
            if (verificaVitimaDireita() == true) {
            pararMotores();
            Curva90Graus(DIREITA, OBS);
            mover(forca, forca * -1);
            delay(820);
            pararMotores();
            resgate();
            delay(1000);
            Curva90Graus(ESQUERDA, OBS);
            pararMotores();
            mover(60, 60);
            }*/

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

    //LED1.turnOn();
    //Buzzer.turnOff();
    //lerTodosSensores(ALLSensor);


    if (verificaEncruzilhada() == true) {
      pararMotores();
      delay(500);
      Serial.println("************** ENCRUZILHADA ***************");


      int e = 0;

      for (int i = 0; i < 10; i++) { // make the calibration take about 10 seconds
        //Serial.println(analogRead(A15));
        //lerTodosSensores(ALLSensor);
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


    /*
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
    */
/*
    if (verificaT() != IDDLE) {
      pararMotores();
      delay(500);
      Serial.println("************** T ***************");


      int t = 0;
      for (int i = 0; i < 10; i++) { // make the calibration take about 10 seconds

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

    }*/

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
