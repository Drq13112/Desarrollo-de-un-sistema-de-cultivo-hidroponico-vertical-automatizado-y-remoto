#ifndef MQTT_h
#define MQTT_h

#include <PubSubClient.h>
#include <WiFi.h>
#include "string.h"

class MQTT {

  public:

    //Methods:

    MQTT();
    ~MQTT();
    void setup_wifi();
    void reconnect();
    int GetResponse();
    void Subscribe(const char *);
    void Publish(float, const char *);
    void Loop();

  private:
    /////////////////////////////////////////////////////////////////////////////
    const char * mqttpassword = "152436978";
    const int Port = 1883;
    const char * IP = "192.168.0.6";
    const char *mqttclient = "ESP32_2";  // Name of our device, must be unique
    const char *ssid =  "vodafoneAAFAVK";   // name of your WiFi network
    const char *password = "KfPTnPFpTkgL6qPF"; // password of the WiFi network
    bool state = 0;

    /////////////////////////////////////////////////////////////////////////////

    //Response:
    void SetResponse();
};
#endif
