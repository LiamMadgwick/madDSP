/*
  ==============================================================================

    LFO.h
    Created: 27 Aug 2022 4:48:30pm
    Author:  Liam Madgwick

  ==============================================================================
*/

#pragma once
#include "DSP2.h"

class LFO
{
public:
    LFO() {};
    ~LFO() {};

    void Init(float sample_rate) { FS = sample_rate; Rate(0.1f), Amp(0.0f); }
    void Amp(float amplitude) { lfo_amp = amplitude; }
    void Rate(float rate)
    { 
        rate = 4.0f * rate / FS;
        rate *= lfo_rate < 0.0f ? -1.f : 1.f;
        lfo_rate = fclamp(rate, -0.25f, 0.25f);
    }
    void Shape(float shape) { lfo_shape = fclamp(shape, 0.025f, 1.0f); }

    inline float ProcessLFO()
    {
        lfo_phase += lfo_rate;

        if (lfo_phase > 1.f)
        {
            lfo_phase -= 2.f;
        }

        return (lfo_phase*(1-abs(lfo_phase))) * lfo_amp;
    }
    
private:
    float FS = 0.f;
    float lfo_rate = 0.f;
    float lfo_amp = 0.f;
    float lfo_phase = 0.f;
    float lfo_phase2 = 0.f;
    float lfo_shape = 0.f;
};