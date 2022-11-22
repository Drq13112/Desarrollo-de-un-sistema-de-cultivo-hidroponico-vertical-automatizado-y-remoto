#ifndef TDS_h
#define TDS_h

#define VREF 3.3  // analog reference voltage(Volt) of the ADC
#define SCOUNT 30 // sum of sample point

class TDS
{

public:
    // Constructor and destructor
    TDS(int, float);
    ~TDS();

    void setTemperature(float);
    void setPin(int);
    float getMedianNum(int*, int);
    float getTDSValue();
    float getRawValue();

private:
    int analogBuffer[SCOUNT]; // store the analog value in the array, read from ADC
    int analogBufferTemp[SCOUNT];
    int analogBufferIndex = 0;
    int copyIndex = 0;
    int TdsSensorPin =0;
    float averageVoltage = 0;
    float tdsValue = 0;
    float temperature = 25; // current temperature for compensation
};

#endif
