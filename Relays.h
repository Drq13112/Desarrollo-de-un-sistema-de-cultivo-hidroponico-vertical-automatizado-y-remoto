#ifndef Relays_h
#define Relays_h

//74HC595 Shift Register module


class Relays
{

private:
    // ST_CP pin 12
    const int latchPin = 10;
    // SH_CP pin 11
    const int clockPin = 11;
    // DS pin 14
    const int dataPin = 12;

public:
    Relays();
    void SetUp();
    void OpenRelay(int);
};
#endif