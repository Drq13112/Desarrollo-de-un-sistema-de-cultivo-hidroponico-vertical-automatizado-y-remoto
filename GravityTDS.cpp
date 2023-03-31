#include "Libraries.h"

ADS1115_PARALLEL ADS(0,0,0,0);
GravityTDS::GravityTDS()
{
  this->pin = 30;               //Not used
  this->temperature = 25.0;
  this->aref = 5.0;             //Not used
  this->adcRange = 1024.0;      //Not used
  this->kValueAddress = 8;
  this->kValue = 1.0;
}

GravityTDS::~GravityTDS()
{
}

void GravityTDS::setPin(int pin)
{
  this->pin = pin;
}

void GravityTDS::setTemperature(float temp)
{
  this->temperature = temp;
}

void GravityTDS::setAref(float value)
{
  this->aref = value;
}

void GravityTDS::setAdcRange(float range)
{
  this->adcRange = range;
}

void GravityTDS::setKvalueAddress(int address)
{
  this->kValueAddress = address;
}

void GravityTDS::begin()
{
  pinMode(this->pin, INPUT);
  delay(50);
  ADS.SetUp();
}

float GravityTDS::getKvalue()
{
  return this->kValue;
}

void GravityTDS::update()
{

  // TDS and EC
  this->analogValueTDS = getAverage(1);
  Serial.println("analogValueTDS:");
  Serial.println(analogValueTDS);
  this->ecValue = (133.42 * this->analogValueTDS * this->analogValueTDS * this->analogValueTDS - 255.86 * this->analogValueTDS * this->analogValueTDS + 857.39 * this->analogValueTDS) * this->kValue;
  this->ecValue25 = this->ecValue / (1.0 + 0.02 * (this->temperature - 25.0)); // temperature compensation
  this->tdsValue = ecValue25 * TdsFactor;
  delay(50);
  // pH
  this->analogValuepH = getAverage(0);
  Serial.print("analogValuepH");
  Serial.println(analogValuepH);
  pHValue = Coef1 * analogValuepH + coef2;
}
float GravityTDS::getpH()
{
  return this->pHValue;
}
float GravityTDS::getTdsValue()
{
  return this->tdsValue;
}

float GravityTDS::getEcValue()
{
  return ecValue25;
}

float GravityTDS::getAverage(int channel)
{

  float averageVoltage = 0.0;

  for (size_t i = 0; i < 10; i++)
  {
    averageVoltage = ADS.ReadVoltage(ADS1115_ADDRESS1, channel) + averageVoltage;
  }

  averageVoltage = averageVoltage / 10.0;
  return averageVoltage;
}
float GravityTDS::getPowerValue()
{

  Voltage = ADS.ReadVoltage(ADS1115_ADDRESS1, 2);
  Current = ADS.ReadVoltage(ADS1115_ADDRESS1, 3);
  return Voltage * Current;
}