#ifndef Motor_h
#define Motor_h
/*
BTS7960-43A-Driver
*/
class BTS7960
{

public:
  // Constructor
  BTS7960();
  // Methods

  void SetUp(float);
  void Run(float);
  void Stop();
  int GetOut();

private:
  // Private variables
  const int Channel = 0;
  const int freq = 1000;    //  Set up PWM Frequency
  const int res = 8;        //  Set up PWM Resolution
  const int RPWM = 33;
  int out = 255;
  float Setpoint = 0;
};

#endif