/*

*/

#include "OneWire.h"
#include "DallasTemperature.h"


//
#define DHTPIN 25
#define DHTTYPE DHT11
//
#define pHPin 33
//
#define WaterTempPin 32
//
#define TdsSensorPin 35
//






#define NutrientTankLevelPin 32
//
#define WaterFLowFDBK   14
#define WaterflowSetPoint 13
//
#define WaterTankLevel 13


//Topics:

struct{
const char *Tank_level = "Hydroponic/Tank_level";
const char *pH = "Hydroponic/pH";
const char *Water_Flow = "Hydroponic/Water_Flow";
const char *TDS = "Hydroponic/TDS"; //
const char *Humidity = "Hydroponic/Humidity";
const char *Water_temperature = "Hydroponic/Water_temperature";
const char *Weather_temperature = "Hydroponic/Weather_temperature";
const char *Nutrient_tank_level = "Hydroponic/Nutrient_tank_level";
const char *Water_pump = "Hydroponic/Water_pump";
const char *Water_pump_fdbk = "Hydroponic/Water_pump_fdbk";
}Topics;

///////////////////////////////////////////////////////////////////////////

char * mqttpassword = "152436978";
const char *mqttclient = "ESP32_1";  // Name of our device, must be unique
const char *mqttclient1 = "ESP32_2";  // Name of our device, must be unique
const char *mqttclient2 = "Hydroponics";  // Name of our device, must be unique
