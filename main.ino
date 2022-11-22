#include "Libraries.h"
#include "Variables.h"

float temperature=20,tdsValue=0;
float Water_temperature=20, pH=0;
float humidity=0;

TDS TdsSensor(TdsSensorPin,temperature);
DHT dht(DHTPIN,DHTTYPE);
OneWire oneWire(WaterTempPin);
DallasTemperature WaterTempSensor(&oneWire);
MQTT mqtt;

void setup() {

  Serial.begin(115200); // Start serial communication at 115200 baud
  WaterTempSensor.begin();
  dht.begin();
  delay(10000);
  mqtt.setup_wifi();
  mqtt.reconnect();
  mqtt.Subscribe(Topics.Humidity);
}
void loop()
{
  /*
  //////////////////////////////////////////////////////////////////////
  
  //
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  
  //
  WaterTempSensor.requestTemperatures(); 
  Water_temperature = WaterTempSensor.getTempCByIndex(0);

  //
  TdsSensor.setTemperature(Water_temperature);
  tdsValue=TdsSensor.getTDSValue();

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
  Serial.print(tdsValue,0);
  Serial.println("ppm"); 
  Serial.print("pH:"); 
  Serial.println(pH,1);  
  */
  mqtt.Publish(99.0, Topics.Humidity);
  mqtt.Loop();
  delay(3000); 
}
