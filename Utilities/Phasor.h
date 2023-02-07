/*

    Phasor
    January 2013
    Liam Madgwick

*/

#pragma once
#include "ooshDSP.h"

class Phasor
{
    public:
    Phasor()  {};
    ~Phasor()  {};
    inline void Init(float sampleRate) 
    {
        FS = sampleRate;
        ms = FS * 0.001f;
    }
    void setFreq(float freq)
    {
        _freq = freq;
        inc = (TWOPI_F * _freq) / FS;
    }
    float Process()
    {
        float x = phase / TWOPI_F;
        phase += inc;
        if (phase > TWOPI_F)
        {
            phase -= TWOPI_F;
        }
        if (phase < 0.0f)
        {
            phase = 0.0f;
        }
        return x;
    }
    private:
    float FS;
    float ms;
    float _freq;
    float inc, phase;
};