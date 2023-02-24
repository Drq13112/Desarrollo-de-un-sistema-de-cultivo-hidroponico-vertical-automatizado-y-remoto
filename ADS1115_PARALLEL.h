#ifndef __ADS1115_PARALLEL_H__
#define __ADS1115_PARALLEL_H__
#include <Adafruit_ADS1X15.h>

#define ADS1115_ADDRESS1 (0x48) ///< (ADDR = GND)
#define ADS1115_ADDRESS2 (0x49) ///< (ADDR = VCC)
#define ADS1115_ADDRESS3 (0x4A) ///< (ADDR = SCL)
#define ADS1115_ADDRESS4 (0x4B) ///< (ADDR = SDA)


Adafruit_ADS1115 ads1;
Adafruit_ADS1115 ads2;
Adafruit_ADS1115 ads3;
Adafruit_ADS1115 ads4;

class ADS1115_PARALLEL{

public:
// Constructor
ADS1115_PARALLEL();
~ADS1115_PARALLEL();

// Methods
void SetUp();

float ReadVoltage(int, int);

};
#endif