#ifndef TDS_h
#define TDS_h
#define VREF 3.3   // analog reference voltage(Volt) of the ADC
#define SCOUNT 30  // sum of sample point
#define measurements 10
#include "Arduino.h"

class TDS {

public:
  // Constructor and destructor
  TDS(int, int, float);
  ~TDS();

  void setTemperature(float);
  void setPin(int, int);
  float getMedianNum(int *, int);
  float getTDSValue();
  float getpH();

private:
  int analogBuffer[SCOUNT];  // store the analog value in the array, read from ADC
  int analogBufferTemp[SCOUNT];
  int analogBufferIndex = 0;
  int copyIndex = 0;
  int TdsSensorPin = 0;
  int pHPin = 0;
  float averageVoltage = 0;
  float tdsValue = 0;
  float temperature = 25;  // current temperature for compensation
  float calibration_value2 = 51;
  float calibration_value1 = -11;
  int phval = 0;
  float ph_act = 0;
  unsigned long int avgval;
  int buffer_arr[10], temp;

  float ReadVoltage(int, int);
};

TDS::TDS(int TdsSensorPin = 0, int pHPin=34, float temperature = 20.0) {
  this->TdsSensorPin = TdsSensorPin;
  this->pHPin = pHPin;
  this->temperature = temperature;
}
TDS::~TDS() {
}
float TDS::getMedianNum(int bArray[], int iFilterLen) {
  int bTab[iFilterLen];

  // Copy to bTab all bArray content
  for (byte i = 0; i < iFilterLen; i++) {
    bTab[i] = bArray[i];
  }

  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++) {
    for (i = 0; i < iFilterLen - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0) {
    bTemp = bTab[(iFilterLen - 1) / 2];
  } else {
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  }
  return float(bTemp);
}

void TDS::setPin(int TdsSensorPin, int pHPin) {
  this->TdsSensorPin = TdsSensorPin;
  this->pHPin = pHPin;
}

float TDS::getTDSValue() {
  static unsigned long printTimepoint = millis();
  if (millis() - printTimepoint > 800U) {
    printTimepoint = millis();

    averageVoltage = analogRead(TdsSensorPin) * float(VREF) / 4096.0;

    // temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);
    // temperature compensation
    float compensationVoltage = averageVoltage / compensationCoefficient;

    // convert voltage value to tds value
    tdsValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage - 255.86 * compensationVoltage * compensationVoltage + 857.39 * compensationVoltage) * 0.5;
  }
  return tdsValue;
}

void TDS::setTemperature(float temperature) {

  this->temperature = temperature;
}

float TDS::getpH() {

  for (int i = 0; i < measurements; i++) {
    buffer_arr[i] = analogRead(34) * float(VREF) / 4096.0;
    delay(50);
  }
  /*
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (buffer_arr[i] > buffer_arr[j]) {
        temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
  }*/
  avgval = 0;
  for (int i = 2; i < 8; i++)
    avgval += buffer_arr[i];

  float volt = (float)avgval * 3.3 / 4096.0 / 6;
  Serial.print("Voltage: ");
  Serial.println(volt);
  ph_act = -11 * volt + 51;
  return ph_act;
}

#endif