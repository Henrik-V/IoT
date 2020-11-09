#include "Arduino.h"
#include "Scaler.h"

//Constructor
Scaler::Scaler()
{
    _resolution = 1024.0;
    _maxVolt = 5.0;
    _minVolt = 1;
    _minTemp = 20;
    _maxTemp = 50;
    
}

float Scaler::ScaleAnalogInput(int analogInput)
{
    return (5.0/1024)*analogInput;
}

float Scaler::VoltToCelsius(float voltage){
    return ((50.0-20.0)/(5.0-1.0))*(voltage-1.0)+20.0;
}