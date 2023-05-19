#include <SoftwareSerial.h>
#include <string.h>
#define rxPin 0
#define txPin 1
#define pH_UP 11
#define pH_DOWN 2
#define NUTRIENT1 13
#define NUTRIENT2 12

void ReadMessageFromESP();
void ActivateRalays();
void SendMessageToESP();
void InitRelays();
int Relay1 = 0;
int Relay2 = 0;
int Relay3 = 0;
int Relay4 = 0;
int Relay5 = 0;
int Relay6 = 0;
int Relay7 = 0;
int Relay8 = 0;
int Array_pin[] = {3, 4, 5, 6, 7, 8, 9, 10};
int Array[] = {Relay1, Relay2, Relay3, Relay4, Relay5, Relay6, Relay7, Relay8};
int pH_UP_state = 0;
int pH_DOWN_state = 0;
int NUTRIENT1_state = 0;
int NUTRIENT2_state = 0;
String Message_from_ESP;
String value;
SoftwareSerial SerialPort(0, 1);

void setup()
{
  // put your setup code here, to run once
  for (size_t i = 0; i < 8; i++)
  {
    pinMode(Array_pin[i], OUTPUT);
  }
  InitRelays();
  pinMode(Relay1, OUTPUT);
  pinMode(pH_UP, INPUT);
  pinMode(pH_DOWN, INPUT);
  pinMode(NUTRIENT1, INPUT);
  pinMode(NUTRIENT2, INPUT);
  SerialPort.begin(115200);
  Serial.begin(115200);
}

void loop()
{
  ReadMessageFromESP();
}
void ReadMessageFromESP()
{
  // ESP is asking for measurements
  if (SerialPort.available())
  {
    char buffer = SerialPort.read();
    if (buffer == 'A')
    {
      //Serial.print(buffer);
      //Serial.print(",");
      for (size_t i = 0; i < 8; i++)
      {
        char buffer = SerialPort.read();
        
         int value = buffer - '0';
         if (value == 1)
         {
          Array[i]= value;
         }else
         {
          Array[i]=0;
         }
        //Serial.print(buffer);
        //Serial.print(",");
      }
      //Serial.println();
      // Message_from_ESP = SerialPort.readString();
      /*
      for (size_t i = 0; i < 8; i++)
      {
        Serial.print(Array[i]);
      }
      Serial.println();*/
      ActivateRalays();
    }
    if (buffer == 'T')
    {
      //Serial.println("Tanks");
      pH_UP_state = digitalRead(pH_UP);
      pH_DOWN_state = digitalRead(pH_DOWN);
      NUTRIENT1_state = digitalRead(NUTRIENT1);
      NUTRIENT2_state = digitalRead(NUTRIENT2);
      SendMessageToESP();
    }
  }
}
void ActivateRalays()
{
  //Serial.print("Activando reles");
  for (size_t i = 0; i < 8; i++)
  {
    if (Array[i] == 1)
    {
      digitalWrite(Array_pin[i], LOW);
      //Serial.print("LOW ");
    }
    else
    {
      digitalWrite(Array_pin[i], HIGH);
      //Serial.print("HIGH ");
    }
  }
  //Serial.println();
}
void SendMessageToESP()
{
  Serial.print('S');
  Serial.print(',');
  Serial.print(pH_UP_state);
  Serial.print(',');
  Serial.print(pH_DOWN_state);
  Serial.print(',');
  Serial.print(NUTRIENT1_state);
  Serial.print(',');
  Serial.print(NUTRIENT2_state);
  Serial.print(',');
  Serial.println();
} void InitRelays()
{
  for (size_t i = 0; i < 8; i++)
  {
    Array[i] = 0;
    digitalWrite(Array_pin[i], HIGH);
  }
}
