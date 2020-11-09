#ifndef LowPassFilter_h
#define LowPassFilter_h

#include <Arduino.h>

class LowPassFilter
{
    public:
        LowPassFilter(float Ts, float Tf);
        float FilteredValue(float unfiltered);
    private:
    float _Ts;
    float _Tf;
    float _out_k1;
    bool _firstPass;

};

#endif