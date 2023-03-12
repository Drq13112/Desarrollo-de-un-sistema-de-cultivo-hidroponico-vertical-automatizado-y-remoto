#include <string.h>

#define SONAR_NUM 5      // Number of level sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.

#define WaterTankLevelTrigger 32
#define WaterTankLevelEcho 39

#define WaterTankLevelReserveTrigger 27
#define WaterTankLevelReserveEcho 36

#define NutrientTankLevelTrigger 26
#define NutrientTankLevelEcho 35

#define pHElevatorTankLevelTrigger 25
#define pHElevatorTankLevelEcho 34

#define pHReductorTankLevelTrigger 23
#define pHReductorTankLevelEcho 33

//
#define DHTPIN 19
#define DHTTYPE DHT11
//
#define WaterTempPin 18
//

//
#define WaterFLowFDBK   17    // Analog
#define WaterflowSetPoint1 16  // Digital PWM
#define WaterflowSetPoint2 15  // Digital PWM
//

// UART Communication

#define RXp2 16
#define TXp2 17
//

String message_from_arduino;
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
/*
const char *mqttpassword = "152436978";
const char *mqttclient = "ESP32_1";  // Name of our device, must be unique
const char *mqttclient1 = "ESP32_2";  // Name of our device, must be unique
const char *mqttclient2 = "Hydroponics";  // Name of our device, must be unique
*/
