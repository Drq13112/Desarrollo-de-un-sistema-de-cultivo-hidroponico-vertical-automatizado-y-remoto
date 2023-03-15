#include "Libraries.h"
#include "Variables.h"

void ReadMessageFromNano();
void SendMessageToNano();

float temperature = 20, tdsValue = -80;
float Water_temperature = 20, pH = 7;
float humidity = 60, Water_flow = 0, Nutrient_tank_level = 60, Tank_level = 70;
float pH_Elevator_Level = 0, pH_Reductor_Level = 0, Water_Reserve_Level = 0;
float Heater=0;

BTS7960 ThermalResistor;
LN298N WaterPump;
Relays Electrovalvulas;

DHT dht(DHTPIN, DHTTYPE);  // I have to read using a pin GPIO
MQTT mqtt;
EasyComma easyComma(9);
HardwareSerial SerialPort(2);
Separador s;


void setup() {

  Serial.begin(9600);
  // Start serial communication at 115200 baud
  SerialPort.begin(115200, SERIAL_8N1, RXp2, TXp2);
  
  delay(50);
  WaterPump.SetUp();
  delay(50);
  ThermalResistor.SetUp();
  delay(50);
  dht.begin();
  Electrovalvulas.SetUp();
  
  mqtt.setup_wifi();
  delay(3000);
  mqtt.reconnect();
  delay(100);
  //Subscribing to all topics
  /*
  mqtt.Subscribe(Topics.Water_pump);
  mqtt.Subscribe(Topics.pH);
  mqtt.Subscribe(Topics.Humidity);
  mqtt.Subscribe(Topics.Water_temperature);
  mqtt.Subscribe(Topics.Weather_temperature);
  mqtt.Subscribe(Topics.Water_Flow);
  mqtt.Subscribe(Topics.Heater);
  mqtt.Subscribe(Topics.Nutrient_tank_level);
  mqtt.Subscribe(Topics.pH_elevator_tank_level);
  mqtt.Subscribe(Topics.pH_reductor_tank_level);
  */
  mqtt.Subscribe(Topics.Update_petition);
  mqtt.Subscribe(Topics.Photo_petition);
  mqtt.Subscribe(Topics.Water_pump_fdbk);
}
void loop() {
  //////////////////////////////////////////////////////////////////////
  
  SendMessageToNano();
  ReadMessageFromNano();
  delay(50);

  //
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  //

  /////////////////////////////////////////////////////////////////////
/*
  Serial.println("");
  Serial.print("Water_temperature:");
  Serial.print(Water_temperature);
  Serial.println("ºC");
  Serial.print("Humidity:");
  Serial.print(humidity);
  Serial.println("%");
  Serial.print("Temperature:");
  Serial.print(temperature);
  Serial.println("ºC");
  Serial.print("TDS Value:");
  Serial.print(tdsValue, 0);
  Serial.println("ppm");
  Serial.print("pH:");
  Serial.println(pH, 1);*/

  mqtt.Publish(humidity, Topics.Humidity);
  mqtt.Publish(Nutrient_tank_level, Topics.Nutrient_tank_level);
  mqtt.Publish(Tank_level, Topics.Tank_level);
  mqtt.Publish(pH, Topics.pH);
  mqtt.Publish(tdsValue, Topics.TDS);
  mqtt.Publish(Water_flow, Topics.Water_Flow);
  mqtt.Publish(Water_temperature, Topics.Water_temperature);
  mqtt.Publish(temperature, Topics.Weather_temperature);
  mqtt.Publish(pH_Elevator_Level, Topics.pH_elevator_tank_level);
  mqtt.Publish(pH_Reductor_Level, Topics.pH_reductor_tank_level);
  mqtt.Publish(Heater, Topics.Heater);
  mqtt.Loop();

  delay(3000);
}

void SendMessageToNano() {
  SerialPort.print("A");  // I use -2 to indicate that it's a message from ESP to Nano asking for measurements
  //Serial.println("Petition sent");
}

void ReadMessageFromNano() {
  delay(1000);  // Time to wait until the response
  //Serial.println("Message Received: ");
  message_from_arduino = SerialPort.readString();
  //Serial.println(message_from_arduino);

  String value = s.separa(message_from_arduino, ',', 1);
  tdsValue = value.toFloat();

  value = s.separa(message_from_arduino, ',', 2);
  pH = value.toFloat();
  delay(50);

  value = s.separa(message_from_arduino, ',', 3);
  Water_temperature = value.toFloat();
  delay(50);

  value = s.separa(message_from_arduino, ',', 4);
  Nutrient_tank_level = value.toFloat();
  delay(50);

  value = s.separa(message_from_arduino, ',', 5);
  Tank_level = value.toFloat();
  delay(50);

  value = s.separa(message_from_arduino, ',', 6);
  pH_Elevator_Level = value.toFloat();
  delay(50);

  value = s.separa(message_from_arduino, ',', 7);
  pH_Reductor_Level = value.toFloat();
  delay(50);

  value = s.separa(message_from_arduino, ',', 8);
  Water_Reserve_Level = value.toFloat();
  delay(50);
}