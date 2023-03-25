#ifndef GRAVITY_TDS_H
#define GRAVITY_TDS_H

#define ReceivedBufferLength 15
#define TdsFactor 0.5  // tds = ec / 2


class GravityTDS
{
public:
    GravityTDS();
    ~GravityTDS();

    void begin();  //initialization
    void update(); //read and calculate
    void setPin(int pin); 
    void setTemperature(float temp);  //set the temperature and execute temperature compensation
    void setAref(float value);  //reference voltage on ADC, default 5.0V on Arduino UNO
    void setAdcRange(float range);  //1024 for 10bit ADC;4096 for 12bit ADC
    void setKvalueAddress(int address); //set the EEPROM address to store the k value,default address:0x08
    float getKvalue(); 
    float getTdsValue();
    float getEcValue();
    float getpH();

private:
    int pin;
    float aref;  // default 5.0V on Arduino UNO
    float adcRange;
    float temperature;
    int kValueAddress;     //the address of the K value stored in the EEPROM
    char cmdReceivedBuffer[ReceivedBufferLength+1];   // store the serial cmd from the serial monitor
    byte cmdReceivedBufferIndex;
 
    float kValue;      // k value of the probe,you can calibrate in buffer solution ,such as 706.5ppm(1413us/cm)@25^C 
    float analogValueTDS;
    float analogValuepH;
    float ecValue; //before temperature compensation
    float ecValue25; //after temperature compensation
    float tdsValue;
    float pHValue;
    float Coef1=-11;
    float coef2 =51;
    float buffer[10];

    void readKValues();
    float getAverage(int);
};  

#endif
