// Paramereters
float MAX_pH = 6;
float MIN_pH = 5.5;
float MAX_EC = 2400;
float MIN_EC = 1000;
float Cycle_time = 10; // mins
float Water_temp_Setpoint = 20.0;
float Kp = 4.0;
float Ki = 0.2;
float Kd = 1;
float Time_pump_works = 5;      // mins
int Time_Nutrient = 10000;    // ms
int Time_pH_Decrease = 10000; // ms
int Time_pH_Up = 10000;       // ms
int Time_Water = 5000;        // ms
#define Height_tank 100;

// PINS
#define DHTPIN 15
#define DHTTYPE DHT11
#define WaterTempPin 23
#define WaterFLowFDBK 17      // Analog
#define Trigger_PIN 19
#define Echo_PIN 18
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
#define pH_Reductor_PIN 39
#define pH_Elevator_PIN 36
#define Nutrient1_Elevator_PIN 35
#define Nutrient2_Elevator_PIN 34

// I2C Communication
#define SDAPIN 21
#define SCLPIN 22

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
  const char *Nutrient_Uppered = "Hydroponic/Low_Nutrient_Tank_1";
  const char *Low_Nutrient_Tank_1 = "Hydroponic/Low_Nutrient_Tank_1";
  const char *Low_Nutrient_Tank_2 = "Hydroponic/Nutrient_Uppered";
  const char *Low_pH_Elevator_Tank = "Hydroponic/Low_pH_Elevator_Tank";
  const char *Low_pH_Reductor_Tank = "Hydroponic/Low_pH_Reductor_Tank";
  const char *Reset_Pushed = "Hydroponic/Reset";
} Topics;

///////////////////////////////////////////////////////////////////////////

// Variables
float temperature = 20;
float tdsValue = 450;
float Water_temperature = 20;
float pH = 5.5;
float humidity = 60;
float Water_flow = 0;
bool Nutrient1_tank_level = false;
bool Nutrient2_tank_level = false;
float Tank_level = 70;
bool pH_Elevator_Level = false;
bool pH_Reductor_Level = false;
float Heater = 0;
float ECValue = 0;

bool Update_Pump_state = false;
bool Update_data_flag = false;
bool Remote_Decrease_pH = false;
bool Remote_Increase_pH = false;
bool Remote_Water_UP = false;
bool Remote_Increase_TDS = false;
bool Low_Nutrient_Tank_1 = false;
bool Low_Nutrient_Tank_2 = false;
bool Low_pH_Elevator_Tank = false;
bool Low_pH_Reductor_Tank = false;
bool Reset = false;

int minute1 = 0;
int minute2 = 0;
int Pump_timer = 0;
int day = 0;
int day_initial = 0;
int Process_week = 0;
int Process_day = 0;
bool Process_ON = false;
bool Reset_Process = false;
bool SetProcess_Configuration = false;
bool Stop_Process = false;
size_t contador =0;

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 1 * 3600;
const int daylightOffset_sec = 0;

const char *mqttpassword = "152436978";
const int Port = 1883;
const char *IP = "192.168.0.6";
const char *mqttclient = "ESP32";        // Name of the device, must be unique
const char *ssid = "vodafoneAAFAVK";       // name of the WiFi network
const char *password = "KfPTnPFpTkgL6qPF"; // password of the WiFi network
bool state = 0;
