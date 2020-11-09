#ifndef Scaler_h
#define Scaler_h

#include "Arduino.h"

class Scaler
{
    public:
        Scaler();
        float ScaleAnalogInput(int analogInput);
        float VoltToCelsius(float voltage);
    private:
        int _resolution;
        int _maxVolt;
        int _minVolt;
        int _maxTemp;
        int _minTemp;
        
};

#endif