#include <LiquidCrystal_I2C.h>
#include "AiEsp32RotaryEncoder.h"
#include "Menu.h"
#include <string.h>

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;
float temp = 0;
String message1 = "";
String message2 = "";

#define ROTARY_ENCODER_A_PIN 25
#define ROTARY_ENCODER_B_PIN 26
#define ROTARY_ENCODER_BUTTON_PIN 27
#define ROTARY_ENCODER_VCC_PIN -1 // put -1 of Rotary encoder Vcc is connected directly to 3,3V; else you can use declared output pin for powering rotary encoder antes 27
#define DIRLCD 0x27               // LCD address
#define ROTARY_ENCODER_STEPS 2

// Variables

extern float MAX_pH;
extern float MIN_pH;
extern float MAX_EC;
extern float MIN_EC;
extern float Cycle_time;
extern float Water_temp_Setpoint;
extern float Kp;
extern float Ki;
extern float Kd;
extern float temperature;
extern float Water_temperature;
extern float pH;
extern float humidity;
extern float Tank_level;
extern float ECValue;
extern float Water_flow;
extern float Tank_level;
extern bool Update_Pump_state;
extern bool Update_data_flag;
extern float Low_Nutrient_Tank_General;
extern float Low_pH_Elevator_Tank;
extern float Low_pH_Reductor_Tank;
extern void Increase_pH();
extern void Decrease_pH();
extern void Increase_TDS();
extern void Decrease_TDS();
extern bool Process_ON;
extern void Reset_ESP();
extern void SaveSettings();
extern void Reset_Settings();
extern void Update_all_data();

const char *Water_Pump_m = "Hydroponic/Water_Pump";
const char *MAX_EC_m = "Hydroponic/MAX_EC";
const char *MIN_EC_m = "Hydroponic/MIN_EC";
const char *MAX_pH_m = "Hydroponic/MAX_pH";
const char *MIN_pH_m = "Hydroponic/MIN_pH";
const char *Temp_SP_m = "Hydroponic/Temp_SP";
const char *Process_ON_m = "Hydroponic/Process_ON";
const char *Cycle_Time_m = "Hydroponic/Cycle_Time";
const char *Kp_m = "Hydroponic/Kp";
const char *Kd_m = "Hydroponic/Kd";
const char *Ki_m = "Hydroponic/Ki";

extern void Publish(float message, const char *topic);
// Menu Layout

String menu[] = {"OVERVIEW", "MANUAL", "AUTOMATIC", "SETTINGS", ""};
String menu_OVERVIEW[] = {"BACK                         ", "UPDATE DATA            ", "PH:", "EC:", "WTR TEMP:", "WTHR TEMP:", "HUMIDITY:", "TANK:", "PH UP:", "PH DOWN:", "EC UP:", "WATERFLOW:", "                                                        "};
String menu_MANUAL[] = {"BACK", "PUMP_ON", "pH UP", "pH DOWN", "NUTRIENT UP", "WATER UP", ""};
String menu_AUTOMATIC[] = {"BACK", "START PROCESS", "STOP PROCESS", ""};
String menu_SETTINGS[] = {"BACK                         ", "MAX_pH:", "MIN_pH:", "MAX_EC:", "MIN_EC:", "TEMP SP:", "CYCLE:", "Kp:", "Kd:", "Ti:", "SAVE SETTINGS            ", "RESET SETTINGS        ", "RESET ESP32                                         ", "                                            "};
int limites[] = {3, 10, 5, 2, 12};
float Values1[] = {0, 0, pH, ECValue, Water_temperature, temperature, humidity, Tank_level, Low_pH_Elevator_Tank, Low_pH_Reductor_Tank, Low_Nutrient_Tank_General, Water_flow, 0};
float Values2[] = {0, MAX_pH, MAX_pH, MAX_EC, MIN_EC, Water_temp_Setpoint, Cycle_time, Kp, Kd, Ki, 0, 0, 0, 0};

// Creates  custom character for the menu display
byte menuCursor[8] = {
    B01000, //  *
    B00100, //   *
    B00010, //    *
    B00001, //     *
    B00010, //    *
    B00100, //   *
    B01000, //  *
    B00000  //
};

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
LiquidCrystal_I2C lcd(DIRLCD, lcdColumns, lcdRows);

Menu::Menu() {}

void Menu::SetUp()
{
  inicializaLcd();
  incializaRotaryEncoder();
  lcd.clear();
}
void Menu::begin()
{
  lcd.clear();
  escribeLcd("HYDROPONIC", "INITIATING");
}

int16_t Menu::leeEncoder()
{
  return (rotaryEncoder.readEncoder());
}

int8_t Menu::deltaEncoder()
{
  int8_t encoderDelta = rotaryEncoder.encoderChanged();
  return encoderDelta;
}

void Menu::inicializaLcd()
{
  lcd.init();      // initialize LCD
  lcd.backlight(); // turn on LCD backlight
  lcd.createChar(0, menuCursor);
  lcd.clear();
}
void IRAM_ATTR Menu::readE()
{
  rotaryEncoder.readEncoder_ISR();
}

void IRAM_ATTR Menu::readB()
{
  rotaryEncoder.readButton_ISR();
}

void Menu::incializaRotaryEncoder()
{
  rotaryEncoder.begin();
  rotaryEncoder.setup(readE, readB);
  // optionally we can set boundaries and if values should cycle or not
  rotaryEncoder.setBoundaries(-10000, 10000, true); // minValue, maxValue, cycle values (when max go to min and vice versa)
}

void Menu::escribeLcd(String mensaje1, String mensaje2)
{
  lcd.setCursor(0, 0);
  lcd.write(0);
  lcd.setCursor(1, 0);
  lcd.print(mensaje1);
  lcd.setCursor(1, 1);
  lcd.print(mensaje2);
}

bool Menu::botonEncoderPulsado()
{
  if (rotaryEncoder.isEncoderButtonClicked() == true)
    return true;
  else
    return false;
}
float Menu::setValue(String Topic, float Value, float inc, float min, float max)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.setCursor(1, 0);
  lcd.print(Topic);
  lcd.setCursor(1, 1);
  lcd.print(Value);
  valEncoderAnt = rotaryEncoder.readEncoder();
  while (rotaryEncoder.isEncoderButtonClicked() != true)
  {
    valEncoder = rotaryEncoder.readEncoder();
    if (valEncoder > valEncoderAnt)
    {
      Value = Value + inc;
      Value = constrain(Value, min, max);
      valEncoderAnt = valEncoder;
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print(Topic);
      lcd.setCursor(0, 1);
      lcd.print(Value);
    }
    else if (valEncoder < valEncoderAnt)
    {
      Value = Value - inc;
      Value = constrain(Value, min, max);
      valEncoderAnt = valEncoder;
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print(Topic);
      lcd.setCursor(0, 1);
      lcd.print(Value);
    }
  }
  delay(100);
  return Value;
}

void Menu::MainMenu()
{
  if (deltaEncoder() != 0)
  {
    lcd.clear();
  }
  menu_pos = leeEncoder();
  upper_limit = limites[menus_pos];
  if (menu_pos > upper_limit)
  {
    rotaryEncoder.reset(upper_limit);
  }
  if (menu_pos < low_limit)
  {
    rotaryEncoder.reset(low_limit);
  }
  // Serial.print("Menu_pos:");
  // Serial.println(menu_pos);
  if (menu_pos <= upper_limit && menu_pos >= 0)
  {

    switch (menus_pos)
    {
    case 0: // OVERVIEW
      escribeLcd(menu[menu_pos], menu[menu_pos + 1]);
      if (botonEncoderPulsado() == true)
      {
        switch (menu_pos)
        {
        case 0: // OVERVIEW
          menus_pos = 1;
          menu_pos = 0;
          lcd.clear();
          break;
        case 1: // MANUAL
          menus_pos = 2;
          menu_pos = 0;
          lcd.clear();
          break;
        case 2: // AUTOMATIC
          menus_pos = 3;
          menu_pos = 0;
          lcd.clear();
          break;
        case 3: // SETTINGS
          menus_pos = 4;
          menu_pos = 0;
          lcd.clear();
          break;
        };
      }
      break;

    case 1: // OVERVIEW
      UpdateData1();
      message1 = menu_OVERVIEW[menu_pos] + String(Values1[menu_pos]);
      message2 = menu_OVERVIEW[menu_pos + 1] + String(Values1[menu_pos + 1]);
      escribeLcd(message1, message2);
      if (botonEncoderPulsado() == true)
      {
        switch (menu_pos)
        {
        case 0: // BACK
          menu_pos = menus_pos;
          menus_pos = 0;
          lcd.clear();
          break;
        case 1: // UPDATE DATA
          Update_data_flag = true;
          break;
        };
      }

      break;

    case 2: // MANUAL
      escribeLcd(menu_MANUAL[menu_pos], menu_MANUAL[menu_pos + 1]);
      if (botonEncoderPulsado() == true)
      {
        switch (menu_pos)
        {
        case 0: // BACK
          menu_pos = menus_pos;
          menus_pos = 0;
          lcd.clear();
          break;
        case 1: // PUMP ON
          if (Update_Pump_state == false)
          {
            Publish(1, Water_Pump_m);
            Update_Pump_state = true;
          }
          else
          {
            Publish(0, Water_Pump_m);
            Update_Pump_state = false;
          }
          break;
        case 2: // pH UP
          if (Process_ON == false)
            Increase_pH();
          break;
        case 3: // pH DOWN
          if (Process_ON == false)
            Decrease_pH();
          break;
        case 4: // NUTRIENT UP
          if (Process_ON == false)
            Increase_TDS();
          break;
        case 5: // WATER UP
          if (Process_ON == false)
            Decrease_TDS();
          break;
        };
      }
      break;

    case 3: // AUTOMATIC
      escribeLcd(menu_AUTOMATIC[menu_pos], menu_AUTOMATIC[menu_pos + 1]);
      if (botonEncoderPulsado() == true)
      {
        switch (menu_pos)
        {
        case 0: // BACK
          menu_pos = menus_pos;
          menus_pos = 0;
          lcd.clear();
          break;
        case 1: // START PROCESS
          Process_ON = true;
          Publish(1, Process_ON_m);
          break;
        case 2: // STOP PROCESS
          Process_ON = false;
          Publish(0, Process_ON_m);
          break;
        };
      }
      break;
    case 4: // SETTINGS
      UpdateData2();
      message1 = menu_SETTINGS[menu_pos] + String(Values2[menu_pos]);
      message2 = menu_SETTINGS[menu_pos + 1] + String(Values2[menu_pos + 1]);
      escribeLcd(message1, message2);
      if (botonEncoderPulsado() == true)
      {
        switch (menu_pos)
        {
        case 0: // BACK
          menu_pos = menus_pos;
          menus_pos = 0;
          lcd.clear();
          break;
        case 1: // "MAX_pH:
          MAX_pH = setValue(menu_SETTINGS[menu_pos], MAX_pH, 0.1, 0, 14);
          Publish(MAX_pH, MAX_pH_m);
          break;
        case 2: // MIN_pH
          MIN_pH = setValue(menu_SETTINGS[menu_pos], MIN_pH, 0.1, 0, 14);
          Publish(MIN_pH, MIN_pH_m);
          break;
        case 3: // MAX_EC
          MAX_EC = setValue(menu_SETTINGS[menu_pos], MAX_EC, 10, 0, 2400);
          Publish(MAX_EC, MAX_EC_m);
          break;
        case 4: // MIN_EC
          MIN_EC = setValue(menu_SETTINGS[menu_pos], MIN_EC, 10, 0, 2400);
          Publish(MIN_EC, MIN_EC_m);
          break;
        case 5: // TEMP SETPOINT
          Water_temp_Setpoint = setValue(menu_SETTINGS[menu_pos], Water_temp_Setpoint, 0.1, 0, 30);
          Publish(Water_temp_Setpoint, Temp_SP_m);
          break;
        case 6: // CYCLE TIME
          temp = setValue(menu_SETTINGS[menu_pos], Cycle_time, 1, 0, 60);
          Cycle_time = round(temp);
          Publish(Cycle_time, Cycle_Time_m);
          break;
        case 7: // Kp
          Kp = setValue(menu_SETTINGS[menu_pos], Kp, 0.1, 0, 100);
          Publish(Kp, Kp_m);
          break;
        case 8: // Kd
          Kd = setValue(menu_SETTINGS[menu_pos], Kd, 0.1, 0, 100);
          Publish(Kd, Kd_m);
          break;
        case 9: // Ki
          Ki = setValue(menu_SETTINGS[menu_pos], Ki, 0.1, 0, 100);
          Publish(Ki, Ki_m);
          break;
        case 10: // SAVE SETTINGS
          SaveSettings();
          break;
        case 11: // RESET SETTINGS
          Reset_Settings();
          break;
        case 12: // FORMAT
          Reset_ESP();
          break;
        };
      }
      break;
    };
  }
}
void Menu::UpdateData1()
{
  Values1[2] = pH;
  Values1[3] = ECValue;
  Values1[4] = Water_temperature;
  Values1[5] = temperature;
  Values1[6] = humidity;
  Values1[7] = Tank_level;
  Values1[8] = Low_pH_Elevator_Tank;
  Values1[9] = Low_pH_Reductor_Tank;
  Values1[10] = Low_Nutrient_Tank_General;
  Values1[11] = Water_flow;

}
void Menu::UpdateData2()
{
  Values2[1] = MAX_pH;
  Values2[2] = MIN_pH;
  Values2[3] = MAX_EC;
  Values2[4] = MIN_EC;
  Values2[5] = Water_temp_Setpoint;
  Values2[6] = Cycle_time;
  Values2[7] = Kp;
  Values2[8] = Kd;
  Values2[9] = Ki;
}