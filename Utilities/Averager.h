/*

    Averager
    January 2013
    Liam Madgwick

*/

#pragma once
#include "ooshDSP.h"

class Averager
{
    public:
    Averager()  {};
    ~Averager() {};
    void Init(float sampleRate) {fs = sampleRate;}
    //attack in ms, fs*x i.e fs* 0.001 = 1 millisecond
    void Attack(float attack)
    {
        _attack = attack;
        ga = exp(-1/(fs*_attack));
    }
    //same as attack, fs*x i.e fs* 0.001 = 1 millisecond
    void Release(float release)
    {
        _release = release;
        gr = exp(-1/(fs*_release));
    }
    float Process(float d)
    {
        if(y[1] < d)
        {
            g = ga;
        }
        else
        {
            g = gr;
        }

        d *= d;
        y[0] = (1 - g)*d + g * y[1];
        y[1] = y[0];
        return y[0];
    }
    private:
    float fs;
    float _attack, _release;
    float g, ga, gr;
    float y[2] = {0.f, 0.f};
};