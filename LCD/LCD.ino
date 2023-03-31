#include <LiquidCrystal_I2C.h>
#include "AiEsp32RotaryEncoder.h"
#include <string.h>

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;
#define ROTARY_ENCODER_A_PIN 25
#define ROTARY_ENCODER_B_PIN 26
#define ROTARY_ENCODER_BUTTON_PIN 27
#define ROTARY_ENCODER_VCC_PIN -1 /*put -1 of Rotary encoder Vcc is connected directly to 3,3V; else you can use declared output pin for powering rotary encoder antes 27 */

#define DIRLCD 0x27  // LCD address
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN);
LiquidCrystal_I2C lcd(DIRLCD, lcdColumns, lcdRows);

int16_t leeEncoder();
int8_t deltaEncoder();
void inicializaLcd();
void incializaRotaryEncoder();
void escribeLcd(String mensaje1, String mensaje2);
bool botonEncoderPulsado();
int16_t valEncoderAnt = 0;
int16_t valEncoder = 0;
int16_t current_pos = 0;
int8_t delta_pos = 0;
int low_limit = 0;
int16_t upper_limit = 0;
bool menu_changed = false;
int16_t menu_pos = 0;   // es el indice de cada menu
int16_t menus_pos = 0;  // indica en que menu de todos estamos

String menu[] = { "OVERVIEW", "MANUAL", "AUTOMATIC", "SETTINGS", "" };
String menu_OVERVIEW[] = { "BACK", "PH: " + String(10), "EC: ", "WATER TEMP: ", "WEATHER TEMP: ", "HUMIDITY:", "" };
String menu_MANUAL[] = { "BACK", "PUMP_ON", "HEATER_ON", "pH UP", "pH DOWN", "NUTRIENT UP", "WATER UP", "" };
String menu_AUTOMATIC[] = { "BACK", "DAY", "WEEK", "START PROCESS", "" };
String menu_SETTINGS[] = { "BACK", "MAX_pH: ", "MIN_pH: ", "MAX_EC: ", "MIN_EC: ", "TEMP SETPOINT: ", "CYCLE TIME: ", "" };
int limites[] = { 3, 5, 6, 3, 6, 5 };
// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch


// Creates  custom character for the menu display
byte menuCursor[8] = {
  B01000,  //  *
  B00100,  //   *
  B00010,  //    *
  B00001,  //     *
  B00010,  //    *
  B00100,  //   *
  B01000,  //  *
  B00000   //
};
byte UP_ARROW[8] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100
};
byte DOWN_ARROW[8] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100
};


// Incialización encoder rotativo HW-040

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  inicializaLcd();
  incializaRotaryEncoder();
  escribeLcd("HYDROPONIC", "INITIATING");
  delay(2000);
  lcd.clear();
}

void loop() {
  if (deltaEncoder() != 0) {
    lcd.clear();
  }
  menu_pos = leeEncoder();
  upper_limit = limites[menus_pos];
  if (menu_pos > upper_limit) {
    rotaryEncoder.reset(upper_limit);
  }
  if (menu_pos < low_limit) {
    rotaryEncoder.reset(low_limit);
  }
  if (menu_pos <= upper_limit) {
    switch (menus_pos) {
      case 0:  // OVERVIEW
        escribeLcd(menu[menu_pos], menu[menu_pos + 1]);

        if (botonEncoderPulsado() == true) {
          switch (menu_pos) {
            case 0:  // OVERVIEW
              menus_pos = 1;
              menu_pos = 0;
              lcd.clear();
              break;
            case 1:  // MANUAL
              menus_pos = 2;
              menu_pos = 0;
              lcd.clear();
              break;
            case 2:  //AUTOMATIC
              menus_pos = 3;
              menu_pos = 0;
              lcd.clear();
              break;
            case 3:  //SETTINGS
              menus_pos = 4;
              menu_pos = 0;
              lcd.clear();
              break;
          };
        }
        break;

      case 1:  // OVERVIEW
        escribeLcd(menu_OVERVIEW[menu_pos], menu_OVERVIEW[menu_pos + 1]);
        if (botonEncoderPulsado() == true && menu_pos == 0) {
          menu_pos = menus_pos;  // BACK
          menus_pos = 0;
          lcd.clear();
        }

        break;

      case 2:  // MANUAL
        escribeLcd(menu_MANUAL[menu_pos], menu_MANUAL[menu_pos + 1]);
        if (botonEncoderPulsado() == true) {
          switch (menu_pos) {
            case 0:  // BACK
              menu_pos = menus_pos;
              menus_pos = 0;
              lcd.clear();
              break;
            case 1:  // PUMP ON

              break;
            case 2:  // HEATER ON

              break;
            case 3:  // pH UP

              break;
            case 4:  // pH DOWN

              break;
            case 5:  // NUTRIENT UP

              break;
            case 6:  // WATER UP

              break;
          };
        }
        break;

      case 3:  //AUTOMATIC
        escribeLcd(menu_AUTOMATIC[menu_pos], menu_AUTOMATIC[menu_pos + 1]);
        if (botonEncoderPulsado() == true) {
          switch (menu_pos) {
            case 0:  // BACK
              menu_pos = menus_pos;
              menus_pos = 0;
              lcd.clear();
              break;
            case 1:  // SET DAY OF PROCESS DEFAULT:0
              setValue(menu_AUTOMATIC[menu_pos], 0, 0.1, 0, 10);
              break;
            case 2:  // SET WEEK OF PROCESS DEFAULT:0
              setValue(menu_AUTOMATIC[menu_pos], 0, 0.1, 0, 10);
              break;
              case 3:  // START PROCESS
              break;
          };
        }
        break;
      case 4:  //SETTINGS
        escribeLcd(menu_SETTINGS[menu_pos], menu_SETTINGS[menu_pos + 1]);
        if (botonEncoderPulsado() == true) {
          switch (menu_pos) {
            case 0:  // BACK
              menu_pos = menus_pos;
              menus_pos = 0;
              lcd.clear();
              break;
            case 1:  // "MAX_pH:
              setValue(menu_SETTINGS[menu_pos], 0, 0.1, 0, 10);
              break;
            case 2:  // MIN_pH
              setValue(menu_SETTINGS[menu_pos], 0, 0.1, 0, 10);
              break;
            case 3:  // MIN_EC
              setValue(menu_SETTINGS[menu_pos], 0, 0.1, 0, 10);
              break;
            case 4:  // MAX_EC
              setValue(menu_SETTINGS[menu_pos], 0, 0.1, 0, 10);
              break;
            case 5:  // TEMP SETPOINT
              setValue(menu_SETTINGS[menu_pos], 9, 0.1, 0, 10);
              break;
            case 6:  // CYCLE TIME
              setValue(menu_SETTINGS[menu_pos], 0, 0.1, 0, 10);
              break;
            case 7:  // WATER UP
              break;
          };
        }
        break;
    };
  }
}
























int16_t leeEncoder() {
  return (rotaryEncoder.readEncoder());
}

int8_t deltaEncoder() {
  int8_t encoderDelta = rotaryEncoder.encoderChanged();
  return encoderDelta;
}

void inicializaLcd() {
  lcd.init();       // initialize LCD
  lcd.backlight();  // turn on LCD backlight
                    // Creates the byte for the 3 custom characters
  lcd.createChar(0, menuCursor);
  lcd.createChar(1, UP_ARROW);
  lcd.createChar(2, DOWN_ARROW);
  lcd.clear();
}

void incializaRotaryEncoder() {
  rotaryEncoder.begin();
  rotaryEncoder.setup([] {
    rotaryEncoder.readEncoder_ISR();
  });
  // optionally we can set boundaries and if values should cycle or not
  rotaryEncoder.setBoundaries(-10000, 10000, true);  // minValue, maxValue, cycle values (when max go to min and vice versa)
}

void escribeLcd(String mensaje1, String mensaje2) {
  lcd.setCursor(0, 0);
  lcd.write(0);
  lcd.setCursor(1, 0);
  lcd.print(mensaje1);
  lcd.setCursor(1, 1);
  lcd.print(mensaje2);
}

bool botonEncoderPulsado() {
  if (rotaryEncoder.currentButtonState() == BUT_RELEASED)
    return true;
  else
    return false;
}
float setValue(String Topic, float Value, float inc, float min, float max) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.setCursor(1, 0);
  lcd.print(Topic);
  lcd.setCursor(1, 1);
  lcd.print(Value);
  valEncoderAnt = rotaryEncoder.readEncoder();
  while (rotaryEncoder.currentButtonState() != BUT_RELEASED) {
    valEncoder = rotaryEncoder.readEncoder();
    if (valEncoder > valEncoderAnt) {
      Value = Value + inc;
      Value = constrain(Value, min, max);
      valEncoderAnt = valEncoder;
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print(Topic);
      lcd.setCursor(0, 1);
      lcd.print(Value);
    } else if (valEncoder < valEncoderAnt) {
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
