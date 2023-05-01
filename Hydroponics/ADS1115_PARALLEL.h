#ifndef __ADS1115_PARALLEL_H__
#define __ADS1115_PARALLEL_H__
#include <Adafruit_ADS1X15.h>

#define ADS1115_ADDRESS1 (0x48) ///< (ADDR = GND)
#define ADS1115_ADDRESS2 (0x49) ///< (ADDR = VCC)
#define ADS1115_ADDRESS3 (0x4A) ///< (ADDR = SCL)
#define ADS1115_ADDRESS4 (0x4B) ///< (ADDR = SDA)

class ADS1115_PARALLEL{

public:
// Constructor
ADS1115_PARALLEL(bool,bool,bool,bool);
~ADS1115_PARALLEL();

// Methods
void SetUp();
bool channel1 = 0;
bool channel2 = 0;
bool channel3 = 0;
bool channel4 = 0;
float ReadVoltage(int, int);

};
#endif