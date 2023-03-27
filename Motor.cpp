#include "Motor.h"
#include <Arduino.h>

/*
BTS7960-43A-Driver
*/
BTS7960::BTS7960() {}

void BTS7960::SetUp()
{

  // put your setup code here, to run once:
  ledcSetup(Channel, freq, res); // setup PWM channel for BST R_PWM
  ledcAttachPin(RPWM, Channel);  // Attach BST R_PWM
  pinMode(REN, OUTPUT);          // declare pin as output
  delay(50);
  digitalWrite(REN, HIGH); // enable BST
}

void BTS7960::Run(float Setpoint)
{
  this->Setpoint = Setpoint;

  out = map(Setpoint, 0, 100, 0, 255);

  ledcWrite(Channel, out);
}
void BTS7960::Stop()
{
  this->Setpoint = 0;
  this->out = 0;
  ledcWrite(Channel, 0);
}
int BTS7960::GetOutPut()
{
  return out;
}