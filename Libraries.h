<<<<<<< HEAD
#include "DHT.h"
#include <NewPing.h>
#include <HardwareSerial.h>
#include "EasyCommaLib.h"
#include "Separador.h"
#include "Motor.h"
#include "Relays.h"
#include <ESP32Time.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "PIDController.hpp"
#include "OneWire.h"
#include "DallasTemperature.h"
#include "TDS.hpp"
#include "GravityTDS.h"
=======
#include "DHT.h"
//#include <NewPing.h>
#include "Arduino.h"
#include <HardwareSerial.h>
#include "Separador.h"
#include "Motor.h"
#include "Relays.h"
#include <ESP32Time.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "PIDController.h"
#include "GravityTDS.h"
#include <Wire.h> 
#include "analogWrite.h"
#include "ADS1115_PARALLEL.h"
#include <EEPROM.h>
#include <string.h>
//#include "AiEsp32RotaryEncoder.h"
>>>>>>> 91e8484924f69b155aa0eee53a62d32cbda31d41
