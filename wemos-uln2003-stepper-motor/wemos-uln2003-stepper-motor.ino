/*
  BYJ48 Stepper motor  Connect :
  ULN2003 --- Arduino Uno
  ========   ============== 
  IN1 >> D8
  IN2 >> D9
  IN3 >> D10
  IN4 >> D11
  VCC ... 5V Prefer to use external 5V Source
  Gnd

  ULN2003 --- ESP-12F(WeMos D1 mini)
  ========   =======================
  IN1 >> GPIO 14 (D5)
  IN2 >> GPIO 12 (D6)
  IN3 >> GPIO 13 (D7)
  IN4 >> GPIO 15 (D8)
  VCC(+) >> ... 5V Prefer to use external 5V Source
  Gnd(-) >> GND (G)

*/

// Arduino UNO Pins:
//#define IN1 8
//#define IN2 9
//#define IN3 10
//#define IN4 11

// ESP-12F(WeMos D1 mini)
#define IN1 14 //GPIO 14
#define IN2 12 //GPIO 12
#define IN3 13 //GPIO 13
#define IN4 15 //GPIO 15

const int NBSTEPS = 4095;
const int STEPTIME = 900;
int Step = 0;
boolean Clockwise = true;

int arrayDefault[4] = {LOW, LOW, LOW, LOW};

int stepsMatrix[8][4] = {
  {LOW, LOW, LOW, HIGH},
  {LOW, LOW, HIGH, HIGH},
  {LOW, LOW, HIGH, LOW},
  {LOW, HIGH, HIGH, LOW},
  {LOW, HIGH, LOW, LOW},
  {HIGH, HIGH, LOW, LOW},
  {HIGH, LOW, LOW, LOW},
  {HIGH, LOW, LOW, HIGH},
};

unsigned long lastTime = 0L;
unsigned long time = 0L;

void writeStep(int outArray[4]);
void stepper();
void setDirection();

void setup() {
  //Arduino UNO
  //Serial.begin(9600);

  //ESP8266 
  Serial.begin(115200);
  Serial.println("Starting...");
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  unsigned long currentMicros;
  int stepsLeft = NBSTEPS;
  time = 0;
  lastTime = micros();
  while (stepsLeft > 0) {
    currentMicros = micros();
    if (currentMicros - lastTime >= STEPTIME) {
      stepper();
      time += micros() - lastTime;
      lastTime = micros();
      stepsLeft--;
    }
    delay(1);
  }
  Serial.println(time);
  Serial.println("Wait...!");
  delay(2000);
  Clockwise = !Clockwise;
  stepsLeft = NBSTEPS;
}

void writeStep(int outArray[4]) {
  digitalWrite(IN1, outArray[0]);
  digitalWrite(IN2, outArray[1]);
  digitalWrite(IN3, outArray[2]);
  digitalWrite(IN4, outArray[3]);
}

void stepper() {
  if ((Step >= 0) && (Step < 8)) {
    writeStep(stepsMatrix[Step]);
  } else {
    writeStep(arrayDefault);
  }
  setDirection();
}

void setDirection() {
  (Clockwise == true) ? (Step++) : (Step--);

  if (Step > 7) {
    Step = 0;
  } else if (Step < 0) {
    Step = 7;
  }
}
