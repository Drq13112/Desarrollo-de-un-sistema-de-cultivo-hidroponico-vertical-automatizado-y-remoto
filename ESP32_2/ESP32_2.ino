#include <Arduino.h>
#include "Certs.h"
#include "ADS1115_PARALLEL.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "WiFi.h"

//Functions

/*
void callback(char *topic, byte *payload, unsigned int length);
void Publish(float message, const char *topic);
void setup_wifi();
void reconnect();

*/
//Settings
const char* ssid = "HydroponicTFG2023";
const char* password = "HydroponicTFG2023";

//Definitions
#define PinRelay 23

//Variables
float Voltage = 0;
float Load_Current = 0;
float Panel_Current = 0;
float Load_Power = 0;
float Panel_Power = 0;

//Objects
ADS1115_PARALLEL I2C_Module(0, 0, 1, 0);
WiFiClientSecure espClient = WiFiClientSecure();
PubSubClient client(espClient);


void callback(String topic, byte* payload, unsigned int length) {
  /*
  String messageTemp;
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }*/
  Serial.print("incoming: ");
  Serial.println(topic);
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
  Serial.println(message);

  if (String(topic) == "Hydroponic/Power") {
    if (message == "1") {
      digitalWrite(PinRelay, HIGH);
    } else {
      digitalWrite(PinRelay, LOW);
    }
  }
}
void setup_wifi() {
  /*
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(HostName);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());*/

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  espClient.setCACert(AWS_CERT_CA);
  espClient.setCertificate(AWS_CERT_CRT);
  espClient.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);

  Serial.println("Connecting to AWS IOT");
}
void reconnect() {

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }

  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }
  Serial.println("AWS IoT Connected!");

  /*
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(HostName)) {
      Serial.println("connected");
      client.subscribe(topic_PHOTO);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }*/
}

void Publish(float message, const char* topic) {
  /*
    Serial.print("TOPIC: ");
    Serial.println(topic);
    Serial.print("message: ");
    Serial.println(message);
    */
  char message_to_upload[7];

  // We pass from a float number into a const char
  dtostrf(message, 6, 1, message_to_upload);
  client.publish(topic, message_to_upload);
}

void setup() {
  Serial.begin(9600);
  pinMode(PinRelay, OUTPUT);
  I2C_Module.SetUp();
  //setup_wifi();
  //reconnect();
  //client.setCallback(callback);
  //client.setServer(mqtt_server, 1883);
}
void loop() {
  Voltage = I2C_Module.ReadVoltage(ADS1115_ADDRESS3, 0);
  Load_Current = I2C_Module.ReadVoltage(ADS1115_ADDRESS3, 1);
  Panel_Current = I2C_Module.ReadVoltage(ADS1115_ADDRESS3, 2);
  Serial.print("Voltage: ");
  Serial.println(Voltage);
  Serial.print("Load current");
  Serial.println(Load_Current);
  Serial.print("Panel_Current:");
  Serial.println(Panel_Current);
  Voltage = 5.64 * (Voltage / 1000);
  Load_Current = 6.92 * (Load_Current) - 2.110);
  Panel_Current = -23 * (Panel_Current)
   - 2.160);
  Load_Power = Voltage * Load_Current;
  Panel_Power = Voltage * Panel_Power;
  /*
  Publish(Voltage,"Hydropionic/Voltage");
  Publish(Load_Power,"Hydropionic/Load_Power");
  Publish(Panel_Power,"Hydropionic/Panel_Power");*/
  delay(10000);
}
