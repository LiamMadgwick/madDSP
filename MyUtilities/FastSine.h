/*
  ==============================================================================

    FastSine.h
    Author:  Liam Madgwick

  ==============================================================================
*/

#pragma once
#include "DSP2.h"

class FastSine
{
    public:
    FastSine()  {};
    ~FastSine()  {};
    void Init(float sampleRate) { FS = sampleRate; s[0] = 0.5f, s[1] = 0.f;}
    void SetFreq(float _freq) {a = 2.f*(float)sin(PI_F*_freq/FS); }
    //select 0 for sine, 1 for cosine
    float Process(int select) 
    {
        s[0] = s[0] - a*s[1]; 
        s[1] = s[1] + a*s[0]; 
        if(select == 0){ return s[0];} //output_sine;
        else if(select == 1){ return s[1];} //output cosine;
    }

    private:
    float FS = 0.f;
    float a = 0.f;
    float s[2] = {0.f, 0.f};
};