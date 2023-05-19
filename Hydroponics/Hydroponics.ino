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
void ReadMessageFromNano();
void Pump_off();
void Pump_on();
void RequestTankData();
void Fill_Tank();
void Stop_Fill_Tank();
void Update_PID_Parameters();
//Local varaibles
String message_from_arduino = "";
size_t contador = 0;
size_t contador_communication = 0;
// Objects
String Response_string = "";
BTS7960 ThermalResistor;
//NewPing sonar(Trigger_PIN, Echo_PIN, MAX_DISTANCE);
Ultrasonic sonar(Trigger_PIN, Echo_PIN);
//Relays Electrovalvulas;
WiFiClientSecure espClient = WiFiClientSecure();
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);
HardwareSerial SerialPort(2);
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
  ThermalResistor.SetUp(255);
  ThermalResistor.Stop();

  // WIFI
  setup_wifi();
  reconnect();
  client.setCallback(callback);

  // LOAD SETTINGS
  LoadSettings();

  // Sensors and actuators
  sonar.SetUp();
  dht.begin();
  //Electrovalvulas.SetUp();
  TdsSensor.begin();
  WaterTempSensor.begin();
  FlowMeter1.setup();
  delay(100);

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
  client.subscribe(Topics.Minimum_Level_Remote);
  client.subscribe(Topics.Control_Action);
  client.subscribe(Topics.Cycle_Time_Remote);
  client.subscribe(Topics.Check_Remote);
  client.subscribe(Topics.Kp_Remote);
  client.subscribe(Topics.Kd_Remote);
  client.subscribe(Topics.Ki_Remote);
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
  Publish(Minimum_Level, Topics.Minimum_Level);

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
  if (Process_ON == true) {
    contador++;
    //Serial.print("contador: ");
    //Serial.println(contador);
    if (contador >= Sampling_Time_Heater) {
      Serial.println("PID in loop: ");
      PID_Control();
      // Control the level of tank
      if (Filling == true) {
        Tank_level = (-100 / Height_tank) * sonar.getDistance() + 100;
        Serial.print("tank in loop:");
        Serial.println(Tank_level);
        if (Tank_level < 0) {
          Tank_level = 0;
        }
        if (Tank_level > Minimum_Level) {
          Stop_Fill_Tank();
          Filling = false;
        }
      }
      contador = 0;
    }
  }
  if (contador_communication >= Check_Communication) {
    Serial.println("Checking communication");
    contador_communication = 0;
    Publish(1, Topics.Check);
    Attempt++;
    if (Connected == false && Attempt >= 2) {
      Serial.println("Connection_Lost");
      SaveSettings();
      Reset_ESP();
    }
  }
  //Serial.print("contador_communication :");
  //Serial.println(contador_communication);
  contador_communication++;
  client.loop();
}
//////////////////////////////////////////////////////////////////////////////////////////////////
void Pump_on() {
  if (Filling == false) {
    Relay5 = 1;
    Relay1 = 1;
    SendMessageToNano();
  }
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
  Serial.print("increase_pH");
  delay(Time_pH_Up);
  Relay2 = 0;
  SendMessageToNano();
  Publish(0, Topics.pH_Uppered);
}
void Decrease_pH() {
  Publish(1, Topics.pH_Decreased);
  Relay4 = 1;
  Serial.print("Decrease_pH");
  SendMessageToNano();
  delay(Time_pH_Decrease);
  Relay4 = 0;
  SendMessageToNano();
  Publish(0, Topics.pH_Decreased);
}
void Increase_TDS() {
  Publish(1, Topics.Nutrient_Uppered);
  Relay6 = 1;
  Relay8 = 1;
  SendMessageToNano();
  Serial.print("increase_TDS");
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
  Serial.print("Decrease_TDS");
  delay(Time_Water);
  Relay7 = 0;
  SendMessageToNano();
  Publish(0, Topics.Water_Uppered);
}
void Fill_Tank() {
  Publish(1, Topics.Water_Uppered);
  Relay7 = 1;
  SendMessageToNano();
}
void Stop_Fill_Tank() {
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
      Update_all_data();
    } else {
      Process_ON = false;
      Filling = false;
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
    Update_PID_Parameters();
  }
  if (String(topic) == Topics.Kd_Remote) {
    Response_string = response;
    Kd = Response_string.toFloat();
    Update_PID_Parameters();
  }
  if (String(topic) == Topics.Ki_Remote) {
    Response_string = response;
    Ki = Response_string.toFloat();
    Update_PID_Parameters();
  }
  if (String(topic) == Topics.Time_Pump_OFF_Remote) {
    Response_string = response;
    Time_Pump_OFF = Response_string.toFloat();
  }
  if (String(topic) == Topics.Time_Pump_ON_Remote) {
    Response_string = response;
    Time_Pump_ON = Response_string.toFloat();
  }
  if (String(topic) == Topics.Control_Action && Process_ON == true) {
    Control_Action();
  }
  if (String(topic) == Topics.Minimum_Level_Remote) {
    Response_string = response;
    Minimum_Level = Response_string.toFloat();
  }
  if (String(topic) == Topics.Check_Remote) {
    Connected = true;
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
  Connected = true;
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
  Serial.print("Cycle_Time:");
  Serial.println(Cycle_time);
  // Checking regulator tanks
  RequestTankData();
  Serial.print("Low_Nutrient_Tank_1: ");
  Serial.println(Low_Nutrient_Tank_1);
  Serial.print("Low_Nutrient_Tank_2: ");
  Serial.println(Low_Nutrient_Tank_2);
  Serial.print("Low_pH_Elevator_Tank:");
  Serial.println(Low_pH_Elevator_Tank);
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
  Tank_level = (-100 / Height_tank) * sonar.getDistance() + 100;
  Serial.print("tank:");
  Serial.println(Tank_level);
  if (Tank_level < 0) {
    Tank_level = 0;
  }
  // 85cm -> 0%
  if (Process_ON == true && Filling == false) {
    if (Tank_level < Minimum_Level) {
      Fill_Tank();
      Filling = true;
    }
  }

  // Flow_Meter
  Water_flow = FlowMeter1.Measure();

  // Water Temperature
  WaterTempSensor.requestTemperatures();
  Water_temperature = WaterTempSensor.getTempCByIndex(0);
  if (Water_temperature == -127) {
    Water_temperature = prev_Water_temperature;
  } else {
    prev_Water_temperature = Water_temperature;
  }

  // EC Value
  TdsSensor.setTemperature(Water_temperature);
  TdsSensor.update();
  // tdsValue = TdsSensor.getTdsValue();
  ECValue = TdsSensor.getEcValue();
  if (ECValue < 0)
    ECValue = 0;

  // pH Value
  pH = TdsSensor.getpH();
  if (pH < 0)
    pH = 0;
  if (pH > 14)
    pH = 14;
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
  Serial.print("PID Output:");
  Serial.print(pidController.Output);

  // Publish data. In case it's are having a measurement out of range it will be printed -1
  Publish(pH, Topics.pH);
  Publish(Tank_level, Topics.Tank_level);
  Publish(ECValue, Topics.TDS);
  if (Water_temperature != -127) {
    Publish(Water_temperature, Topics.Water_temperature);
  }
  Publish(humidity, Topics.Humidity);
  Publish(Water_flow, Topics.Water_Flow);
  Publish(temperature, Topics.Weather_temperature);
  Publish(PID_OutPut, Topics.PID_OutPut);
}
void Update_PID_Parameters() {
  parameters.Set(Kp, Kd, Ki);
  pidController.Setpoint = Water_temp_Setpoint;
  pidController.TurnOn();
}
void PID_Control() {
  // Water Temperature
  WaterTempSensor.requestTemperatures();
  Water_temperature = WaterTempSensor.getTempCByIndex(0);
  if (Water_temperature == -127) {
    Water_temperature = prev_Water_temperature;
  } else {
    prev_Water_temperature = Water_temperature;
  }
  Serial.println("Water_temperature to PID: ");
  Serial.println(Water_temperature);
  pidController.Setpoint = Water_temp_Setpoint;
  Serial.print("Water_temperature: ")
  Serial.println(Water_temperature:)
  pidController.Update(Water_temperature);
  PID_OutPut = pidController.Output;
  ThermalResistor.Run(PID_OutPut);
  Serial.print("PID output:");
  Serial.println(PID_OutPut);
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
  SerialPort.print("A");
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
  SerialPort.print("A");
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
void RequestTankData() {
  SerialPort.println('T');
  delay(10);
  SerialPort.println('T');
  delay(500);
  ReadMessageFromNano();
}
void ReadMessageFromNano() {
  if (SerialPort.available()) {
    // Serial.println("Message Received: ");
    message_from_arduino = SerialPort.readString();
    Serial.println(message_from_arduino);
    String value = s.separa(message_from_arduino, ',', 0);
    if (value == "S") {
      String value = s.separa(message_from_arduino, ',', 1);
      Low_pH_Elevator_Tank = value.toFloat();

      value = s.separa(message_from_arduino, ',', 2);
      Low_pH_Reductor_Tank = value.toFloat();

      value = s.separa(message_from_arduino, ',', 3);
      Low_Nutrient_Tank_1 = value.toFloat();

      value = s.separa(message_from_arduino, ',', 4);
      Low_Nutrient_Tank_2 = value.toFloat();
    }
  }
}