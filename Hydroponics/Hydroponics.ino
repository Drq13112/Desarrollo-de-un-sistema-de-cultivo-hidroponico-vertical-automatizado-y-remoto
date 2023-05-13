#include "Libraries.h"
#include "Menu.h"
#include "Variables.h"
#include "FlowMeter.h"
#include "Eeprom.hpp"
#include "Certs.h"

// Functions
void regulate_pH();
void regulate_Solution();
void Increase_pH();
void Decrease_pH();
void Increase_TDS();
void Decrease_TDS();
void callback(char *topic, byte *payload, unsigned int length);
void Publish(float message, const char *topic);
void Update_all_data();
void PID_Control();
void Reset_ESP();
void Reset_Settings();
void Control_Action();
void SendMessageToNano();
void Pump_off();
void Pump_on();

// Objects
String Response_string = "";
BTS7960 ThermalResistor;
//NewPing sonar(Trigger_PIN, Echo_PIN, MAX_DISTANCE);
Ultrasonic sonar(Trigger_PIN, Echo_PIN);
Relays Electrovalvulas;
WiFiClientSecure espClient = WiFiClientSecure();
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);
HardwareSerial SerialPort(0);
Separador s;
GravityTDS TdsSensor;
OneWire oneWire(WaterTempPin);
DallasTemperature WaterTempSensor(&oneWire);
Menu MiMenu;
FlowMeter FlowMeter1(WaterFLow_PIN);
PID::PIDParameters<double> parameters(Kp, Ki, Kd);
PID::PIDController<double> pidController(parameters);

void setup() {
  Serial.begin(9600);
  SerialPort.begin(115200, SERIAL_8N1, RXp0, TXp0);
  MiMenu.begin();

  // WIFI
  setup_wifi();
  reconnect();
  client.setCallback(callback);

  //
  // LOAD SETTINGS
  LoadSettings();

  // Sensors and actuators
  ThermalResistor.SetUp(255);
  dht.begin();
  sonar.SetUp();
  Electrovalvulas.SetUp();
  TdsSensor.begin();
  WaterTempSensor.begin();
  FlowMeter1.setup();

  //
  pinMode(Nutrient1_Elevator_PIN, INPUT);
  pinMode(Nutrient2_Elevator_PIN, INPUT);
  pinMode(pH_Elevator_PIN, INPUT);
  pinMode(pH_Reductor_PIN, INPUT);

  // Subscribing to  topics
  client.subscribe(Topics.Update_petition);
  client.subscribe(Topics.Water_Pump_Remote);
  client.subscribe(Topics.MAX_EC_Remote);
  client.subscribe(Topics.MIN_EC_Remote);
  client.subscribe(Topics.MAX_pH_Remote);
  client.subscribe(Topics.MIN_pH_Remote);
  client.subscribe(Topics.Temp_SP_Remote);
  client.subscribe(Topics.Process_ON_Remote);
  client.subscribe(Topics.pH_Up);
  client.subscribe(Topics.pH_Decrease);
  client.subscribe(Topics.Water_Up);
  client.subscribe(Topics.Nutrient_Up);
  client.subscribe(Topics.Reset_Pushed_Remote);
  client.subscribe(Topics.Reset_Settings);
  client.subscribe(Topics.Save_Settings_Remote);
  client.subscribe(Topics.Time_Pump_OFF_Remote);
  client.subscribe(Topics.Time_Pump_ON_Remote);
  delay(10);
  // Publish initial values
  Publish(0, Topics.pH_Uppered);
  Publish(0, Topics.Water_Uppered);
  Publish(0, Topics.pH_Decreased);
  Publish(0, Topics.Nutrient_Uppered);
  Publish(0, Topics.Water_Pump);
  Publish(1, Topics.Low_Nutrient_Tank);
  Publish(1, Topics.Low_pH_Elevator_Tank);
  Publish(1, Topics.Low_pH_Reductor_Tank);
  Publish(MAX_EC, Topics.MAX_EC);
  Publish(MIN_EC, Topics.MIN_EC);
  Publish(MAX_pH, Topics.MAX_pH);
  Publish(MIN_pH, Topics.MIN_pH);
  Publish(Water_temp_Setpoint, Topics.Temp_SP);
  Publish(Cycle_time, Topics.Cycle_Time);
  Publish(Time_Pump_ON, Topics.Time_Pump_ON);
  Publish(Time_Pump_OFF, Topics.Time_Pump_OFF);
  Publish(Kp, Topics.Kp);
  Publish(Kd, Topics.Kd);
  Publish(Ki, Topics.Ki);
  Publish(Process_ON, Topics.Process_ON);

  // PID
  pidController.Input = analogRead(Water_temperature);
  pidController.Setpoint = Water_temp_Setpoint;
  pidController.TurnOn();

  // MENU
  MiMenu.SetUp();
  Serial.println("Setup Finished");
}
void loop() {
  MiMenu.MainMenu();
  if (Update_data_flag == true) {
    Serial.println("Update solicitated");
    Update_data_flag = false;
    Update_all_data();
  }
  client.loop();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Pump_on() {
  Relay5 = 1;
  Relay1 = 1;
  SendMessageToNano();
}
void Pump_off() {
  Relay5 = 0;
  Relay1 = 0;
  SendMessageToNano();
}
void Control_Action() {
  Update_all_data();
  regulate_pH();
  regulate_Solution();
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
  if (ECValue > MAX_EC) {
    Decrease_TDS();
  }

  // TDS<rango minimo
  if (ECValue < MIN_EC) {
    Increase_TDS();
  }
}
void Increase_pH() {
  Publish(1, Topics.pH_Uppered);
  Relay2 = 1;
  SendMessageToNano();
  delay(Time_pH_Up);
  Relay2 = 0;
  SendMessageToNano();
  Publish(0, Topics.pH_Uppered);
}
void Decrease_pH() {
  Publish(1, Topics.pH_Decreased);
  Relay3 = 1;
  SendMessageToNano();
  delay(Time_pH_Decrease);
  Relay3 = 0;
  SendMessageToNano();
  Publish(0, Topics.pH_Decreased);
}
void Increase_TDS() {
  Publish(1, Topics.Nutrient_Uppered);
  Electrovalvulas.OpenRelay(7);
  Relay6 = 1;
  Relay8 = 1;
  SendMessageToNano();
  delay(Time_Nutrient);
  Relay8 = 0;
  Relay6 = 0;
  SendMessageToNano();
  Publish(0, Topics.Nutrient_Uppered);
}
void Decrease_TDS() {
  Publish(1, Topics.Water_Uppered);
  Relay7 = 1;
  SendMessageToNano();
  delay(Time_Water);
  Relay7 = 0;
  SendMessageToNano();
  Publish(0, Topics.Water_Uppered);
}

void callback(char *topic, byte *payload, unsigned int length) {
  String response = "";
  for (int i = 0; i < length; i++) {
    response += (char)payload[i];
  }
  // Serial.print("incoming: ");
  // Serial.println(topic);
  // StaticJsonDocument<200> doc;
  // deserializeJson(doc, payload);
  // const char *response = doc["message"];
  // Serial.println(message);
  Serial.print(topic);
  Serial.print(" message: ");
  Serial.println(response);

  if (String(topic) == Topics.Update_petition) {
    Update_all_data();
  }
  if (String(topic) == Topics.Water_Pump_Remote) {
    if (response == "1") {
      Pump_on();
    } else {
      Pump_off();
    }
  }
  if (String(topic) == Topics.pH_Up && Process_ON == false) {
    Increase_pH();
  }
  if (String(topic) == Topics.pH_Decrease && Process_ON == false) {
    Decrease_pH();
  }
  if (String(topic) == Topics.Nutrient_Up && Process_ON == false) {
    Increase_TDS();
  }
  if (String(topic) == Topics.Water_Up && Process_ON == false) {
    Decrease_TDS();
  }
  if (String(topic) == Topics.Temp_SP_Remote) {
    Response_string = response;
    Water_temp_Setpoint = Response_string.toFloat();
  }
  if (String(topic) == Topics.MAX_EC_Remote) {
    Response_string = response;
    MAX_EC = Response_string.toFloat();
  }
  if (String(topic) == Topics.MIN_EC_Remote) {
    Response_string = response;
    MIN_EC = Response_string.toFloat();
  }
  if (String(topic) == Topics.MAX_pH_Remote) {
    Response_string = response;
    MAX_pH = Response_string.toFloat();
  }
  if (String(topic) == Topics.MIN_pH_Remote) {
    Response_string = response;
    MIN_pH = Response_string.toFloat();
  }
  if (String(topic) == Topics.Process_ON_Remote) {
    if (response == "1") {
      Process_ON = true;
    } else {
      Process_ON = false;
    }
  }
  if (String(topic) == Topics.Reset_Pushed_Remote) {
    Reset_ESP();
  }
  if (String(topic) == Topics.Save_Settings_Remote) {
    SaveSettings();
  }
  if (String(topic) == Topics.Reset_Settings) {
    Reset_Settings();
  }
  if (String(topic) == Topics.Cycle_Time_Remote) {
    Response_string = response;
    Cycle_time = Response_string.toFloat();
  }
  if (String(topic) == Topics.Kp_Remote) {
    Response_string = response;
    Kp = Response_string.toFloat();
  }
  if (String(topic) == Topics.Kd_Remote) {
    Response_string = response;
    Kd = Response_string.toFloat();
  }
  if (String(topic) == Topics.Ki_Remote) {
    Response_string = response;
    Ki = Response_string.toFloat();
  }
  if (String(topic) == Topics.Time_Pump_OFF_Remote) {
    Response_string = response;
    Time_Pump_OFF = Response_string.toFloat();
  }
  if (String(topic) == Topics.Time_Pump_ON_Remote) {
    Response_string = response;
    Time_Pump_ON = Response_string.toFloat();
  }
  if (String(topic) == Topics.Control_Action && Process_ON == false) {
    Control_Action();
  }
}
void setup_wifi() {
  /*
  Serial.println("Initialiting System");
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password); // Connect to network

  while (WiFi.status() != WL_CONNECTED)
  { // Wait for connection
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());*/

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  espClient.setCACert(AWS_CERT_CA);
  espClient.setCertificate(AWS_CERT_CRT);
  espClient.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);

  Serial.println("Connecting to AWS IOT");
}

// Reconnect to client

void reconnect() {
  /*
  client.setServer(IP, Port);
  client.setCallback(callback);

  // Loop until we're reconnected
  while (!client.connected())
  {

    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32"))
    {
      Serial.println("connected");
      Serial.println('\n');
    }
    else
    {
      Serial.println(" try again in 5 seconds");
      Serial.println(client.state());
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }*/
  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }

  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }
  Serial.println("AWS IoT Connected!");
}

void Publish(float message, const char *topic) {
  /*
    Serial.print("TOPIC: ");
    Serial.println(topic);
    Serial.print("message: ");
    Serial.println(message);
    */
  char message_to_upload[7];

  // We pass from a float number into a const char
  dtostrf(message, 6, 1, message_to_upload);
  client.publish(topic, message_to_upload);
}

void Update_all_data() {
  // Checking regulator tanks
  Low_Nutrient_Tank_1 = digitalRead(Nutrient1_Elevator_PIN);
  Serial.print("Low_Nutrient_Tank_1: ");
  Serial.println(Low_Nutrient_Tank_1);
  Low_Nutrient_Tank_2 = digitalRead(Nutrient2_Elevator_PIN);
  Serial.print("Low_Nutrient_Tank_2: ");
  Serial.println(Low_Nutrient_Tank_2);
  Low_pH_Elevator_Tank = digitalRead(pH_Elevator_PIN);
  Serial.print("Low_pH_Elevator_Tank:");
  Serial.println(Low_pH_Elevator_Tank);
  Low_pH_Reductor_Tank = digitalRead(pH_Reductor_PIN);
  Serial.print("Low_pH_Reductor_Tank:");
  Serial.println(Low_pH_Reductor_Tank);

  if (Low_Nutrient_Tank_1 == 0.0 || Low_Nutrient_Tank_2 == 0.0) {
    Low_Nutrient_Tank_General = 0.0;
    Publish(0, Topics.Low_Nutrient_Tank);
  } else {
    Low_Nutrient_Tank_General = 1;
    Publish(1, Topics.Low_Nutrient_Tank);
  }
  delay(50);
  if (Low_pH_Elevator_Tank == 0) {
    Publish(0, Topics.Low_pH_Elevator_Tank);
  } else {
    Publish(1, Topics.Low_pH_Elevator_Tank);
  }
  delay(50);
  if (Low_pH_Reductor_Tank == 0) {
    Publish(0, Topics.Low_pH_Reductor_Tank);
  } else {
    Publish(1, Topics.Low_pH_Reductor_Tank);
  }
  // Nutrient Level tank
  Tank_level = Height_tank - sonar.getDistance();
  Serial.print("tank:");
  Serial.println(Tank_level);
  if (Tank_level < 0) {
    Tank_level = 0;
  }
  // 100cm -> 0%
  // 1cm -> 1%  -> 10L  aprox
  // 10cm -> 10% -> 100L aprox

  // Flow_Meter

  Water_flow = FlowMeter1.Measure();

  // Water Temperature
  WaterTempSensor.requestTemperatures();
  Water_temperature = WaterTempSensor.getTempCByIndex(0);
  if (Water_temperature == -127) {
    Water_temperature = 25;
  }

  // EC Value
  TdsSensor.setTemperature(Water_temperature);
  TdsSensor.update();
  // tdsValue = TdsSensor.getTdsValue();
  ECValue = TdsSensor.getEcValue();

  // pH Value
  pH = TdsSensor.getpH();

  // Weather Condition
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

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
  Serial.print("EC Value:");
  Serial.println(ECValue, 0);
  Serial.print("pH:");
  Serial.println(pH, 2);
  Serial.print("Tank level:");
  Serial.println(Tank_level, 0);
  Serial.print("Flow rate: ");
  Serial.print(Water_flow, 0);
  Serial.println("L/min");

  // Publish data. In case it's are having a measurement out of range it will be printed -1
  if (pH >= 0 && pH <= 14) {
    Publish(pH, Topics.pH);
  } else {
    Publish(-1, Topics.pH);
  }
  if (Tank_level >= 0 && Tank_level <= 100) {
    Publish(Tank_level, Topics.Tank_level);
  } else {
    Publish(-1, Topics.Tank_level);
  }
  if (ECValue >= 0) {
    Publish(ECValue, Topics.TDS);
  } else {
    Publish(-1, Topics.TDS);
  }
  Publish(Water_temperature, Topics.Water_temperature);
  if (humidity >= 0 && humidity <= 100) {
    Publish(humidity, Topics.Humidity);
  } else {
    Publish(-1, Topics.Humidity);
  }
  if (Water_flow >= 0) {
    Publish(Water_flow, Topics.Water_Flow);
  } else {
    Publish(-1, Topics.Water_Flow);
  }
  Publish(temperature, Topics.Weather_temperature);
  delay(1000);
}

void PID_Control() {
  parameters.Set(Kp, Kd, Ki);
  pidController.Input = analogRead(Water_temperature);
  pidController.Update();
  ThermalResistor.Run(pidController.Output);
  Serial.print("PID output:");
  Serial.println(pidController.Output);
}

void Reset_ESP() {
  ESP.restart();
}
void Reset_Settings() {
  ResetSettings();
  Publish(MAX_EC, Topics.MAX_EC);
  Publish(MIN_EC, Topics.MIN_EC);
  Publish(MAX_pH, Topics.MAX_pH);
  Publish(MIN_pH, Topics.MIN_pH);
  Publish(Water_temp_Setpoint, Topics.Temp_SP);
  Publish(Cycle_time, Topics.Cycle_Time);
  Publish(Kp, Topics.Kp);
  Publish(Kd, Topics.Kd);
  Publish(Ki, Topics.Ki);
}

void SendMessageToNano() {
  SerialPort.print('A');
  SerialPort.print(Relay1);
  SerialPort.print(Relay2);
  SerialPort.print(Relay3);
  SerialPort.print(Relay4);
  SerialPort.print(Relay5);
  SerialPort.print(Relay6);
  SerialPort.print(Relay7);
  SerialPort.print(Relay8);
  SerialPort.println("");
  delay(10);
  SerialPort.print('A');
  SerialPort.print(Relay1);
  SerialPort.print(Relay2);
  SerialPort.print(Relay3);
  SerialPort.print(Relay4);
  SerialPort.print(Relay5);
  SerialPort.print(Relay6);
  SerialPort.print(Relay7);
  SerialPort.print(Relay8);
  SerialPort.println("");
}