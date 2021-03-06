#include <Arduino.h>

/*
  ROBOT GENERAL CONFIGURATION
*/

/************************ MOTORES ************************/
// Motores: Pololu 298:1 Micro Motor HPCB 12V 0.8A 100 RPM
// Utilizamos 4 motores porém eles estão em paralelo

// Motor ESQUERDO DIANETEIRO
#define MotorEsquerdoFrente 4
#define MotorEsquerdoTras 3

// Motor DIREITO DIANTEIRO

#define MotorDireitoFrente 6
#define MotorDireitoTras 5


void ligarMotores() {
  // MOTOR ESQUERDO DIANTEIRO
  pinMode(MotorEsquerdoFrente, OUTPUT);
  pinMode(MotorEsquerdoTras, OUTPUT);


  // MOTOR DIREITO DIANTEIRO
  pinMode(MotorDireitoFrente, OUTPUT);
  pinMode(MotorDireitoTras, OUTPUT);

}
// Encoder Motor Traseiro Esquerdo
// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
#define encoderEsquerdoA 19
#define encoderEsquerdoB 18

Encoder encoderEsquerdo(encoderEsquerdoA, encoderEsquerdoB);


/************************ SENSORES ************************/
// Sensores: Pololu QTR-8A reflectance sensor, Sharp 4-30cm, Divisor de Tensão, GY521 IMU
// PORTAS ANÁLOGICAS

// SENSORES ARRAY QTR-8-A

#define luminosidade1     A5 // SENSOR LINHA 1
#define luminosidade2     A6 // SENSOR LINHA 2
#define luminosidade3     A7 // SENSOR LINHA 3
#define luminosidade4     A8 // SENSOR LINHA 4
#define luminosidade5     A9 // SENSOR LINHA 5
#define luminosidade6     A10 // SENSOR LINHA 6
#define luminosidade7     A11// SENSOR LINHA 7
#define luminosidade8     A12  // SENSOR LINHA 8

#define sensorVerdeEsquerda A13
#define sensorVerdeDireita A14

int QTR[] = {
  0,
  luminosidade1,
  luminosidade2,
  luminosidade3,
  luminosidade4,
  luminosidade5,
  luminosidade6,
  luminosidade7,
  luminosidade8,

};

// SETTINGS PARA LER O ARRAY COM A FUNÇÃO .readLine()
#define NUM_SENSORS             8  // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading
//#define EMITTER_PIN             2  // emitter is controlled by digital pin 2

// Sensores de 0 a 8 que estão conectados nas portas analógicas
QTRSensorsAnalog qtra((unsigned char[]) {
  luminosidade1, luminosidade2, luminosidade3, luminosidade4, luminosidade5, luminosidade6, luminosidade7, luminosidade8
},
NUM_SENSORS, NUM_SAMPLES_PER_SENSOR);
unsigned int sensorValues[NUM_SENSORS];


// SENSORES SHARP 4-30cm
#define sharp1     A4 // SENSOR ESQUERDA ok
#define sharp2     A2 // SENSOR FRENTE - CIMA
#define sharp3     A3 // SENSOR DIREITA OK
#define sharp4     A0 // SENSOR FRENTE - BAIXO OK
#define sharp5     A1 // SENSOR FRENTE - ESQUERDA ok
#define sharp6     A15 // SENSOR FRENTE - DIREITA

int Sharp[] {
  0,
  sharp1,
  sharp2,
  sharp3,
  sharp4,
  sharp5,
  sharp6
};



// Tensão Bateria
#define VBAT    A0 // SENSOR DE TENSÃO DA BATERIA
#define VOLTAGE_LOW   11.5 // Tensão mínima
#define VOLTAGE_HIGH  14 // Tensão maxima


// 0 in ADC now means -5v. full ADC (1024) means 10v
AnalogVoltage myCustomVoltimeter(VBAT, 0, 15.1);
// Turns LED on
float pinVoltage = myCustomVoltimeter.readVoltage();

// Giroscópio e AcelerometrO Gy 521
MPU6050 mpu;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, '\r', '\n' };



// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
  mpuInterrupt = true;
}

bool IMU_init() {

  Wire.begin();
  TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)

  // initialize device
  Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();

  // verify connection
  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

  // load and configure the DMP
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    // turn on the DMP, now that it's ready
    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);

    // enable Arduino interrupt detection
    Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
    attachInterrupt(2, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }

}


unsigned long lastRun = 0;


#define angulo_curva_direita 90
#define angulo_curva_esquerda 60

#define angulo_curva_direita_45graus 45
#define angulo_curva_esquerda_45graus 35

int obstaculo_count = 0;

int i;
double gyroScale = 131;           // From datasheet
double timeStep, time, timePrev;
double arx, ary, arz, grx, gry, grz, gsx, gsy, gsz, rx, ry, rz;

#define ANGULO_RAMPA 11
/************************ SERVOS ************************/
// SERVO MOTORES PARA A GARRA - 180 graus - Servo 9g
// PORTAS PWM
#define servo1 8 // PORTA PWM
#define servo2 11 // PORTA PWM


Servo Servo1;
Servo Servo2;


/************************ INTERFACE ************************/
// Botões para iniciar ações
// PORTAS DIGITAIS
#define btn1 38 // LED Robô ligado
#define btn2 36 // LED Tensão bateria
#define btn3 34 // LED Teste do robô
#define btn4 32 // LED Calibração

int Botoes[] {
  0,
  btn1,
  btn2,
  btn3,
  btn4
};
DigitalIn Botao1(btn1);
DigitalIn Botao2(btn2);
DigitalIn Botao3(btn3);
DigitalIn Botao4(btn4);


// LEDs para identificação de ações
#define led1  53 // LED Robô ligado
#define led2  51 // LED Programação Rodando
#define led3  29 // LED
#define led4  27 // LED piscando na Calibração (Durante void setup())

int LEDs[] {
  0,
  led1,
  led2,
  led3,
  led4
};
DigitalOut LED1(led1);
DigitalOut LED2(led2);
DigitalOut LED3(led3);
DigitalOut LED4(led4);

// Buzzer para fazer barulinho
#define buzzer 25
DigitalOut Buzzer(buzzer);


/************************ ESTADOS *********************/
#define ESQUERDA -1
#define DIREITA   1
#define ST_PRINCIPAL 2
#define ST_RAMPA     4
#define CIMA 1
#define BAIXO 0
#define IDDLE   0

#define R 0
#define G 1
#define B 2

#define OBS 1
#define LIN   0

#define QTRSensor 1
#define VERDESensor 2
#define SHARPSensor 3
#define ALLSensor 4
#define MPU 5

/************************ VARIÁVEIS DO ROBÔ ************************/
int pulsosPorRotacao = 3550; // um giro do encoder equivale a isso de pulsos
float pi = 3.141592;

float distanciaEntreEixos = 14.4; // 144 mm
float raioRoda = 3.5; // 35 mm
float circunferenciaRoda = 2 * pi * raioRoda;

int forcaPID = 65;
int forcaRampa = 200;
int forca_Curva = 120; // forca da curva no obstaculo
int forca = 110;
int forca_Baixa = 70;

int P; int I; int D; int ganho;
double motorB; double motorC;

unsigned long tempoinicial = millis();

float KP = 0.13;
float KI = 0;
float KD = 0.12; // Constante da Derivada


//0.25
// 0.16
int branco = 100; // Força normal para seguir linha
int preto = 550; // Força para rampa
int silver_tape = 300; // luminosidade da fita silver_tape

int offset = ((branco + preto) / 2); // media seguidor

int setPoint = 3500;

int auxGyro = 0;
