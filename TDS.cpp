#include "TDS.h"
#include "Arduino.h"

TDS::TDS(int TdsSensorPin,float temperature){

  this-> TdsSensorPin=TdsSensorPin;
  this-> temperature=temperature;
}
TDS::~TDS()
{}
float TDS::getRawValue()
{
  return analogRead(TdsSensorPin);
  
}
float TDS::getMedianNum(int bArray[], int iFilterLen){
  int bTab[iFilterLen];
  
  // Copy to bTab all bArray content
  for (byte i = 0; i<iFilterLen; i++){
    bTab[i] = bArray[i];}

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
  if ((iFilterLen & 1) > 0){
    bTemp = bTab[(iFilterLen - 1) / 2];
  }
  else {
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  }
  return float(bTemp);
}

void TDS::setPin(int TdsSensorPin){
  this-> TdsSensorPin=TdsSensorPin;
}

float TDS::getTDSValue(){
 static unsigned long printTimepoint = millis();
  if(millis()-printTimepoint > 800U){
    printTimepoint = millis();
    
      averageVoltage = analogRead(TdsSensorPin)* float(VREF) / 4096.0;
      
      
      Serial.print("float(VREF)");
      Serial.println((float)VREF);
      Serial.print("averageVoltage");
      Serial.println(averageVoltage);
      
      
      //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0)); 
      float compensationCoefficient = 1.0+0.02*(temperature-25.0);
      //temperature compensation
      float compensationVoltage=averageVoltage/compensationCoefficient;
      
      //convert voltage value to tds value
      tdsValue=(133.42*compensationVoltage*compensationVoltage*compensationVoltage - 255.86*compensationVoltage*compensationVoltage + 857.39*compensationVoltage)*0.5;

      //Serial.print("voltage:");
      //Serial.print(averageVoltage,2);
      //Serial.print("V   ");
    //}
  }
  return tdsValue;
}

void TDS::setTemperature(float temperature)
{

  this->temperature=temperature;
}
