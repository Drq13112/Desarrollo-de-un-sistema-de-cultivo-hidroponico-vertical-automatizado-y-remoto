#include <Arduino.h>
#include "Certs.h"
#include "ADS1115_PARALLEL.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "WiFi.h"

#define Check_Communication 10000000

// Functions

/*
void callback(char *topic, byte *payload, unsigned int length);
void Publish(float message, const char *topic);
void setup_wifi();
void reconnect();

*/
// Settings
const char *ssid = "HydroponicTFG2023";
const char *password = "HydroponicTFG2023";

// Definitions
#define PinRelay 23

// Variables
float Voltage = 0;
float Load_Current = 0;
float Panel_Current = 0;
float Load_Power = 0;
float Panel_Power = 0;
size_t contador_communication = 0;
size_t Relay_State = 0;  // Close
bool Connected = false;
size_t Attempt =0;

// Objects
ADS1115_PARALLEL I2C_Module(0, 0, 1, 0);
WiFiClientSecure espClient = WiFiClientSecure();
PubSubClient client(espClient);

void callback(String topic, byte *payload, unsigned int length) {
  String response = "";
  for (int i = 0; i < length; i++) {
    response += (char)payload[i];
  }
  // Serial.print("incoming: ");
  // Serial.println(topic);
  // StaticJsonDocument<200> doc;
  // deserializeJson(doc, payload);
  // const char *response = doc["message"];
  // Serial.println(message);
  Serial.print(topic);
  Serial.print(" message: ");
  Serial.println(response);

  if (String(topic) == "Hydroponic/Power_Remote") {
    if (response == "1") {
      digitalWrite(PinRelay, HIGH);  // Close
      Relay_State = 1;
    } else {
      digitalWrite(PinRelay, LOW);  //Open
      Relay_State = 0;
    }
  }
  if (String(topic) == "Hydroponic/Reset_Remote") {
    Reset_ESP();
  }
  if (String(topic) == "Hydroponic/Update_petition_power") {
    Update_Data();
  }
  if (String(topic) == "Hydroponic/Communication_Check_Remote_2") {
    Connected == true;
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
  Connected = true;
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

void Publish(float message, const char *topic) {
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
void Reset_ESP() {
  ESP.restart();
}

void Update_Data() {
  Voltage = I2C_Module.ReadVoltage(ADS1115_ADDRESS3, 0);
  Load_Current = I2C_Module.ReadVoltage(ADS1115_ADDRESS3, 1);
  Panel_Current = I2C_Module.ReadVoltage(ADS1115_ADDRESS3, 2);
  Serial.print("Load current");
  Serial.println(Load_Current);
  Serial.print("Panel_Current:");
  Serial.println(Panel_Current);
  Voltage = 9.52 * (Voltage);
  Load_Current = -(Load_Current * 14.925) + 37.163;
  if (Load_Current < 0) {
    Load_Current = 0;
  }
  Panel_Current = (Panel_Current * 14.925) - 37.163;
  if (Panel_Current < 0) {
    Panel_Current = 0;
  }
  Load_Power = Voltage * Load_Current;
  Panel_Power = Voltage * Panel_Current;
  Serial.print("Voltage: ");
  Serial.println(Voltage);
  Serial.print("Load current");
  Serial.println(Load_Current);
  Serial.print("Panel_Current:");
  Serial.println(Panel_Current);
  Publish(Voltage, "Hydroponic/Voltage");
  Publish(Load_Power, "Hydroponic/Load_Power");
  Publish(Panel_Power, "Hydroponic/Panel_Power");
}

void setup() {
  Serial.begin(115200);
  pinMode(PinRelay, OUTPUT);
  I2C_Module.SetUp();
  setup_wifi();
  reconnect();
  client.setCallback(callback);
  client.subscribe("Hydroponic/Update_petition_power");
  client.subscribe("Hydroponic/Reset_Remote");
  client.subscribe("Hydroponic/Power_Remote");
  // client.setServer(mqtt_server, 1883);
  Publish(Relay_State, "Hydroponic/Power");
}
void loop() {

  if (contador_communication >= Check_Communication) {
    Serial.println("Checking communication");
    contador_communication = 0;
    Publish(1, "Hydroponic/Communication_Check_2");
    Attempt++;
    if (Connected == false && Attempt >= 2) {
      Serial.println("Connection_Lost");
      Reset_ESP();
    }
  }
  //Serial.print("contador_communication :");
  //Serial.println(contador_communication);
  contador_communication++;
  client.loop();
}
