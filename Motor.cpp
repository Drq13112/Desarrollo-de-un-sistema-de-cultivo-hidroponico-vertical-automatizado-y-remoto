#include "Motor.h"
#include <Arduino.h>

/*
BTS7960-43A-Driver
*/
BTS7960::BTS7960() {}

void BTS7960::SetUp() {
  
  delay(50);
  pinMode(RPWM, OUTPUT);
  delay(50);
  pinMode(LPWM, OUTPUT);
  delay(50);
  pinMode(REN, OUTPUT);
  delay(50);
  digitalWrite(REN, HIGH);
  delay(50);
}

void BTS7960::Run(float Setpoint) {
  this->Setpoint = Setpoint;

  out = map(Setpoint, 0, 100, 0, 255);
  analogWrite(RPWM, out);
  analogWrite(LPWM, 0);
}
void BTS7960::Stop() {
  this->Setpoint = 0;
  analogWrite(RPWM, 0);
  analogWrite(LPWM, 0);
}
int BTS7960::GetOutPut() {
  return out;
}

/*
LN298N-Driver
*/
LN298N::LN298N() {}

void LN298N::SetUp() {
  pinMode(PinIN1, OUTPUT);
  delay(50);
  pinMode(PinIN2, OUTPUT);
  delay(50);
  pinMode(ENB, OUTPUT);
  delay(50);
}

void LN298N::Run(float Setpoint) {
  this->Setpoint = Setpoint;
  digitalWrite(PinIN1, HIGH);
  digitalWrite(PinIN2, LOW);
  out = map(Setpoint, 0, 100, 0, 255);
  analogWrite(ENB, out);
}
void LN298N::Stop() {
  digitalWrite(PinIN1, LOW);
  digitalWrite(PinIN2, LOW);
}

int LN298N::GetOutPut() {
  return out;
}