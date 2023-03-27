#include "libraries.h"

Adafruit_ADS1115 ads1;
Adafruit_ADS1115 ads2;
Adafruit_ADS1115 ads3;
Adafruit_ADS1115 ads4;

ADS1115_PARALLEL::ADS1115_PARALLEL(bool channel1 = 1, bool channel2 = 0, bool channel3 = 0, bool channel4 = 0) {
  this->channel1 = channel1;
  this->channel2 = channel2;
  this->channel3 = channel3;
  this->channel4 = channel4;
}
ADS1115_PARALLEL::~ADS1115_PARALLEL() {}

void ADS1115_PARALLEL::SetUp() {
  if (channel1 == 1 && !ads1.begin(0x48)) {
    Serial.println(channel1);
    Serial.println("Failed to initialize ADS 0x48.");
    while (1)
      ;
  }
  if (channel2 == 1 && !ads2.begin(0x49)) {
    Serial.println("Failed to initialize ADS 0x49.");
    while (1)
      ;
  }
  if (channel3 == 1 && !ads3.begin(0x4A)) {
    Serial.println("Failed to initialize ADS 0x4A.");
    while (1)
      ;
  }
  if (channel4 == 1 && !ads4.begin(0x4B)) {
    Serial.println(channel4);
    Serial.println("Failed to initialize ADS 0x4B.");
    while (1)
      ;
  }
}

float ADS1115_PARALLEL::ReadVoltage(int address = ADS1115_ADDRESS1, int channel = 0) {
  int16_t adc;
  float volts = 0;
  switch (address) {
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