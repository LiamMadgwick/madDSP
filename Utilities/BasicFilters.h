/*

    BasicFilters
    January 2013
    Liam Madgwick

*/
//All filters from DAFX second edition

#pragma once
#include "ooshDSP.h"

class SV_filter
{
    public:
    SV_filter() {};
    ~SV_filter()    {};
    void Init(float sampleRate){fs = sampleRate;}
    //0 = highpass state, 1 = bandpass state, 2 = lowpass state
    void State(int _state)
    {
        state = _state;
    }
    //cutoff in hz
    void Cutoff(float fc)
    {
        _fc = fc;
    }
    //q should be between 4 - 50? (maybe higher)
    void Q(float q)
    {
        _q = q;
    }
    float Process(float x)
    {
        coeffs();
        
        float y = 0.f;

        yh[0] = x - yl[1] - q1*yb[1];
        if(state == 0)
        {
           y = yh[0];
        }
        yb[0] = f1*yh[0] + yb[1];
        if(state == 1)
        {
            y = yb[0];
        }
        yl[0] = f1*yb[0] + yl[1];
        if(state == 2)
        {
            y = yl[0];
        }

        yh[1] = yh[0];
        yb[1] = yb[0];
        yl[1] = yl[0];

        return y;
    }
    private:
    void coeffs()
    {
        f1 = 2*sin(PI_F*_fc/fs);
        q1 = 1/_q;
    }
    float fs;
    float _fc, _q;
    float f1, q1;
    int state;
    float yh[2] = {0.f, 0.f};
    float yb[2] = {0.f, 0.f};
    float yl[2] = {0.f, 0.f};
};

class First_Order_Canonical
{   
    public:
    First_Order_Canonical() {};
    ~First_Order_Canonical()    {};
    //samplerate required, state 0 = Lowpass, state 1 = Highpass, state 2 = Allpass, picked at the start of operation and shouldnt be changed while in use.
    void Init(float sampleRate, int state) {fs = sampleRate; _state = state;}
    //cutoff in hz..
    void Cutoff(float fc)
    {
        K = tan(PI_F*fc/fs);
    }
    float Process(float x)
    {
        coefficients();
        xh[0] = x - a1*xh[1];
        float y = b0*xh[0] + b1* xh[1];
        xh[1] = xh[0];
        xh[0] = x;
        return y;
    }
    private:
    void coefficients()
    {
        switch(_state)
        {
            case 0:
            lowpass();
            break;
            case 1:
            highpass();
            break;
            case 2:
            allpass();
            break; 
        }
    }
    void lowpass()
    {
        b0 = K/(K + 1);
        b1 = b0;
        a1 = (K - 1)/(K + 1);
    }
    void highpass()
    {
        b0 = 1/(K + 1);
        b1 = -1/(K + 1);
        a1 = (K - 1)/(K + 1);
    }
    void allpass()
    {
        b0 = (K - 1)/(K + 1); 
        b1 = 1;
        a1 = (K - 1)/(K + 1);
    }
    int _state;
    float fs;
    float K;
    float b0, b1, a1;
    float xh[2] = {0.f, 0.f};
};

//currently unfinished!!
class Second_Order_Canonical
{
    public:
    Second_Order_Canonical()    {};
    ~Second_Order_Canonical()   {};
    //state must be set at start.. state = 0 for lowpass, state = 1 for highpass, state = 2 for allpass, state = 3 for bandpass, state = 4 for bandreject
    void Init(float sampleRate, int state) {fs = sampleRate; _state = state;}
    void Cutoff(float fc)
    {
        _fc = fc;
    }
    void Q(float q)
    {
        _q = 1.2f + q;
    }
    float Process(float x)
    {   
        coefficients();
        xh[0] = x - a1*xh[1] - a2*xh[2];
        float y = 0.f;
        y = b0*xh[0] + b1*xh[1] + b2*xh[2];

        xh[2] = xh[1];
        xh[1] = xh[0];

        return y;
    }
    private:
    void coefficients()
    {
        K = tan(PI_F*_fc/fs);
        Q1 = 1/_q;
        
        allpass();
    }
    void allpass()
    {
        b0 = (K*K-Q1-K+Q1)/(K*K*Q1+K+Q1);
        b1 = (2*Q1*(K*K-1))/(K*K*Q1+K+Q1);
        b2 = 1;
        a1 = (2*Q1*(K*K-1))/(K*K*Q1+K+Q1);
        a2 = (K*K*Q1-K+Q1)/(K*K*Q1+K+Q1);
    }
    int _state;
    float fs;
    float _fc, _q;
    float K, KK, Q1;
    float b0, b1, b2, a1, a2;
    float xh[3] = {0.f, 0.f, 0.f};
    
};

class FirstOrder_Allpass
{
    public:
    FirstOrder_Allpass() {};
    ~FirstOrder_Allpass()   {};
    void Init(float sampleRate) {fs = sampleRate;}
    void Cutoff(float fc)
    {
        _fc = fc;
    }
    float Process(float x)
    {
        float y = 0.f;
        xh[0] = x = c*xh[1];
        y = c*xh[0] + xh[1];
        xh[1] = xh[0];
        return y;
    }
    private:
    void coefficients()
    {
        c = (tan(PI_F * _fc/fs) - 1) / (tan(PI_F * _fc/fs) + 1);
    }
    float fs;
    float c;
    float _fc;
    float xh[2] = {0.f, 0.f};
};

class Second_Order_Allpass
{
    public:
    Second_Order_Allpass()  {};
    ~Second_Order_Allpass() {};
    void Init(float sampleRate){fs = sampleRate;}
    void Cutoff(float fc)
    {
        _fc = 2*fc/fs;
    }
    void Band(float fb)
    {
        _fb = 2*fb/fs;
    }
    float Process(float x)
    {
        coefficients();
        float xh_new = x - d*(1-c)*xh[0] + c*xh[1];
        float ap_y = -c*xh_new + d*(1-c)*xh[0] + xh[1];
        xh[1] = xh[0];
        xh[0] = xh_new;
        return ap_y;
    }

    private:
    void coefficients()
    {
        c = (tan(PI_F*_fb/2)) - 1 / (tan(PI_F*_fb/2)) + 1;
        d = -cos(PI_F*_fc);
    }
    float fs;
    float c, d;
    float _fc, _fb;
    float xh[2] = {0.f, 0.f};
};