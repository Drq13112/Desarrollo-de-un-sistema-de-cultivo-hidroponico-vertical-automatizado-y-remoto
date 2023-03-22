#ifndef Motor_h
#define Motor_h
/*
BTS7960-43A-Driver
*/
class BTS7960 {

public:
  // Constructor
  BTS7960();
  // Metodos

  void SetUp();
  void Run(float);
  void Stop();
  int GetOutPut();

private:
  // Variables internas

  const int RPWM = 25;
  const int LPWM = 33;
  const int REN = 26;
  int out = 0;
  float Setpoint = 0;
};

/*
LN298N-Driver
*/

class LN298N {
private:
  float Setpoint = 0;
  const int PinIN1 = 5;
  const int PinIN2 = 18;
  const int ENB = 19;
  int out;

public:
  LN298N();
  void SetUp();
  void Run(float);
  void Stop();
  int GetOutPut();
};

#endif