#include <MobaTools.h>

//dir 2 pwm 3
const byte GoogleDriveStep = A0;
const byte GoogleDriveDir =7 ;
const int stepsPerRevGoogle = 400;

MoToStepper GoogleDriveStepper(stepsPerRevGoogle, STEPDIR);

const int DCMotorR3dir = 4;
const int DCMotorR3pwm = 5;
const int DCMotorR2dir = 2;
const int DCMotorR2pwm = 3;
const int DCMotorR1dir = 8;
const int DCMotorR1pwm = A1;

const byte receiver_E = 10;
const byte receiver_F = 11;

int readValueGoogle, readValueDC;

void setup() {

  Serial.begin(115200);

  GoogleDriveStepper.attach(GoogleDriveStep, GoogleDriveDir);
  GoogleDriveStepper.setZero();

  pinMode(receiver_E, INPUT);
  pinMode(receiver_F, INPUT);

  pinMode(DCMotorR1dir, OUTPUT);
  pinMode(DCMotorR1pwm, OUTPUT);
  pinMode(DCMotorR2dir, OUTPUT);
  pinMode(DCMotorR2pwm, OUTPUT);
  pinMode(DCMotorR3pwm, OUTPUT);
  pinMode(DCMotorR3dir, OUTPUT);


  //  pinMode(DCMotorR3dir, OUTPUT);
  //  pinMode(DCMotorR3pwm, OUTPUT);
}

void loop() {

  readValueGoogle = pulseIn(receiver_E, HIGH);
  Serial.print("ReadValueGoogle PulseIn: ");
  Serial.println(readValueGoogle);

  readValueDC = pulseIn(receiver_F, HIGH);
  Serial.print("ReadValueDC PulseIn: ");
  Serial.println(readValueDC);

  if (readValueGoogle == 0)
    readValueGoogle = 0;

  else if (readValueGoogle > 1100 && readValueGoogle < 1600)
    readValueGoogle = 0;

  else if (readValueGoogle > 1600)
    readValueGoogle = 1;

  else
    readValueGoogle = -1;

  Serial.print("ReadValueGoogle: ");
  Serial.println(readValueGoogle);

  if (readValueDC == 0)
    readValueDC = 0;
  else if (readValueDC > 1200 && readValueDC < 1600)
    readValueDC = 0;
  else if (readValueDC > 1600)
    readValueDC = 1;
  else
    readValueDC = -1;

  Serial.print("ReadValueDC: ");
  Serial.println(readValueDC);

  if (readValueGoogle == 1) {
    GoogleDriveStepper.setSpeed(2000);
    GoogleDriveStepper.rotate(1);
    digitalWrite(DCMotorR1dir, HIGH);
    analogWrite(DCMotorR1pwm, 255);
  }

  else if (readValueGoogle == -1) {
    GoogleDriveStepper.setSpeed(2000);
    GoogleDriveStepper.rotate(-1);
    analogWrite(DCMotorR1pwm, 0);
  }

  else{
    GoogleDriveStepper.stop();
    analogWrite(DCMotorR1pwm, 0);
  }

if (readValueDC == -1) {
  digitalWrite(DCMotorR2dir, HIGH);
  analogWrite(DCMotorR2pwm, 255);
  digitalWrite(DCMotorR3dir, HIGH);
  analogWrite(DCMotorR3pwm, 255);
}

else if (readValueDC == 0) {

  analogWrite(DCMotorR2pwm, 0);
  analogWrite(DCMotorR3pwm, 0);

}

else if (readValueDC == 1) {
  digitalWrite(DCMotorR2dir, LOW);
  analogWrite(DCMotorR2pwm, 255);
  digitalWrite(DCMotorR3dir, LOW);
  analogWrite(DCMotorR3pwm, 255);
}

Serial.print("ReadValueGoogle : ");
Serial.println(readValueGoogle);
}
