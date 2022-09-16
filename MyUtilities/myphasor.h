/*
  ==============================================================================

    myphasor.h
    Created: 15 9 2022
    Author:  Liam Madgwick

  ==============================================================================
*/

#pragma once
#include "DSP2.h"

class myphasor
{
    public:
    myphasor()  {};
    ~myphasor()  {};
    inline void Init(float sampleRate) 
    {
        FS = sampleRate;
        ms = FS * 0.001f;
    }
    void setFreq(float freq_)
    {
        freq = freq_;
        inc = (TWOPI_F * freq) / FS;
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
    float freq;
    float inc, phase;
};