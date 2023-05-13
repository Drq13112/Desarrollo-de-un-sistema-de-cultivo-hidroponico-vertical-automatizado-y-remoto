#include "Libraries.h"

/*
BTS7960-43A-Driver
*/
BTS7960::BTS7960() {}

void BTS7960::SetUp(float Setpoint)
{

  this->Setpoint = Setpoint;
  // put your setup code here, to run once:
  ledcSetup(Channel, freq, res); // setup PWM channel for BST R_PWM
  ledcAttachPin(RPWM, Channel);  // Attach BST R_PWM
  delay(50);
}

void BTS7960::Run(float Setpoint)
{
  this->Setpoint = Setpoint;

  out = map(Setpoint, 0, 255, 155, 0); // The H bridge is inverted

  ledcWrite(Channel, out);
}
void BTS7960::Stop()
{
  Setpoint = 0;
  out = 255;
  ledcWrite(Channel, 255);
}
int BTS7960::GetOut()
{
  return out;
}