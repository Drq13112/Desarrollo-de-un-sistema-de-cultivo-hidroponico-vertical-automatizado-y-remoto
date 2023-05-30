#ifndef Ultrasonic_h
#define Ultrasonic_h

// Inspired in DroneBot Workshop 2021
// https://dronebotworkshop.com
class Ultrasonic
{
    public:
    Ultrasonic(int, int);
    void SetUp();
    float getDistance();

    private:
    // pins
    int TRIGPIN;
    int ECHOPIN;
    // Variables to calculate distance:
    float duration;
    float distance;
    float average = 10;
};
#endif