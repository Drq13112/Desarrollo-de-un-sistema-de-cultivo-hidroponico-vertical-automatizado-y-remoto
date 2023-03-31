#include "DHT.h"
#include <NewPing.h>
#include <Arduino.h>
#include <HardwareSerial.h>
#include "Separador.h"
#include "Motor.h"
#include "Relays.h"
#include <ESP32Time.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "PIDController.hpp"
#include "GravityTDS.h"
#include <Wire.h>
#include "OneWire.h"
#include "DallasTemperature.h"
#include "ADS1115_PARALLEL.h"
#include <string.h>
#include "AiEsp32RotaryEncoder.h"
#include "Menu.h"
#include <LiquidCrystal_I2C.h>
