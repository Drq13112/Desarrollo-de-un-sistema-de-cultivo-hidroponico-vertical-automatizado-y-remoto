#include "Relays.h"
#include <Arduino.h>

Relays::Relays() {}

void Relays::SetUp()
{
    // Setup pins as Outputs
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
}

void Relays::OpenRelay(int Relay)
{
    // ST_CP LOW to keep LEDs from changing while reading serial data
    digitalWrite(latchPin, LOW);

    // Shift out the bits
    shiftOut(dataPin, clockPin, MSBFIRST, Relay);

    // ST_CP HIGH change LEDs
    digitalWrite(latchPin, HIGH);

    delay(500);
}