#include "Libraries.h"
#include "Variables.h"

float temperature=20,tdsValue=0;
float Water_temperature=20, pH=0;
float humidity=0;

TDS TdsSensor;
DHT dht(DHTPIN,DHTTYPE);

//OneWire oneWire(WaterTempPin);
//DallasTemperature WaterTempSensor(&oneWire);

void setup() {

  Serial.begin(115200); // Start serial communication at 115200 baud
  TdsSensor.setPin(TdsSensorPin);
  TdsSensor.setTemperature(temperature);
  //WaterTempSensor.begin();
  pinMode(pHPin,INPUT);
  pinMode(WaterTempPin,INPUT);
  dht.begin();
}


void loop()
{
  //////////////////////////////////////////////////////////////////////
  //
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  //
  TdsSensor.setTemperature(Water_temperature);
  tdsValue=TdsSensor.getTDSValue();

  //
  //WaterTempSensor.requestTemperatures(); 
  //Water_temperature = WaterTempSensor.getTempCByIndex(0);
  Water_temperature=analogRead(WaterTempPin);

  //
  pH= analogRead(pHPin);
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
  Serial.print(tdsValue);
  Serial.println("ppm"); 
  Serial.print("pH:"); 
  Serial.println(pH);  
  
  delay(3000);

}

/*
void setup() {

  Serial.begin(115200); // Start serial communication at 115200 baud
  setup_wifi(); // Connect to network
  client.setServer(broker, Port);
}

void loop() {
  if (!client.connected())  // Reconnect if connection is lost
  {
    reconnect();
  }
  client.loop();
}
*/
