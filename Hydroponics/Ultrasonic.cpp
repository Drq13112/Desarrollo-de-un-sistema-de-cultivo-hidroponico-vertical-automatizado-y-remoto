#include "Libraries.h"

Ultrasonic::Ultrasonic(int TRIGPIN, int ECHOPIN)
{
    this->ECHOPIN = ECHOPIN;
    this->TRIGPIN = TRIGPIN;
}

void Ultrasonic::SetUp()
{
    // Set pinmodes for sensor connections
    pinMode(ECHOPIN, INPUT_PULLUP);
    pinMode(TRIGPIN, OUTPUT);
}
float Ultrasonic::getDistance()
{
    // Set the trigger pin LOW for 2uS
    digitalWrite(TRIGPIN, LOW);
    delayMicroseconds(2);

    // Set the trigger pin HIGH for 20us to send pulse
    digitalWrite(TRIGPIN, HIGH);
    delayMicroseconds(20);

    // Return the trigger pin to LOW
    digitalWrite(TRIGPIN, LOW);

    // Measure the width of the incoming pulse
    duration = pulseIn(ECHOPIN, HIGH);

    // Determine distance from duration
    // Use 343 metres per second as speed of sound
    // Divide by 1000 as we want millimeters

    distance = (duration / 2) * 0.343;

    // Print result to serial monitor
    Serial.print("distance: ");
    Serial.print(distance);
    Serial.println(" mm");

    return distance;
}