#include "Arduino.h"
#include "LowPassFilter.h"

LowPassFilter::LowPassFilter(float Ts, float Tf)
{
    _Ts = Ts;
    _Tf =Tf;
    _firstPass = true;
    _out_k1 = 0;
}

float LowPassFilter::FilteredValue(float unfiltered)
{
    float a = _Ts/(_Ts+_Tf);
    if(_firstPass)
    {
        _firstPass = false;
        _out_k1 = unfiltered;
        return unfiltered;
    }
    else
    {
        _out_k1 = (1-a)*_out_k1 + a*unfiltered;
        return _out_k1;
    }
}