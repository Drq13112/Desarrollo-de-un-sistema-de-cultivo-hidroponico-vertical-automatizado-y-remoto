#include <PubSubClient.h>
#include <WiFi.h>

const char *ID = "Hydroponic_ESP32";  // Name of our device, must be unique

//Topics:
/*
const char *Tank_level = "Hydroponic/Tank_level"; 
const char *pH = "Hydroponic/pH";
const char *Water_Flow = "Hydroponic/Water_Flow";
const char *p= "Hydroponic/p"; //
const char *Humidity = "Hydroponic/Humidity";
const char *Water_temperature = "Hydroponic/Water_temperature";
const char *Weather_temperature = "Hydroponic/Weather_temperature";
const char *Nutrient_tank_level = "Hydroponic/Nutrient_tank_level";
const char *Water_pump = "Hydroponic/Water_pump";
*/
//////////////////////////////////////////////////////////////////
const char *ssid =  "vodafoneAAFAVK";   // name of your WiFi network
const char *password = "KfPTnPFpTkgL6qPF"; // password of the WiFi network

IPAddress broker(192,168,1,50); // IP address of your MQTT broker eg. 192.168.1.50
WiFiClient wclient;

PubSubClient client(wclient); // Setup MQTT client
bool state=0;


// Connect to WiFi network
void setup_wifi() {
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password); // Connect to network

  while (WiFi.status() != WL_CONNECTED) { // Wait for connection
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Reconnect to client
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(ID)) {
      Serial.println("connected");
      Serial.println('\n');

    } else {
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
