/*

*/

#include "OneWire.h"
#include "DallasTemperature.h"

//
#define TdsSensorPin 35

//
#define pHPin 39

//
#define WaterTempPin 34

//
#define Port 1883

//
#define WaterFLowFDBK   14
#define WaterflowSetPoint 13

//
#define NutrientTankLevelPin 32

//
#define WaterTankLevel 13

//
#define DHTPIN 36
#define DHTTYPE DHT11


//Topics:

struct{
const char *Tank_level_topic = "Hydroponic/Tank_level";
const char *pH = "Hydroponic/pH";
const char *Water_Flow = "Hydroponic/Water_Flow";
const char *p = "Hydroponic/p"; //
const char *Humidity = "Hydroponic/Humidity";
const char *Water_temperature = "Hydroponic/Water_temperature";
const char *Weather_temperature = "Hydroponic/Weather_temperature";
const char *Nutrient_tank_level = "Hydroponic/Nutrient_tank_level";
const char *Water_pump = "Hydroponic/Water_pump";
}Topics;

///////////////////////////////////////////////////////////////////////////

char * mqttpassword = "152436978";
const char *mqttclient = "ESP32_1";  // Name of our device, must be unique
const char *mqttclient1 = "ESP32_2";  // Name of our device, must be unique
const char *mqttclient2 = "Hydroponics";  // Name of our device, must be unique
