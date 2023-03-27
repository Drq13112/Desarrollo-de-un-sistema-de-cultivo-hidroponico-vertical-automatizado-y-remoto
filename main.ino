#include "Libraries.h"
#include "Variables.h"

#define measurements 10
#include "Arduino.h"
//void ReadMessageFromNano();
//void SendMessageToNano();
void regulate_pH();
void regulate_Solution();
void Increase_pH();
void Decrease_pH();
void Increase_TDS();
void Decrease_TDS();
void callback(char *topic, byte *payload, unsigned int length);
void Publish(float message, const char *topic);

String Response_string = "";
BTS7960 ThermalResistor;
LN298N WaterPump;
Relays Electrovalvulas;
ESP32Time rtc;
WiFiClient wclient;
PubSubClient client(wclient);
DHT dht(DHTPIN, DHTTYPE);
EasyComma easyComma(9);
HardwareSerial SerialPort(2);
Separador s;
OneWire oneWire(WaterTempPin);
DallasTemperature WaterTempSensor(&oneWire);
PID::PIDParameters<double> parameters(4.0, 0.2, 1);
PID::PIDController<double> pidController(parameters);
void setup() {

  Serial.begin(9600);
  // Start serial communication at 115200 baud
  SerialPort.begin(115200, SERIAL_8N1, RXp2, TXp2);

  delay(50);
  WaterPump.SetUp();
  ThermalResistor.SetUp();
  dht.begin();
  Electrovalvulas.SetUp();
  setup_wifi();
  WaterTempSensor.begin();
  minute1 = rtc.getMinute();
  day_initial = rtc.getDayofYear();

  delay(3000);
  reconnect();
  delay(100);

  // Configuro hora:
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    rtc.setTimeStruct(timeinfo);
  }

  // Subscribing to all topics
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
  client.subscribe(Topics.Update_petition);
  client.subscribe(Topics.Photo_petition);
  client.subscribe(Topics.Water_pump_fdbk);
  Publish(0, Topics.pH_Uppered);
  Publish(0, Topics.Water_Uppered);
  Publish(0, Topics.pH_Decreased);
  Publish(0, Topics.Nutrient_Uppered);

  pidController.Input = analogRead(Water_temperature);
  pidController.Setpoint = Water_temp_Setpoint;
  pidController.TurnOn();
}
void loop() {
  //////////////////////////////////////////////////////////////////////

  minute2 = rtc.getMinute();
  day = rtc.getDayofYear();

  // Semana 1-> 1,5 ml // CE entre 1,0
  // Smena 2-> 1,5 ml
  // Semana 3-> 2 ml
  // Semana 4-> 2 ml
  // Semana 5-> 4,5 ml
  // Semana 6-> 4,5 ml
  // Semana 7-> 4,5 ml
  // Semana 8-> 5 ml
  // Semana 9-> 5 ml // CE 2.5
  // pH entre 5,5 – 5,8

  // Accionamiento remoto de la bomba
  if (Update_Pump_state == true) {
    WaterPump.Run(100);
    Serial.println("Water pump ON");
    Pump_timer = rtc.getMinute();
    if ((Pump_timer - minute2) == 5) {
      Update_Pump_state = false;
    }
  }
  if (Remote_Decrease_pH == true) {
    Decrease_pH();
    Publish(0, Topics.pH_Decreased);
    Remote_Decrease_pH = false;
  }
  if (Remote_Increase_pH == true) {
    Increase_pH();
    Publish(0, Topics.pH_Uppered);
    Remote_Increase_pH = false;
  }

  if (Remote_Increase_TDS == true) {
    Remote_Increase_TDS = false;
    Increase_TDS();
    Publish(0, Topics.Nutrient_Uppered);
  }
  if (Remote_Water_UP == true) {
    Remote_Water_UP = false;
    Decrease_TDS();
    Publish(0, Topics.Water_Uppered);
  }

  // Check and control of water
  /*
  // Control PID de la temperatura

  pidController.Input = analogRead(Water_temperature);
  pidController.Update();
  ThermalResistor.Run(pidController.Output);
  Serial.print("PID output:");
  Serial.println(pidController.Output);*/
  
    SendMessageToNano();
    delay(100);
    ReadMessageFromNano();
    delay(100);
  //
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  //Water Temperature
  WaterTempSensor.requestTemperatures();
  Water_temperature = WaterTempSensor.getTempCByIndex(0);


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
  Serial.print("TDS Value TDS:");
  Serial.print(tdsValue, 0);
  Serial.println("ppm");
  Serial.print("pH:");
  Serial.println(pH, 1);
  delay(1000);

  if (abs(minute2 - minute1) >= Cycle_time || Update_data_flag == true) {
    if (Update_data_flag == true) {
      Serial.println("Update solicitated");
      Update_data_flag = false;
    }
    minute1 = minute2;
    /*
    SendMessageToNano();
    delay(100);
    ReadMessageFromNano();
    delay(100);
*/
    //
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

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

    Publish(humidity, Topics.Humidity);
    Publish(Nutrient_tank_level, Topics.Nutrient_tank_level);
    Publish(Tank_level, Topics.Tank_level);
    Publish(pH, Topics.pH);
    Publish(tdsValue, Topics.TDS);
    Publish(Water_flow, Topics.Water_Flow);
    Publish(Water_temperature, Topics.Water_temperature);
    Publish(temperature, Topics.Weather_temperature);
    Publish(pH_Elevator_Level, Topics.pH_elevator_tank_level);
    Publish(pH_Reductor_Level, Topics.pH_reductor_tank_level);
    Publish(Heater, Topics.Heater);

    // regulate_pH();
    // regulate_Solution();
  }
  client.loop();
}

void SendMessageToNano() {
  SerialPort.print("A");  // I use -2 to indicate that it's a message from ESP to Nano asking for measurements
  // Serial.println("Petition sent");
}

void ReadMessageFromNano() {
  delay(1000);  // Time to wait until the response
  // Serial.println("Message Received: ");
  message_from_arduino = SerialPort.readString();
  // Serial.println(message_from_arduino);

  String value = s.separa(message_from_arduino, ',', 1);
  tdsValue = value.toFloat();

  value = s.separa(message_from_arduino, ',', 2);
  pH = value.toFloat();

  value = s.separa(message_from_arduino, ',', 3);
  //Water_temperature = value.toFloat();

  value = s.separa(message_from_arduino, ',', 4);
  Nutrient_tank_level = value.toFloat();

  value = s.separa(message_from_arduino, ',', 5);
  Tank_level = value.toFloat();

  value = s.separa(message_from_arduino, ',', 6);
  pH_Elevator_Level = value.toFloat();

  value = s.separa(message_from_arduino, ',', 7);
  pH_Reductor_Level = value.toFloat();

  value = s.separa(message_from_arduino, ',', 8);
  Water_Reserve_Level = value.toFloat();
}
void Interprete_values() {
  // Distances
}
void regulate_pH() {

  // pH>rango max
  if (pH > MAX_pH) {
    Decrease_pH();
  }

  // pH<rango minimo
  if (pH < MIN_pH) {
    Increase_pH();
  }
}
void regulate_Solution() {
  // TDS>rango max
  if (tdsValue > MAX_EC) {
    Decrease_TDS();
  }

  // TDS<rango minimo
  if (tdsValue < MIN_EC) {
    Increase_TDS();
  }
}

void Increase_pH() {
  Electrovalvulas.OpenRelay(2);
  delay(Time_pH_Up);
  Electrovalvulas.CloseRelay(2);
  Publish(1, Topics.pH_Uppered);
  Serial.print("Current pH value: ");
  Serial.println(pH);
  delay(2000);
}
void Decrease_pH() {
  Electrovalvulas.OpenRelay(3);
  delay(Time_pH_Decrease);
  Electrovalvulas.CloseRelay(3);
  Publish(1, Topics.pH_Decreased);
  Serial.print("Current pH value: ");
  Serial.println(pH);
  delay(2000);
}
void Increase_TDS() {
  Electrovalvulas.OpenRelay(5);
  Electrovalvulas.OpenRelay(6);
  delay(Time_Nutrient);
  Electrovalvulas.CloseRelay(5);
  Electrovalvulas.CloseRelay(6);
  Publish(1, Topics.Nutrient_Uppered);
  Serial.print("Current TDS value: ");
  Serial.println(tdsValue);
  delay(2000);
}
void Decrease_TDS() {
  Electrovalvulas.OpenRelay(4);
  delay(Time_Water);
  Electrovalvulas.CloseRelay(4);
  Publish(1, Topics.Water_Uppered);
  Serial.print("Current TDS value: ");
  Serial.println(tdsValue);
  delay(2000);
}

void callback(char *topic, byte *payload, unsigned int length) {

  String response = "";
  for (int i = 0; i < length; i++) {
    response = ((char)payload[i]);
  }
  if (String(topic) == "Hydroponic/Update_petition") {
    if (response == "1") {
      Update_data_flag = true;
    } else {
      Update_data_flag = false;
    }
  }
  if (String(topic) == "Hydroponic/Water_pump_fdbk") {
    if (response == "1") {
      Update_Pump_state = true;
    } else {
      Update_Pump_state = false;
    }
  }

  if (String(topic) == "Hydroponic/pH_Up") {
    if (response == "1") {
      Remote_Increase_pH = true;
    } else {
      Remote_Increase_pH = false;
    }
  }
  if (String(topic) == "Hydroponic/pH_Decrease") {
    if (response == "1") {
      Remote_Decrease_pH = true;
    } else {
      Remote_Decrease_pH = false;
    }
  }

  if (String(topic) == "Hydroponic/Nutrient_Up") {
    if (response == "1") {
      Remote_Increase_TDS = true;
    } else {
      Remote_Increase_TDS = false;
    }
  }

  if (String(topic) == "Hydroponic/Water_Up") {
    if (response == "1") {
      Remote_Water_UP = true;
      Decrease_TDS();
      Publish(0, Topics.Water_Uppered);
    } else {
      Remote_Water_UP = false;
    }
  }
  if (String(topic) == "Hydroponic/Heater_control") {
    Response_string = response;
    // Setting the new Setpoint
    Water_temp_Setpoint = float(Response_string.toInt());
  }
}

void setup_wifi() {

  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);  // Connect to network

  while (WiFi.status() != WL_CONNECTED) {  // Wait for connection
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

  client.setServer(IP, Port);
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

void Publish(float message, const char *topic) {

  char message_to_upload[7];

  // We pass from a float number into a const char
  dtostrf(message, 6, 1, message_to_upload);
  client.publish(topic, message_to_upload);
}


