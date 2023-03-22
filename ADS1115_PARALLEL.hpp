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


ADS1115_PARALLEL::ADS1115_PARALLEL(){}
ADS1115_PARALLEL::~ADS1115_PARALLEL(){}

void ADS1115_PARALLEL::SetUp()
{
    if (!ads1.begin(0x48))
    {
        Serial.println("Failed to initialize ADS 0x48.");
        while(1);
    }
    if (!ads2.begin(0x49))
    {
        Serial.println("Failed to initialize ADS 0x49.");
        while(1);
    }
    if (!ads3.begin(0x4A))
    {
        Serial.println("Failed to initialize ADS 0x4A.");
        while(1);
    }
    if (!ads4.begin(0x4B))
    {
        Serial.println("Failed to initialize ADS 0x4B.");
        while(1);
    }
}

float ADS1115_PARALLEL::ReadVoltage(int address = ADS1115_ADDRESS1, int channel = 0)
{
    int16_t adc;
    float volts=0;
    switch (address)
    {
    case ADS1115_ADDRESS1:
        adc = ads1.readADC_SingleEnded(channel);
        volts = ads1.computeVolts(adc);
        break;
    case ADS1115_ADDRESS2:
        adc = ads2.readADC_SingleEnded(channel);
        volts = ads2.computeVolts(adc);
        break;
    case ADS1115_ADDRESS3:
        adc = ads3.readADC_SingleEnded(channel);
        volts = ads3.computeVolts(adc);
        break;
    case ADS1115_ADDRESS4:
        adc = ads4.readADC_SingleEnded(channel);
        volts = ads4.computeVolts(adc);
        break;
    
    default:
        break;
    }
    return volts;
}
#endif