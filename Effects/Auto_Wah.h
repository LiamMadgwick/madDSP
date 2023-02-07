/*

    Auto Wah
    January 2013
    Liam Madgwick

*/

//specifically designed for the oosh petal hardware!

#pragma once
#include "ooshDSP.h"
#include "Averager.h"
#include "BasicFilters.h"

#define MIN_CUTOFF 300.f
#define MAX_CUTOFF 1100.f - MIN_CUTOFF
#define MIN_Q 4.f
#define MAX_Q 50.f - MIN_Q
#define MAX_ATTACK 0.1 //100 ms
#define MAX_RELEASE 1.1 //1100 ms
#define MAX_SENSITIVITY 4.5f
#define EXPRESSION_CUTOFF_RANGE 400.f
#define EXPRESSION_Q_RANGE 10.f

class Auto_Wah
{
    public:
    Auto_Wah()  {};
    ~Auto_Wah() {};

    void Init(float sampleRate)
    {
        fs = sampleRate;
        bp.Init(sampleRate);
        bp.State(1);
        ar_d.Init(sampleRate);
    }

    void Cutoff(float fc)
    {
        _fc = MIN_CUTOFF + (fc * MAX_CUTOFF);
    }

    void Q(float qc)
    {
        _qc = MIN_Q + (qc * MAX_Q);
    }

    void Attack(float at)
    {
        _at = at * MAX_ATTACK;
    }

    void Release(float rt)
    {
        _rt = rt * MAX_RELEASE;
    }

    void Sensitivity(float ar_s)
    {
        _ar_s = ar_s * MAX_SENSITIVITY;
    }

    void Blend(float blend)
    {
        _blend = blend;
    }

    void Expression(float exp)
    {
        _exp_fc = exp * EXPRESSION_CUTOFF_RANGE;
        _exp_qc = exp * EXPRESSION_Q_RANGE;
    }

    float Process(float x)
    {
        d = ar_d.Process(x * _ar_s);
        Controls();
        float y = (bp.Process(x) * _blend) + ((1.f - _blend) * x);
        return y;
    }

    float AR_led()
    {
        return d;
    }
    private:
    void Controls()
    {
        float cutoff = _fc + _exp_fc;
        float mod = d * (cutoff - 10.f);
        ar_d.Attack(_at);
        ar_d.Release(_rt);
        bp.Cutoff(cutoff + mod);
        bp.Q(_qc + _exp_qc);
    }
    float fs;
    float _fc, _qc, _at, _rt, _ar_s, _exp_fc, _exp_qc, _blend;
    float d;
    typedef SV_filter filter;
    filter bp;
    Averager ar_d;
};
