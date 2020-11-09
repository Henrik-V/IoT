#ifndef PIC_h
#define PIC_h

#include "Arduino.h"

class PIC
{
    public:
        PIC(float Kp, float Ti, float dt);
        float ControlSignal(float setPoint, float temperature);
        void SetParams(float Kp, float Ti);
    private:
        float _Kp;
        float _Ti;
        float _dt;
        int _setPointPin;
        int _controlPin;
        float _u_k1;
        float _e_k1;
        float _u_max;
        float _u_min;
};

#endif