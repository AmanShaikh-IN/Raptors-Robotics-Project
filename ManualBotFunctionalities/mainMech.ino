#include <MobaTools.h>
#include <Servo.h>

const byte IntakeStepperDir = 22;
const byte IntakeStepperPwm = 12;

const int relaypin1 = 28;

const byte BeltDCDir = 26;
const byte BeltDCPwm = 10;

const byte IntakeDCDir = 24;
const byte IntakeDCPwm = 9;

const byte ArmStepVert = 3;
const byte ArmDirVert = 4;
const int stepsPerRevArmVert = 800;

const byte ArmStepHorz = 6;
const byte ArmDirHorz = 5;
const int stepsPerRevArmHorz = 800;

const byte receiver_G = A12;     //Ball Intake
const byte receiver_H = A11;     //Channel 5
const byte receiver_I = A13;     //Channel 8

const byte receiver_E = A8;      //horizontal_10
const byte receiver_F = A9;      //vertical_9
const byte receiver_D = A10;      //servo_7s

Servo gripper_1;
int gripper1_pin = 8;


MoToStepper ArmStepperVert(stepsPerRevArmVert, STEPDIR);
MoToStepper ArmStepperHorz(stepsPerRevArmHorz, STEPDIR);

int readValueHorz, readValueVert, readValueGripper, RelayValue, IntakeValue, IntakeMotorValue;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  ArmStepperVert.attach(ArmStepVert, ArmDirVert);
  ArmStepperHorz.attach(ArmStepHorz, ArmDirHorz);

  pinMode(receiver_D, INPUT);
  pinMode(receiver_E, INPUT);
  pinMode(receiver_F, INPUT);
  pinMode(receiver_G, INPUT);
  pinMode(receiver_H, INPUT);
  pinMode(receiver_I, INPUT);

  pinMode(IntakeStepperPwm, OUTPUT);
  pinMode(IntakeStepperDir, OUTPUT);

  pinMode(IntakeDCDir, OUTPUT);
  pinMode(IntakeDCPwm, OUTPUT);

  pinMode(relaypin1, OUTPUT);
  pinMode(BeltDCDir, OUTPUT);
  pinMode(BeltDCPwm, OUTPUT);

  ArmStepperVert.setZero();
  ArmStepperHorz.setZero();

  gripper_1.attach(gripper1_pin);

}

void loop() {

  readValueHorz = pulseIn(receiver_E, HIGH);
  Serial.print("ReadValueHorz PulseIn: ");
  Serial.println(readValueHorz);

  readValueVert = pulseIn(receiver_F, HIGH);
  Serial.print("ReadValueVert PulseIn: ");
  Serial.println(readValueVert);

  readValueGripper = pulseIn(receiver_D, HIGH);
  Serial.print("ReadValueGripper PulseIn: ");
  Serial.println(readValueGripper);

  RelayValue = pulseIn(receiver_I, HIGH);
  Serial.print("Relay Pin Value: ");
  Serial.println(RelayValue);

  IntakeMotorValue = pulseIn(receiver_G, HIGH);
  Serial.print("Intake DC Motor Value: ");
  Serial.println(IntakeMotorValue);

  IntakeValue = pulseIn(receiver_H, HIGH);
  Serial.print("Intake Value: ");
  Serial.println(IntakeValue);

  if (readValueHorz == 0) {
    readValueHorz = 0;
  }
  if ((readValueHorz > 1200) && (readValueHorz < 1600)) {
    readValueHorz = 0;
  } else if (readValueHorz > 1600) {
    readValueHorz = 1;
  } else if (readValueHorz < 1200) {
    readValueHorz = -1;
  }

  Serial.print("ReadValueHorz Value: ");
  Serial.println(readValueHorz);

  if (readValueVert == 0) {
    readValueVert = 0;
  }
  if ((readValueVert > 1200) && (readValueVert < 1600)) {
    readValueVert = 0;
  } else if (readValueVert > 1600) {
    readValueVert = 1;
  } else if (readValueVert < 1200) {
    readValueVert = -1;
  }

  Serial.print("ReadValueVert Value: ");
  Serial.println(readValueVert);

  if (readValueGripper == 0)
    readValueGripper = 0;

  else if (readValueGripper > 1200 && readValueGripper < 1600)
    readValueGripper = 0;

  else if (readValueGripper > 1600)
    readValueGripper = 1;

  else
    readValueGripper = -1;

  Serial.print("ReadValueGripper Value: ");
  Serial.println(readValueGripper);

  if (readValueHorz == 1) {
    ArmStepperHorz.setSpeed(300);
    ArmStepperHorz.rotate(-1);
  }

  else if (readValueHorz == 0) {
    ArmStepperHorz.stop();
  }

  else if (readValueHorz == -1) {
    ArmStepperHorz.setSpeed(300);
    ArmStepperHorz.rotate(1);
  }

  if (readValueVert == 1) {
    ArmStepperVert.setSpeed(1000);
    ArmStepperVert.rotate(1);
  }

  else if (readValueVert == 0) {
    ArmStepperVert.stop();
  }

  else if (readValueVert == -1) {
    ArmStepperVert.setSpeed(1000);
    ArmStepperVert.rotate(-1);
  }

  if (readValueGripper == -1) {

    gripper_1.write(0);

  }

  else if (readValueGripper == 0) {

    gripper_1.write(90);

  }

  else if (readValueGripper == 1) {

    gripper_1.write(180);

  }

  if (RelayValue > 1400 || RelayValue == 0) {

    digitalWrite(relaypin1, HIGH);
    digitalWrite(BeltDCDir, LOW);
    analogWrite(BeltDCPwm, 0);
  }

  else if (RelayValue < 1400) {
    
    digitalWrite(relaypin1, LOW);
    digitalWrite(BeltDCDir, HIGH);
    analogWrite(BeltDCPwm, 255);


  }

  if (IntakeMotorValue == 0 || IntakeMotorValue > 1400) {
    digitalWrite(IntakeDCDir, LOW);
    analogWrite(IntakeDCPwm, 0);
  }

  else if (IntakeMotorValue < 1400) {
    digitalWrite(IntakeDCDir, LOW);
    analogWrite(IntakeDCPwm, 255);
  }

  if ((IntakeValue > 1200 && IntakeValue < 1600) || IntakeValue == 0) {

    Serial.println("Nothing");
  }

  else if (IntakeValue < 1200) {
    // BallIntakeStepper.write(1800);
    // digitalWrite(IntakeDCDir, HIGH);
    // analogWrite(IntakeDCPwm, 200);
    digitalWrite(IntakeStepperDir, LOW);
    for (int x = 0; x < 1200; x++) {
      digitalWrite(IntakeStepperPwm, HIGH);
      delayMicroseconds(500);
      digitalWrite(IntakeStepperPwm, LOW);
      delayMicroseconds(250);
    }
    Serial.println("one way");
  }

  else if (IntakeValue > 1600) {
    // BallIntakeStepper.write(0);
    // analogWrite(IntakeDCPwm, 0);

    digitalWrite(IntakeStepperDir, HIGH);
    for (int x = 0; x < 1200; x++) {
      digitalWrite(IntakeStepperPwm, HIGH);
      delayMicroseconds(500);
      digitalWrite(IntakeStepperPwm, LOW);
      delayMicroseconds(250);
    }
    Serial.println("another way");
  }

}
