#include "Libraries.h"
#include "Variables.h"
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

String Response_string = "";
BTS7960 ThermalResistor;
NewPing sonar(Trigger_PIN, Echo_PIN, MAX_DISTANCE);
Relays Electrovalvulas;
ESP32Time rtc;
WiFiClient wclient;
PubSubClient client(wclient);
DHT dht(DHTPIN, DHTTYPE);
HardwareSerial SerialPort(2);
Separador s;
GravityTDS TdsSensor;
OneWire oneWire(WaterTempPin);
DallasTemperature WaterTempSensor(&oneWire);

PID::PIDParameters<double> parameters(Kp, Ki, Kd);
PID::PIDController<double> pidController(parameters);

void setup()
{

  Serial.begin(9600);
<<<<<<< HEAD:Hydroponics.ino
  inicializaLcd();
  incializaRotaryEncoder();

  // WaterPump.SetUp();
  // sensor.start();
=======
>>>>>>> d942c004b9b55ebde5c6986ff0de81a1605f7300:main_sin_nano.ino
  ThermalResistor.SetUp();
  dht.begin();
  Electrovalvulas.SetUp();
  //setup_wifi();
  TdsSensor.begin();
  WaterTempSensor.begin();
  minute1 = rtc.getMinute();
  day_initial = rtc.getDayofYear();

  void incializaRotaryEncoder();
// Incialización del lcd

void inicializaLcd();
  /*
    delay(3000);
    reconnect();
    delay(100);

    */

  // Configuro hora:
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if (getLocalTime(&timeinfo))
  {
    rtc.setTimeStruct(timeinfo);
  }
  /*
    // Subscribing to  topics
    client.subscribe(Topics.Update_petition);
    client.subscribe(Topics.Photo_petition);
    client.subscribe(Topics.Water_pump_fdbk);

    // Publish a initial value
    Publish(0, Topics.pH_Uppered);
    Publish(0, Topics.Water_Uppered);
    Publish(0, Topics.pH_Decreased);
    Publish(0, Topics.Nutrient_Uppered);
  */
  pidController.Input = analogRead(Water_temperature);
  pidController.Setpoint = Water_temp_Setpoint;
  pidController.TurnOn();
}
void loop()
{
  //////////////////////////////////////////////////////////////////////

  minute2 = rtc.getMinute();
  day = rtc.getDayofYear();

  if (Reset_Process == true)
  {
    day_initial = 0;
    Process_week = 0;
    Process_ON = false;
    Process_day = 0;
  }
  // Semana 0-> 1,5 ml // CE entre 1,0
  // Semena 1-> 1,5 ml
  // Semana 2-> 2 ml
  // Semana 3-> 2 ml
  // Semana 4-> 4,5 ml
  // Semana 5-> 4,5 ml
  // Semana 6-> 4,5 ml
  // Semana 7-> 5 ml
  // Semana 8-> 5 ml // CE 2.5
  // pH entre 5,5 – 5,8
  if (Process_ON == false)
    day_initial = rtc.getDayofYear();
    Serial.print("Day_initial: ");
    Serial.println(day_initial);


  // Depending on the week the the process is, we use some patameters or others.
  if (Process_ON == true)
  {
    Process_day = day - day_initial;

    if (Process_day % 7 == true)
      Process_week = Process_week + 1;

    // Modo automatico
    switch (Process_week)
    {
    case 0:
    case 1:
      MAX_pH = 6;
      MIN_pH = 5.5;
      MAX_EC = 1100;
      MIN_EC = 900;
      MAX_TEMP = 25;
      MIN_TEMP = 15;
      break;
    case 2:
    case 3:
      MAX_pH = 6;
      MIN_pH = 5.5;
      MAX_EC = 1700;
      MIN_EC = 1400;
      MAX_TEMP = 25;
      MIN_TEMP = 15;
      break;
    case 4:
    case 5:
    case 6:

      MAX_pH = 6;
      MIN_pH = 5.5;
      MAX_EC = 2000;
      MIN_EC = 1800;
      MAX_TEMP = 25;
      MIN_TEMP = 15;

      break;
    case 7:
    case 8:

      MAX_pH = 6;
      MIN_pH = 5.5;
      MAX_EC = 2500;
      MIN_EC = 2300;
      MAX_TEMP = 25;
      MIN_TEMP = 15;
      break;
    default:
      break;
    };

    // PID Control
    PID_Control();

    if (abs(minute2 - minute1) >= Cycle_time || Update_data_flag == true)
    {
      if (Update_data_flag == true)
      {
        Serial.println("Update solicitated");
        Update_data_flag = false;
      }
      minute1 = minute2;
      // Uploading parameters

      Update_all_data();
      regulate_pH();
      regulate_Solution();
    }
  }
  else
  {
    // Modo Manual

    // Accionamiento de la bomba
    if (Update_Pump_state == true)
    {
      // WaterPump.Run(100);
      Serial.println("Water pump ON");
      Pump_timer = rtc.getMinute();
      if ((Pump_timer - minute2) == 5)
      {
        Update_Pump_state = false;
      }
    }
    // Accionamiento pH Reductor
    if (Remote_Decrease_pH == true)
    {
      Decrease_pH();
      Publish(0, Topics.pH_Decreased);
      Remote_Decrease_pH = false;
    }
    // Accionamiento pH Incrementador
    if (Remote_Increase_pH == true)
    {
      Increase_pH();
      Publish(0, Topics.pH_Uppered);
      Remote_Increase_pH = false;
    }
    // Accionamiento Increase EC
    if (Remote_Increase_TDS == true)
    {
      Remote_Increase_TDS = false;
      Increase_TDS();
      Publish(0, Topics.Nutrient_Uppered);
    }
    // Accionamiento Decrease EC
    if (Remote_Water_UP == true)
    {
      Remote_Water_UP = false;
      Decrease_TDS();
      Publish(0, Topics.Water_Uppered);
    }
    // PID Control
    PID_Control();
  }
  client.loop();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void regulate_pH()
{

  // pH>rango max
  if (pH > MAX_pH)
  {
    Decrease_pH();
  }

  // pH<rango minimo
  if (pH < MIN_pH)
  {
    Increase_pH();
  }
}
void regulate_Solution()
{
  // TDS>rango max
  if (ECValue > MAX_EC)
  {
    Decrease_TDS();
  }

  // TDS<rango minimo
  if (ECValue < MIN_EC)
  {
    Increase_TDS();
  }
}

void Increase_pH()
{
  Electrovalvulas.OpenRelay(2);
  delay(Time_pH_Up);
  Electrovalvulas.CloseRelay(2);
  Publish(1, Topics.pH_Uppered);
  Serial.print("Current pH value: ");
  Serial.println(pH);
  delay(2000);
}
void Decrease_pH()
{
  Electrovalvulas.OpenRelay(3);
  delay(Time_pH_Decrease);
  Electrovalvulas.CloseRelay(3);
  Publish(1, Topics.pH_Decreased);
  Serial.print("Current pH value: ");
  Serial.println(pH);
  delay(2000);
}
void Increase_TDS()
{
  Electrovalvulas.OpenRelay(5);
  Electrovalvulas.OpenRelay(6);
  delay(Time_Nutrient);
  Electrovalvulas.CloseRelay(5);
  Electrovalvulas.CloseRelay(6);
  Publish(1, Topics.Nutrient_Uppered);
  Serial.print("Current TDS value: ");
  Serial.println(ECValue);
  delay(2000);
}
void Decrease_TDS()
{
  Electrovalvulas.OpenRelay(4);
  delay(Time_Water);
  Electrovalvulas.CloseRelay(4);
  Publish(1, Topics.Water_Uppered);
  Serial.print("Current TDS value: ");
  Serial.println(ECValue);
  delay(2000);
}

void callback(char *topic, byte *payload, unsigned int length)
{

  String response = "";
  for (int i = 0; i < length; i++)
  {
    response = ((char)payload[i]);
  }
  if (String(topic) == "Hydroponic/Update_petition")
  {
    if (response == "1")
    {
      Update_data_flag = true;
    }
    else
    {
      Update_data_flag = false;
    }
  }
  if (String(topic) == "Hydroponic/Water_pump_fdbk")
  {
    if (response == "1")
    {
      Update_Pump_state = true;
    }
    else
    {
      Update_Pump_state = false;
    }
  }

  if (String(topic) == "Hydroponic/pH_Up")
  {
    if (response == "1")
    {
      Remote_Increase_pH = true;
    }
    else
    {
      Remote_Increase_pH = false;
    }
  }
  if (String(topic) == "Hydroponic/pH_Decrease")
  {
    if (response == "1")
    {
      Remote_Decrease_pH = true;
    }
    else
    {
      Remote_Decrease_pH = false;
    }
  }

  if (String(topic) == "Hydroponic/Nutrient_Up")
  {
    if (response == "1")
    {
      Remote_Increase_TDS = true;
    }
    else
    {
      Remote_Increase_TDS = false;
    }
  }

  if (String(topic) == "Hydroponic/Water_Up")
  {
    if (response == "1")
    {
      Remote_Water_UP = true;
      Decrease_TDS();
      Publish(0, Topics.Water_Uppered);
    }
    else
    {
      Remote_Water_UP = false;
    }
  }
  if (String(topic) == "Hydroponic/Heater_control")
  {
    Response_string = response;
    // Setting the new Setpoint
    Water_temp_Setpoint = float(Response_string.toInt());
  }

  if (String(topic) == "Hydroponic/SetProcess_Configuration")
    if (response == "1")
    {
    }
    else
    {
    }
}

void setup_wifi()
{

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
  Serial.println(WiFi.localIP());
}

// Reconnect to client

void reconnect()
{

  client.setServer(IP, Port);
  client.setCallback(callback);

  // Loop until we're reconnected
  while (!client.connected())
  {

    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32_2"))
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
  }
}

void Publish(float message, const char *topic)
{

  char message_to_upload[7];

  // We pass from a float number into a const char
  dtostrf(message, 6, 1, message_to_upload);
  client.publish(topic, message_to_upload);
}

void Update_all_data()
{
  // Checking regulator tanks

  Low_Nutrient_Tank_1 = digitalRead(Nutrient1_Elevator_PIN);
  Low_Nutrient_Tank_2 = digitalRead(Nutrient2_Elevator_PIN);
  Low_pH_Elevator_Tank = digitalRead(pH_Elevator_PIN);
  Low_pH_Reductor_Tank = digitalRead(pH_Reductor_PIN);

  // Nutrient Level tank
  Tank_level = Height_tank - (sonar.ping() / US_ROUNDTRIP_CM);

  // 100cm -> 0%
  // 1cm -> 1%  -> 10L  aprox
  // 10cm -> 10% -> 100L aprox

  // Water Temperature
  WaterTempSensor.requestTemperatures();
  Water_temperature = WaterTempSensor.getTempCByIndex(0);
  delay(50);

  // EC Value
  TdsSensor.setTemperature(Water_temperature);
  TdsSensor.update();
  // tdsValue = TdsSensor.getTdsValue();
  ECValue = TdsSensor.getEcValue();

  // pH Value
  pH = TdsSensor.getpH();
  delay(50);

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
  delay(1000);
}

void PID_Control()
{
  pidController.Input = analogRead(Water_temperature);
  pidController.Update();
  ThermalResistor.Run(pidController.Output);
  Serial.print("PID output:");
  Serial.println(pidController.Output);
}
