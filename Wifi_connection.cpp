#include "Wifi_connection.h"
WiFiClient wclient;
PubSubClient client(wclient);

String Response_String = "";
/*

   The callback function, to execute when a message is received for a subscribed topic.
   The arguments of this callback function are the name of the topic, the payload (in bytes) and the length of the message received.
   The message should also return void.
   As can be seen in the code bellow, we will first print the topic name and then each byte of the message received.
   In the end we will also print some separator characters, to differentiate messages received.

*/
void callback(char* topic, byte* payload, unsigned int length) {

  String response = "";

  Serial.print("Message arrived [");
  Serial.print(topic);

  for (int i = 0; i < length; i++) {
    response = ((char)payload[i]);
  }

  Serial.println();
  Serial.println("-----------------------");

  Response_String = response;

}

MQTT::MQTT(){}
MQTT::~MQTT() {}
void MQTT::SetIP(char* IP)
{
  this-> IP = IP;
}
void MQTT::SetPort(uint8_t Port)
{
  this->Port = Port;
}
int MQTT::GetResponse()
{
  return Response_String.toInt();
}

// Connect to WiFi network
void MQTT::setup_wifi() {

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

void MQTT::reconnect() {

  client.setServer(IP, 1883);
  client.setCallback(callback);

  // Loop until we're reconnected
  while (!client.connected()) {

    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32_2")) {
      Serial.println("connected");
      Serial.println('\n');

    } else {
      Serial.println(" try again in 5 seconds");
      Serial.println(client.state());
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void MQTT::Subscribe(const char* topic)
{
  client.subscribe(topic);
}
void MQTT::Publish(float message, const char* topic)
{

  char message_to_upload [7];

  // We pass from a float nimbre into a const char
  dtostrf(message, 6, 2, message_to_upload);
  client.publish(topic, message_to_upload);
}

void MQTT::Loop()
{
  client.loop();
}
