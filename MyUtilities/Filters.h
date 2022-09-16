/*
  ==============================================================================

    Filters.h
    Created: 28 Aug 2022 7:52:44pm
    Author:  Liam Madgwick

  ==============================================================================
*/

#pragma once
#include "DSP2.h"

class OnePole
{
public:
    OnePole() {};
    ~OnePole()    {};
    void Init(float sampleRate) {FS = sampleRate;}
    float Process(float inSample){  y += a0 * (inSample - y); return y;}
    void setFreq(float _freq){float x = exp(-2.0*PI_F*_freq/FS); a0 = 1.0-x; b1 = -x;}
private:
    float a0, b1;
    float y;
    float FS;
};

class RC_Filter
{
public:
    RC_Filter() {};
    ~RC_Filter()    {};
    void Init(float sampleRate) {FS = sampleRate;}
    void setFandQ(float _freq, float Q) {c = 2*PI_F*_freq/FS; r = pow(0.5, (Q + 24)/16.0f);}
    float Process(float inSample)
    {
        v0 =  (1-r*c)*v0  -  (c)*v1  + (c)*inSample;
        v1 =  (1-r*c)*v1  +  (c)*v0;

        return v1;
    }

private:
    float FS;
    float v0, v1;
    float c, r;
};