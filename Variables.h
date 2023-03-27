<<<<<<< HEAD
#include <string.h>

// Paramereters
int MAX_pH = 10;
int MIN_pH = 0;
int MAX_TDS = 1000;
int MIN_TDS = 300;
int MAX_TEMP = 25;
int MIN_TEMP = 15;
int Cycle_time = 5; // mins
float Water_temp_Setpoint = 20.0;
#define MAX_DISTANCE 200       // Maximum distance (in cm) to ping.
int Time_pump_works = 5;     // mins
#define Time_Nutrient 10000    // ms
#define Time_pH_Decrease 10000 // ms
#define Time_pH_Up 10000       // ms
#define Time_Water 5000        // ms

// PINS
#define DHTPIN 32
#define DHTTYPE DHT11
#define WaterFLowFDBK 17      // Analog
#define WaterflowSetPoint1 16 // Digital PWM
#define WaterflowSetPoint2 15 // Digital PWM
#define WaterTempPin 23
#define TdsPin 33
#define pHPin 35

// UART Communication
#define RXp2 16
#define TXp2 17
//

String message_from_arduino;
// Topics:

struct
{
  const char *Tank_level = "Hydroponic/Tank_level";
  const char *pH = "Hydroponic/pH";
  const char *Water_Flow = "Hydroponic/Water_Flow";
  const char *TDS = "Hydroponic/TDS";
  const char *Humidity = "Hydroponic/Humidity";
  const char *Water_temperature = "Hydroponic/Water_temperature";
  const char *Weather_temperature = "Hydroponic/Weather_temperature";
  const char *Nutrient_tank_level = "Hydroponic/Nutrient_tank_level";
  const char *pH_elevator_tank_level = "Hydroponic/pH_elevator_tank_level";
  const char *pH_reductor_tank_level = "Hydroponic/pH_reductor_tank_level";
  const char *Water_pump = "Hydroponic/Water_pump";
  const char *Heater = "Hydroponic/Heater";
  const char *Water_pump_fdbk = "Hydroponic/Water_pump_fdbk";
  const char *Photo_petition = "Hydroponic/Photo_petition";
  const char *Update_petition = "Hydroponic/Update_petition";
  const char *Nutrient_Up = "Hydroponic/Nutrient_Up";
  const char *pH_Up = "Hydroponic/pH_Up";
  const char *pH_Decrease = "Hydroponic/pH_Decrease";
  const char *Water_Up = "Hydroponic/Water_Up";
  const char *Water_Uppered = "Hydroponic/Water_Uppered";
  const char *pH_Decreased = "Hydroponic/pH_Decreased";
  const char *pH_Uppered = "Hydroponic/pH_Uppered";
  const char *Nutrient_Uppered = "Hydroponic/Nutrient_Uppered";
} Topics;

///////////////////////////////////////////////////////////////////////////

// Varaibles
float temperature = 20;
float tdsValue = 450;
float Water_temperature = 20;
float pH = 5.5;
float humidity = 60;
float Water_flow = 0;
float Nutrient_tank_level = 60;
float Tank_level = 70;
float pH_Elevator_Level = 0;
float pH_Reductor_Level = 0;
float Water_Reserve_Level = 0;
float Heater = 0;
bool Update_Pump_state = false;
bool Update_data_flag = false;
bool Remote_Decrease_pH = false;
bool Remote_Increase_pH = false;
bool Remote_Water_UP = false;
bool Remote_Increase_TDS = false;

int minute1 = 0;
int minute2 = 0;
int Pump_timer = 0;
int day = 0;
int day_initial = 0;

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 1 * 3600;
const int daylightOffset_sec = 0;

const char *mqttpassword = "152436978";
const int Port = 1883;
const char *IP = "192.168.0.6";
const char *mqttclient = "ESP32_2";        // Name of the device, must be unique
const char *ssid = "vodafoneAAFAVK";       // name of the WiFi network
const char *password = "KfPTnPFpTkgL6qPF"; // password of the WiFi network
bool state = 0;
=======
// Paramereters
int MAX_pH = 10;
int MIN_pH = 0;
int MAX_TDS = 1000;
int MIN_TDS = 300;
int MAX_TEMP = 25;
int MIN_TEMP = 15;
int Cycle_time = 5; // mins
float Water_temp_Setpoint = 20.0;
#define MAX_DISTANCE 200       // Maximum distance (in cm) to ping.
int Time_pump_works = 5;     // mins
#define Time_Nutrient 10000    // ms
#define Time_pH_Decrease 10000 // ms
#define Time_pH_Up 10000       // ms
#define Time_Water 5000        // ms

// PINS
#define DHTPIN 32
#define DHTTYPE DHT11
#define WaterTempPin 18
#define WaterFLowFDBK 17      // Analog
#define WaterflowSetPoint1 16 // Digital PWM
#define WaterflowSetPoint2 15 // Digital PWM

// UART Communication
#define RXp2 16
#define TXp2 17
//

String message_from_arduino;
// Topics:

struct
{
  const char *Tank_level = "Hydroponic/Tank_level";
  const char *pH = "Hydroponic/pH";
  const char *Water_Flow = "Hydroponic/Water_Flow";
  const char *TDS = "Hydroponic/TDS";
  const char *Humidity = "Hydroponic/Humidity";
  const char *Water_temperature = "Hydroponic/Water_temperature";
  const char *Weather_temperature = "Hydroponic/Weather_temperature";
  const char *Nutrient_tank_level = "Hydroponic/Nutrient_tank_level";
  const char *pH_elevator_tank_level = "Hydroponic/pH_elevator_tank_level";
  const char *pH_reductor_tank_level = "Hydroponic/pH_reductor_tank_level";
  const char *Water_pump = "Hydroponic/Water_pump";
  const char *Heater = "Hydroponic/Heater";
  const char *Water_pump_fdbk = "Hydroponic/Water_pump_fdbk";
  const char *Photo_petition = "Hydroponic/Photo_petition";
  const char *Update_petition = "Hydroponic/Update_petition";
  const char *Nutrient_Up = "Hydroponic/Nutrient_Up";
  const char *pH_Up = "Hydroponic/pH_Up";
  const char *pH_Decrease = "Hydroponic/pH_Decrease";
  const char *Water_Up = "Hydroponic/Water_Up";
  const char *Water_Uppered = "Hydroponic/Water_Uppered";
  const char *pH_Decreased = "Hydroponic/pH_Decreased";
  const char *pH_Uppered = "Hydroponic/pH_Uppered";
  const char *Nutrient_Uppered = "Hydroponic/Nutrient_Uppered";
} Topics;

///////////////////////////////////////////////////////////////////////////

// Varaibles
float temperature = 20;
float tdsValue = 450;
float Water_temperature = 20;
float pH = 5.5;
float humidity = 60;
float Water_flow = 0;
float Nutrient_tank_level = 60;
float Tank_level = 70;
float pH_Elevator_Level = 0;
float pH_Reductor_Level = 0;
float Water_Reserve_Level = 0;
float Heater = 0;
bool Update_Pump_state = false;
bool Update_data_flag = false;
bool Remote_Decrease_pH = false;
bool Remote_Increase_pH = false;
bool Remote_Water_UP = false;
bool Remote_Increase_TDS = false;

int minute1 = 0;
int minute2 = 0;
int Pump_timer = 0;
int day = 0;
int day_initial = 0;

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 1 * 3600;
const int daylightOffset_sec = 0;

const char *mqttpassword = "152436978";
const int Port = 1883;
const char *IP = "192.168.0.6";
const char *mqttclient = "ESP32_2";        // Name of the device, must be unique
const char *ssid = "vodafoneAAFAVK";       // name of the WiFi network
const char *password = "KfPTnPFpTkgL6qPF"; // password of the WiFi network
bool state = 0;
>>>>>>> 91e8484924f69b155aa0eee53a62d32cbda31d41
