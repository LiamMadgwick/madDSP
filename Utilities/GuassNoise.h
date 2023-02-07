/*

    GuassNoise
    January 2013
    Liam Madgwick

*/

//taken from musicdsp.org

#pragma once

#include <math.h>
#include "ooshDSP.h"

#define ranf() ((float) rand() / (float) RAND_MAX)

class GuassNoise
{
    public:
    GuassNoise()    {};
    ~GuassNoise()   {};

    //the mean and the spread,
    //spread should be 2.f, and the mean -1.f, 
    //inside the process function looks like this ... (noise output * s) + m, 
    //so spread 2 and mean -1 would give us a signal from -1 to 1
    void Init(float m, float s) {_m = m, _s = s;}

    float Process()
    {
        if(pass)
        {
            y1 = y2;
        }
        else {
            do {
                x1 = 2.f * ranf() - 1.f;
                x2 = 2.f * ranf() - 1.f;
                w = x1 * x1 + x2 * x2;
                }   while(w >= 1.f);
                
                w = (float)sqrt(-2.f * log(w) / w);
                y1 = x1 * w;
                y2 = x2 * w;
        }
        pass = !pass;

        return (y1 * _s + _m);
    }
    private:
    float _m, _s;
    int pass = 0;
    float y2;
    float x1, x2, w, y1;
};