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

/************************ SENSORES ************************/
// Sensores: Pololu QTR-8A reflectance sensor, Sharp 4-30cm, Divisor de Tensão, GY521 IMU
// PORTAS ANÁLOGICAS

// SENSORES ARRAY QTR-8-A
#define luminosidade1     A12 // SENSOR LINHA 1
#define luminosidade2     A11 // SENSOR LINHA 2
#define luminosidade3     A10 // SENSOR LINHA 3
#define luminosidade4     A9 // SENSOR LINHA 4
#define luminosidade5     A8 // SENSOR LINHA 5
#define luminosidade6     A7 // SENSOR LINHA 6
#define luminosidade7     A6 // SENSOR LINHA 7
#define luminosidade8     A5 // SENSOR LINHA 8

#define sensorVerdeEsquerda A15
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
#define sharp1     A3 // SENSOR ESQUERDA
#define sharp2     A2 // SENSOR FRENTE - CIMA
#define sharp3     A1 // SENSOR DIREITA
#define sharp4     A0 // SENSOR FRENTE - BAIXO
#define sharp5     A15 // SENSOR FRENTE - 
#define sharp6     A13 // SENSOR FRENTE - DIREITA

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

#define BTN_REDUTOR A15

// 0 in ADC now means -5v. full ADC (1024) means 10v
AnalogVoltage myCustomVoltimeter(VBAT, 0, 15.1);
// Turns LED on
float pinVoltage = myCustomVoltimeter.readVoltage();

// Giroscópio e AcelerometrO Gy 521

void IMU_init();

MPU6050 mpu;

// These are my MPU6050 Offset numbers: for mpu.setXGyroOffset()
// supply your own gyro offsets here, scaled for min sensitivity use MPU6050_calibration.ino <<< download to calibrate your MPU6050 put the values the probram returns below
//                       XA      YA      ZA      XG      YG      ZG
int MPUOffsets[6] = {  -4232,  -706,   1729,    173,    -94,     100};

#define LED_PIN 13 // 

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
  mpuInterrupt = true;
}

// ================================================================
// ===                      MPU DMP SETUP                       ===
// ================================================================
int FifoAlive = 0; // tests if the interrupt is triggering
int IsAlive = -20;     // counts interrupt start at -20 to get 20+ good values before assuming connected
// MPU control/status vars
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
float Yaw, Pitch, Roll; // in degrees



void MPU6050Connect() {
  static int MPUInitCntr = 0;
  // initialize device
  mpu.initialize(); // same
  // load and configure the DMP
  devStatus = mpu.dmpInitialize();// same

  if (devStatus != 0) {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)

    //char * StatStr[5] { "No Error", "initial memory load failed", "DMP configuration updates failed", "3", "4"};

    MPUInitCntr++;

    // Serial.print(F("MPU connection Try #"));
    Serial.println(MPUInitCntr);
    Serial.print(F("DMP Initialization failed (code "));
    //Serial.print(StatStr[devStatus]);
    // Serial.println(F(")"));

    if (MPUInitCntr >= 10) return; //only try 10 times
    delay(1000);
    MPU6050Connect(); // Lets try again
    return;
  }

  mpu.setXAccelOffset(MPUOffsets[0]);
  mpu.setYAccelOffset(MPUOffsets[1]);
  mpu.setZAccelOffset(MPUOffsets[2]);
  mpu.setXGyroOffset(MPUOffsets[3]);
  mpu.setYGyroOffset(MPUOffsets[4]);
  mpu.setZGyroOffset(MPUOffsets[5]);

  Serial.println(F("Enabling DMP..."));
  mpu.setDMPEnabled(true);
  // enable Arduino interrupt detection

  Serial.println(F("Enabling interrupt detection (Arduino external interrupt pin 2 on the Uno)..."));
  attachInterrupt(0, dmpDataReady, FALLING); //pin 2 on the Uno

  mpuIntStatus = mpu.getIntStatus(); // Same
  // get expected DMP packet size for later comparison
  packetSize = mpu.dmpGetFIFOPacketSize();
  delay(1); // Let it Stabalize
  mpu.resetFIFO(); // Clear fifo buffer
  mpu.getIntStatus();
  mpuInterrupt = false; // wait for next interrupt

}
// ================================================================
// ===                      i2c SETUP Items                     ===
// ================================================================
void i2cSetup() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif
}

#define angulo_rampa_subida -21
#define angulo_rampa_descida 20


#define angulo_curva_direita 78
#define angulo_curva_esquerda 78
#define angulo_curva_direita_obs 85
#define angulo_curva_esquerda_obs 76

#define angulo_curva_direita_obs2 42
#define angulo_curva_esquerda_obs2 42


int obstaculo_count = 0;

int i;
double gyroScale = 131;           // From datasheet
double timeStep, time, timePrev;
double arx, ary, arz, grx, gry, grz, gsx, gsy, gsz, rx, ry, rz;

#define ANGULO_RAMPA 19
/************************ SERVOS ************************/
// SERVO MOTORES PARA A GARRA - 180 graus - Servo 9g
// PORTAS PWM
#define servo1 11 // PORTA PWM
#define servo2 10 // PORTA PWM


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


/************************ VARIÁVEIS DO ROBÔ ************************/
int forca = 110;
int forca_Baixa = 70;

int forcaRampa = 200;
int forca_Curva = 120; // forca da curva no obstaculo

int P; int I; int D; int ganho;
double motorB; double motorC;

unsigned long tempoinicial = millis();

float KP = 0.15;
float KI = 0;
float KD = 0.2; // Constante da Derivada

float KP_redutor = 0.2;
float KI_redutor = 0;
float KD_redutor = 0;

float forca_Redutor = 200;

int branco = 100; // Força normal para seguir linha
int preto = 650; // Força para rampa
int silver_tape = 300; // luminosidade da fita silver_tape

int forcaPID = 75;

int offset = ((branco + preto) / 2); // media seguidor

int setPoint = 3500;


