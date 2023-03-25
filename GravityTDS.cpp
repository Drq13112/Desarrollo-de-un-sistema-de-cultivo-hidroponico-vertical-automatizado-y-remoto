/***************************************************
 DFRobot Gravity: Analog TDS Sensor/Meter
 <https://www.dfrobot.com/wiki/index.php/Gravity:_Analog_TDS_Sensor_/_Meter_For_Arduino_SKU:_SEN0244>
 
 ***************************************************
 This sample code shows how to read the tds value and calibrate it with the standard buffer solution.
 707ppm(1413us/cm)@25^c standard buffer solution is recommended.
 
 Created 2018-1-3
 By Jason <jason.ling@dfrobot.com@dfrobot.com>
 
 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution.
 ****************************************************/

#include "Libraries.h"

#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}


ADS1115_PARALLEL ADS;
GravityTDS::GravityTDS()
{
    this->pin = 30;
    this->temperature = 25.0;
    this->aref = 5.0;
    this->adcRange = 1024.0;
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
	pinMode(this->pin,INPUT);
	readKValues();
  ADS.SetUp();
}

float GravityTDS::getKvalue()
{
	return this->kValue;
}

void GravityTDS::update()
{

  // TDS and EC
  this->analogValueTDS=getAverage(1);
	//this->analogValue = analogRead(this->pin);
	//this->voltage = this->analogValue/this->adcRange*this->aref;
	this->ecValue=(133.42*this->analogValueTDS*this->analogValueTDS*this->analogValueTDS - 255.86*this->analogValueTDS*this->analogValueTDS + 857.39*this->analogValueTDS)*this->kValue;
	this->ecValue25  =  this->ecValue / (1.0+0.02*(this->temperature-25.0));  //temperature compensation
	this->tdsValue = ecValue25 * TdsFactor;
  // pH

  this->analogValuepH=getAverage(0);
  pHValue= Coef1*analogValuepH +coef2;
}
float GravityTDS::getpH()
{

  return pHValue;
}
float GravityTDS::getTdsValue()
{
	return tdsValue;
}

float GravityTDS::getEcValue()
{
      return ecValue25;
}


void GravityTDS::readKValues()
{
    EEPROM_read(this->kValueAddress, this->kValue);  
    if(EEPROM.read(this->kValueAddress)==0xFF && EEPROM.read(this->kValueAddress+1)==0xFF && EEPROM.read(this->kValueAddress+2)==0xFF && EEPROM.read(this->kValueAddress+3)==0xFF)
    {
      this->kValue = 1.0;   // default value: K = 1.0
      EEPROM_write(this->kValueAddress, this->kValue);
    }
}
float GravityTDS::getAverage(int channel){

  float averageVoltage=0.0;

  for(size_t i=0; i<10; i++){
    averageVoltage= ADS.ReadVoltage(ADS1115_ADDRESS1, channel)+averageVoltage;
  }

  averageVoltage=averageVoltage/10.0;
  return averageVoltage;
}
