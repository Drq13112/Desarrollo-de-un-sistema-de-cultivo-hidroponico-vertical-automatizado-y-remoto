#include "Libraries.h"

Relays::Relays() {}

void Relays::SetUp()
{
  delay(100);
  pinMode(SER_Pin, OUTPUT);
  delay(50);
  pinMode(RCLK_Pin, OUTPUT);
  delay(50);
  pinMode(SRCLK_Pin, OUTPUT);
  delay(500);
  clearRegisters();
  delay(50);
  writeRegisters();
  delay(500);
}

void Relays::OpenRelay(int relayId)
{
  // Opening certain relay
  registers[relayId] = LOW;
  writeRegisters();
}

void Relays::CloseRelay(int relayId)
{
  // Closing certain relay
  registers[relayId] = HIGH;
  writeRegisters();
}
void Relays::Reset()
{
  Serial.println(F("Reset all relays"));
  clearRegisters();
  writeRegisters();
}

void Relays::clearRegisters()
{ /* function clearRegisters */
  //// Clear registers variables
  for (int i = numOfRegisterPins - 1; i >= 0; i--)
  {
    registers[i] = HIGH;
  }
  printRegisters();
}
void Relays::writeRegisters()
{ /* function writeRegisters */
  //// Write register after being set
  digitalWrite(RCLK_Pin, LOW);
  for (int i = numOfRegisterPins - 1; i >= 0; i--)
  {
    digitalWrite(SRCLK_Pin, LOW);
    digitalWrite(SER_Pin, registers[i]);
    digitalWrite(SRCLK_Pin, HIGH);
  }
  digitalWrite(RCLK_Pin, HIGH);
}
void Relays::printRegisters()
{ /* function clearRegisters */
  //// Clear registers variables
  for (int i = 0; i < numOfRegisterPins; i++)
  {
    Serial.print(registers[i]);
    Serial.print(F(" ,"));
  }
  Serial.println();
}
