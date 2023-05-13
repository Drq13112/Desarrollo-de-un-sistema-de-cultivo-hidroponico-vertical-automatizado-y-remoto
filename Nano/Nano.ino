#include <SoftwareSerial.h>
#include <string.h>
#define rxPin 0
#define txPin 1

void ReadMessageFromESP();
void ActivateRalays();
int Relay1 = 0;
int Relay2 = 0;
int Relay3 = 0;
int Relay4 = 0;
int Relay5 = 0;
int Relay6 = 0;
int Relay7 = 0;
int Relay8 = 0;
int Array_pin[] = { 3, 4, 5, 6, 7, 8, 9, 10 };
int Array[] = {Relay1, Relay2, Relay3, Relay4, Relay5, Relay6, Relay7, Relay8 };
int state;
String Message_from_ESP;
String value;
SoftwareSerial SerialPort(0, 1);

void setup() {
  // put your setup code here, to run once
  for (size_t i=0;i<8; i++)
  {
    pinMode(Array_pin[i], OUTPUT);
  }
  pinMode(Relay1, OUTPUT);
  SerialPort.begin(115200);
  Serial.begin(115200);
}

void loop() {
  ReadMessageFromESP();
  ActivateRalays();
}
void ReadMessageFromESP() {
  // ESP is asking for measurements
  if (SerialPort.available()) {
    char buffer = SerialPort.read();
    if (buffer == 'A') {
      Serial.print(buffer);
      for (size_t i = 0; i < 8; i++) {
        char buffer = SerialPort.read();
        Array[i] = buffer - '0';
        Serial.print(buffer);
        Serial.print(",");
      }
      Serial.println();
      //Message_from_ESP = SerialPort.readString();
      for (size_t i = 0; i < 8; i++) {

        Serial.print(Array[i]);
      }
      Serial.println();
    }
  }
}
void ActivateRalays() {

  for (size_t i = 0; i < 8; i++) {
    if (Array[i] == 1) {
      digitalWrite(Array_pin[i], LOW);
    } else {
      digitalWrite(Array_pin[i], HIGH);
    }
  }
}
