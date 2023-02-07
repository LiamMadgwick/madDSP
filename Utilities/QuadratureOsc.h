/*

    QuadratureOsc
    January 2013
    Liam Madgwick

*/

//taken from musicdsp.org

#pragma once
#include <stdint.h>
#include "ooshDSP.h"

class QuadratureOsc
{
    public:
    QuadratureOsc() {};
    ~QuadratureOsc()    {};
    void Init(float fs)
    {
        u = 1;
        v = 0;
        FS = fs;
    }
    void setFreq(float freq)
    {
        calc_coeffs(freq);
    }
    //0 for sin... 1 for cosin..
    float Process(int select)
    {
        float tmp = u - (k1*v);
        v = v + k2*tmp;
        u = tmp - k1*v;
        
        if(select == 0)
        {return v;}
        else
        {return u;}
    }
    private:
    inline void calc_coeffs(float w)
    {
        k1 = tan(w * PI_F / FS);
        k2 = 2 * k1 / (1 + k1 * k1);
    }
    float u, v, FS;
    float k1, k2;
};
