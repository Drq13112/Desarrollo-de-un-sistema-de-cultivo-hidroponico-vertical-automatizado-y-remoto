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

  void SetUp();
  void Run(float);
  void Stop();
  int GetOutPut();

private:
  // Private variables
  const int Channel = 0;
  const int freq = 1000; //  Set up PWM Frequency
  const int res = 8;     //  Set up PWM Resolution
  const int RPWM = 33;
  const int REN = 32;
  int out = 0;
  float Setpoint = 0;
};

#endif