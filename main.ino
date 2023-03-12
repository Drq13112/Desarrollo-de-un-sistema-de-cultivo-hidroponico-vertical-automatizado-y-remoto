#include "Libraries.h"
#include "Variables.h"

void ReadMessage();

float temperature = 20, tdsValue = -80;
float Water_temperature = 20, pH = 7;
float humidity = 60, Water_flow = 0, Nutrient_tank_level = 60, Tank_level = 70;
float pH_Elevator_Level = 0, pH_Reductor_Level = 0, Water_Reserve_Level = 0;

DHT dht(DHTPIN, DHTTYPE);  // I have to read using a pin GPIO
MQTT mqtt;
EasyComma easyComma(9);
HardwareSerial SerialPort(2);
Separador s;
BTS7960 ThermalResistor;
LN298N WaterPump;
Relays Electrovalvulas;

void setup() {

  Serial.begin(9600);  // Start serial communication at 115200 baud
  SerialPort.begin(115200, SERIAL_8N1, RXp2, TXp2);
  dht.begin();
  WaterPump.SetUp();
  ThermalResistor.SetUp();
  Electrovalvulas.SetUp();
  /*
  mqtt.setup_wifi();
  delay(3000);
  mqtt.reconnect();
  mqtt.Subscribe(Topics.Water_pump);
  */
}
void loop() {

  for (int i=0;i<5;i++)
  {
    Electrovalvulas.OpenRelay(i);
    delay(1000);
  }
  
  
  
  //////////////////////////////////////////////////////////////////////
  ReadMessage();
  delay(50);
 
  //
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  //

  /////////////////////////////////////////////////////////////////////

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
  Serial.println(pH, 1);
  /*
  
  mqtt.Publish(humidity, Topics.Humidity);
  mqtt.Publish(Nutrient_tank_level, Topics.Nutrient_tank_level);
  mqtt.Publish(Tank_level, Topics.Tank_level);
  mqtt.Publish(pH, Topics.pH);
  mqtt.Publish(tdsValue, Topics.TDS);
  mqtt.Publish(Water_flow, Topics.Water_Flow);
  mqtt.Publish(Water_temperature, Topics.Water_temperature);
  mqtt.Publish(temperature, Topics.Weather_temperature);
  //mqtt.Publish(temperature, Topics.Weather_temperature);
  mqtt.Loop();
  */
  /*
  Serial.print("Repite");
 
  */
  delay(3000);
}

void ReadMessage() {
  Serial.println("Message Received: ");
  message_from_arduino = SerialPort.readString();
  Serial.println(message_from_arduino);

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