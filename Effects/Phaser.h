/*

    Phaser
    January 2013
    Liam Madgwick

*/

#pragma once

#include "BasicFilters.h"
#include "QuadratureOsc.h"

#define MAX_DEPTH 400.f
#define MIN_DEPTH 50.f
#define MAX_LFO_RATE 12.f
#define MIN_CUTOFF 420.f - MAX_DEPTH

class Phaser
{
    public:
    Phaser()    {};
    ~Phaser()    {};
    void Init(float sampleRate)
    {
        fs = sampleRate;
        lfo.Init(fs);
        for(int i = 0; i < 4; i ++)
        {
            ap[i].Init(fs, 2);
        }
    }
    void Depth(float depth)
    {
        _depth = MIN_DEPTH + (depth * MAX_DEPTH);
    }
    void Rate(float rate)
    {
        _rate = rate * MAX_LFO_RATE;
        lfo.setFreq(_rate);
    }
    void Blend(float blend)
    {
        _blend = blend;
    }
    float Process(float x)
    {
        mod = lfo.Process(0) * _depth;
        ap[0].Cutoff(MIN_CUTOFF + mod);
        ap[1].Cutoff(MIN_CUTOFF + (MIN_CUTOFF * 0.25f) + mod);
        ap[2].Cutoff(MIN_CUTOFF + (MIN_CUTOFF * 0.5f) + mod);
        ap[3].Cutoff(MIN_CUTOFF + (MIN_CUTOFF * 0.75f) + mod);

        y[0] = ap[0].Process(x);
        y[1] = ap[1].Process(y[0]);
        y[2] = ap[2].Process(y[1]);
        y[3] = ap[3].Process(y[2]);

        return y[3] * _blend + x * (1.f - _blend);
    }
    float LFOreturn()
    {
        return mod;
    }
    private:
    float fs;
    float _depth, _rate, _blend;
    float mod;
    float y[4] = {0.f, 0.f, 0.f, 0.f};
    QuadratureOsc lfo;
    Second_Order_Canonical ap[4];
};