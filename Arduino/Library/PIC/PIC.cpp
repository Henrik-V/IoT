#include "Arduino.h"
#include "PIC.h"



//Constructor
PIC::PIC(float Kp, float Ti, float dt){
    _Kp = Kp;
    _Ti = Ti;
    _dt = dt;
    _u_max = 5.0;
    _u_min = 0.0;
    _u_k1 = 0.0;
    _e_k1 = 0.0;
}

float PIC::ControlSignal(float setPoint, float temperature){
       float e = setPoint-temperature;
       float u_k = _Kp*(e-_e_k1);
       float u_i = _Kp*_dt/_Ti*e;
       float u = _u_k1+u_k+u_i;

       if(u > _u_max)
       {
           u = _u_max;
       }
       if(u < _u_min)
       {
           u = _u_min;
       }

       _u_k1 = u;
       _e_k1 = e;

       return u;

}

void PIC::SetParams(float Kp, float Ti){
    _Kp = Kp;
    _Ti = Ti;
}