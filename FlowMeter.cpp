#include "Libraries.h"

FlowMeter::FlowMeter(uint_8 PIN)
{
    this->PIN = PIN;
}
void IRAM_ATTR FlowMeter::pulseCounter()
{
    pulseCount++;
}
void FlowMeter::setup()
{
    pinMode(PIN, INPUT_PULLUP);
    pulseCount = 0;
    flowRate = 0.0;
    flowMilliLitres = 0;
    totalMilliLitres = 0;
    previousMillis = 0;

    attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);
}

float FlowMeter::Measure()
{
    currentMillis = millis();
    period = currentMillis - previousMillis;
    if(period > interval)
    {

        pulse1Sec = pulseCount;
        pulseCount = 0;

        // Because this loop may not complete in exactly 1 second intervals we calculate
        // the number of milliseconds that have passed since the last execution and use
        // that to scale the output. We also apply the calibrationFactor to scale the output
        // based on the number of pulses per second per units of measure (litres/minute in
        // this case) coming from the sensor.
        flowRate = ((period / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
        previousMillis = millis();

        // Divide the flow rate in litres/minute by 60 to determine how many litres have
        // passed through the sensor in this 1 second interval, then multiply by 1000 to
        // convert to millilitres.
        flowMilliLitres = (flowRate / 60) * 1000;

        // Add the millilitres passed in this second to the cumulative total
        totalMilliLitres += flowMilliLitres;

        // Print the flow rate for this second in litres / minute
        Serial.print("Flow rate: ");
        Serial.print(int(flowRate)); // Print the integer part of the variable
        Serial.print("L/min");
        Serial.print("\t"); // Print tab space

        // Print the cumulative total of litres flowed since starting
        Serial.print("Output Liquid Quantity: ");
        Serial.print(totalMilliLitres);
        Serial.print("mL / ");
        Serial.print(totalMilliLitres / 1000);
        Serial.println("L");
    }

    return flowRate;
}
