#include "Libraries.h"
#include "Variables.h"

float temperature=20,tdsValue=0;
float Water_temperature=20, pH=7;
float humidity=60,Water_flow=0,Nutrient_tank_level=60,Tank_level=70;

ADS1115_PARALLEL ADS;
TDS TdsSensor(TdsSensorPin,temperature);
DHT dht(DHTPIN,DHTTYPE); // I have to read using a pin GPIO
OneWire oneWire(WaterTempPin);
DallasTemperature WaterTempSensor(&oneWire);
MQTT mqtt;

Adafruit_ADS1115 ads1;
Adafruit_ADS1115 ads2;
Adafruit_ADS1115 ads3;
Adafruit_ADS1115 ads4;

void setup() {

  Serial.begin(115200); // Start serial communication at 115200 baud
  ADS.SetUp();
  WaterTempSensor.begin();
  dht.begin();
  mqtt.setup_wifi();
  delay(3000);
  mqtt.reconnect();
  mqtt.Subscribe(Topics.Water_pump);
}
void loop()
{
  
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
  pH= analogRead(pHPin)*14.0/4096.0;
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
  delay(3000); 
}
