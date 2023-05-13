#ifndef Relays_h
#define Relays_h

// 74HC595 Shift Register module

class Relays
{

private:
    // Variables:

    // ST_CP pin 12
    const int SER_Pin = 32;
    // SH_CP pin 11
    const int RCLK_Pin = 12;
    // DS pin 14
    const int SRCLK_Pin = 13;
    int numOfRegisterPins = 8;
    bool registers[8];

    // Private methods

    void printRegisters();
    void writeRegisters();
    void clearRegisters();

public:
    Relays();
    void SetUp();
    void OpenRelay(int);
    void CloseRelay(int);
    void Reset();
};
#endif