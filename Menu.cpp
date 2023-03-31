
#include "Libraries.h"
/*
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "AiEsp32RotaryEncoder.h"
#include "Menu.h"*/

extern int consigna_indice;
extern double consigna_valor[];

extern int objeto_control_indice;
extern String objeto_control_nombre[];
extern double objeto_control_vel_max[];
extern double objeto_control_vel_min[];
extern double objeto_control_vel_paso[];
extern double objeto_control_pos_max[];
extern double objeto_control_pos_min[];
extern double objeto_control_pos_paso[];

extern bool Process_ON;
extern bool realizado;
extern int contador;

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(DIRLCD, lcdColumns, lcdRows);

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

// Incialización encoder rotativo HW-040

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN);

int16_t leeEncoder()
{
  return (rotaryEncoder.readEncoder());
}

int8_t deltaEncoder()
{
  int8_t encoderDelta = rotaryEncoder.encoderChanged();
  return encoderDelta;
}

void inicializaLcd()
{
  lcd.init();      // initialize LCD
  lcd.backlight(); // turn on LCD backlight
                   // Creates the byte for the 3 custom characters
  lcd.createChar(0, menuCursor);
  // lcd.createChar(1, upArrow);
  // lcd.createChar(2, downArrow);
}

void incializaRotaryEncoder()
{
  rotaryEncoder.begin();
  rotaryEncoder.setup([]
                      { rotaryEncoder.readEncoder_ISR(); });
  // optionally we can set boundaries and if values should cycle or not
  rotaryEncoder.setBoundaries(-10000, 10000, true); // minValue, maxValue, cycle values (when max go to min and vice versa)
}

void escribeLcd(String mensaje1, String mensaje2)
{
  static int k = 0;

  lcd.setCursor(0, 0);
  if (mensaje1.length() <= lcdColumns)
    lcd.print(mensaje1);
  else
  {
    if (mensaje1.length() > lcdColumns + k)
    {
      k++;
    }
    else
      k = 0;
    lcd.print(mensaje1.substring(0 + k, lcdColumns + k));
    if (k + lcdColumns > mensaje1.length())
      lcd.print(" " + mensaje1.substring(0, k + lcdColumns - mensaje1.length()));
  }

  lcd.setCursor(0, 1);
  if (mensaje2.length() <= lcdColumns)
    lcd.print(mensaje2);
  else
  {
    if (mensaje2.length() > lcdColumns + k)
    {
      k++;
    }
    else
      k = 0;
    lcd.print(mensaje2.substring(0 + k, lcdColumns + k));
    if (k + lcdColumns > mensaje2.length())
      lcd.print(" " + mensaje2.substring(0, k + lcdColumns - mensaje2.length()));
  }
}

bool botonEncoderPulsado()
{
  if (rotaryEncoder.currentButtonState() == BUT_RELEASED)
    return true;
  else
    return false;
}

void muestraMenu(String menu[], int maxMenuItems, String opDefecto[], int opcionMenu)
{
  int numPags, pag;
  String linea;
  // int maxTam=14;
  static int k = 0;
  int menuMaxLineas = 2;

  numPags = round((float)maxMenuItems / menuMaxLineas + 0.4999);
  pag = (opcionMenu - 1) / menuMaxLineas;
  lcd.setCursor(0, 0);
  lcd.clear();
  // Serial.println("NumPags: "+String(numPags));
  // Serial.println("Pag: "+String(pag));
  // Serial.println("opcionMenu: "+String(opcionMenu));
  if (pag == 0)
  { // Primera página
    for (int i = 0; i < (maxMenuItems < menuMaxLineas ? maxMenuItems : menuMaxLineas); i++)
    {
      lcd.setCursor(0, i);
      linea = menu[i] + " " + opDefecto[i];
      if (opcionMenu == i + 1)
      {
        lcd.write(byte(0));

        if (linea.length() <= lcdColumns - 1)
          lcd.print(linea);
        else
        {
          if (linea.length() > lcdColumns - 1 - k)
          {
            k++;
          }
          else
            k = 0;
          lcd.print(linea.substring(0 + k, lcdColumns - 1 + k));
          if (k + lcdColumns - 1 > linea.length())
            lcd.print(" " + linea.substring(0, k + lcdColumns - 1 - linea.length() - 1));
        }
      }
      else
      {
        lcd.print(" ");
        if (linea.length() <= lcdColumns - 1)
          lcd.print(linea);
        else
          lcd.print(linea.substring(0, lcdColumns - 1));
      }
    }
  }
  else if (pag + 1 == numPags)
  { // Ultima Pagina
    for (int i = 0; i < menuMaxLineas; i++)
    {
      lcd.setCursor(0, i);
      linea = menu[maxMenuItems - menuMaxLineas + i] + " " + opDefecto[maxMenuItems - menuMaxLineas + i];
      if (opcionMenu == maxMenuItems - menuMaxLineas + i + 1)
      {
        lcd.write(byte(0));

        if (linea.length() <= lcdColumns - 1)
          lcd.print(linea);
        else
        {
          if (linea.length() > lcdColumns - 1 - k)
            k++;
          else
            k = 0;
          lcd.print(linea.substring(0 + k, lcdColumns - 1 - k));
          if (k + lcdColumns - 1 > linea.length())
            lcd.print(" " + linea.substring(0, k + lcdColumns - 1 - linea.length() - 1));
        }
      }
      else
      {
        lcd.print(" ");
        ;
        if (linea.length() <= lcdColumns - 1)
          lcd.print(linea);
        else
          lcd.print(linea.substring(0, lcdColumns - 1));
      }
    }
  }
  else
  {
    for (int i = 0; i < menuMaxLineas; i++)
    {
      lcd.setCursor(0, i);
      linea = menu[pag * menuMaxLineas + i] + " " + opDefecto[pag * menuMaxLineas + i];
      if (opcionMenu == pag * menuMaxLineas + i + 1)
      {
        lcd.write(byte(0));
        if (linea.length() <= lcdColumns - 1)
          lcd.print(linea);
        else
        {
          if (linea.length() > lcdColumns - 1 + k)
            k++;
          else
            k = 0;
          lcd.print(linea.substring(k, lcdColumns - 1 - k));
          if (k + lcdColumns - 1 > linea.length())
            lcd.print(" " + linea.substring(0, k + lcdColumns - 1 - linea.length() - 1));
        }
      }
      else
      {
        lcd.print(" ");
        if (linea.length() <= lcdColumns - 1)
          lcd.print(linea);
        else
          lcd.print(linea.substring(0, lcdColumns - 1));
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////*******dameValor ******///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

double dameValor(String cadena, double valor, double inc, double min, double max)
{
  int valEncoder, valEncoderAnt;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(cadena + String(":"));
  lcd.setCursor(0, 1);
  lcd.print(valor);
  valEncoderAnt = rotaryEncoder.readEncoder();
  while (rotaryEncoder.currentButtonState() != BUT_RELEASED)
  {
    valEncoder = rotaryEncoder.readEncoder();
    if (valEncoder > valEncoderAnt)
    {
      valor = valor + inc;
      valor = constrain(valor, min, max);
      valEncoderAnt = valEncoder;
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print(cadena + String(":"));
      lcd.setCursor(0, 1);
      lcd.print(valor);
    }
    else if (valEncoder < valEncoderAnt)
    {
      valor = valor - inc;
      valor = constrain(valor, min, max);
      valEncoderAnt = valEncoder;
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print(cadena + String(":"));
      lcd.setCursor(0, 1);
      lcd.print(valor);
    }
  }
  delay(100);
  return valor;
}
<<<<<<< HEAD
void MainMenu()
{
  // Inicializamos las opciones de menu
  String menu[] = {"OVERVIEW", "MANUAL", "AUTOMATICO"};
  String opDefecto[3];
  int index = 0;
=======


void FuncionMenuPrincipal(){
  //Inicializamos las opciones de menu
  String menu[] = {"BACK", "OVERVIEW","MOOD","CONFIGUTARION"};

  
  String opDefecto[5];
  int index=0;
>>>>>>> d942c004b9b55ebde5c6986ff0de81a1605f7300
  double temp = consigna_valor[0];
  do
  {

    index = miMenu(menu, 3, opDefecto, index); // si se está en manual pero no seleccionado el eyector
  } while (1);
}

int miMenu(String menu[], int maxMenuItems, String opDefecto[], int nMenuOpDef)
{
  int valEncoder, valEncoderAnt;
  int opcionMenu = nMenuOpDef + 1;
  static long int tiempo = millis();

  valEncoderAnt = rotaryEncoder.readEncoder();
  muestraMenu(menu, maxMenuItems, opDefecto, opcionMenu);
  while (rotaryEncoder.currentButtonState() != BUT_RELEASED)
  {
    valEncoder = rotaryEncoder.readEncoder();
    if (valEncoder > valEncoderAnt)
    {
      opcionMenu++;
      opcionMenu = constrain(opcionMenu, 1, maxMenuItems);
      muestraMenu(menu, maxMenuItems, opDefecto, opcionMenu);
      valEncoderAnt = valEncoder;
    }
    else if (valEncoder < valEncoderAnt)
    {
      opcionMenu--;
      opcionMenu = constrain(opcionMenu, 1, maxMenuItems);
      muestraMenu(menu, maxMenuItems, opDefecto, opcionMenu);
      valEncoderAnt = valEncoder;
    }
    else
    { // Entra cada cierto tiempo
      if (millis() - tiempo > 400)
      {
        if (menu[opcionMenu - 1].length() + opDefecto[opcionMenu - 1].length() >= lcdColumns - 1) // Solo se llama a la funcion si el texto es largo
          muestraMenu(menu, maxMenuItems, opDefecto, opcionMenu);
        tiempo = millis();
      }
    }
  }
  lcd.clear();
  delay(100);
  return opcionMenu - 1;
}
