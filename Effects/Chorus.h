/*

    Chorus
    January 2013
    Liam Madgwick

*/

//specifically designed for the oosh petal hardware!

#include "DelayLine.h"
#include "BasicFilters.h"
#include "QuadratureOsc.h"
#include "GuassNoise.h"
#include "ooshDSP.h"

#define MAX_LFO_RATE 12.f
#define MAX_DELAY static_cast<size_t>(22050)
#define MAX_DEPTH 15.f
#define MIN_OFFSET 1.f //in ms..
#define MULT_OFFSET 29.f //this plus min_offset are the total sum in ms.
#define MIN_CUTOFF 1.f //in hz for the lowpass filter on the noise mod;
#define MAX_CUTOFF (18.f - MIN_CUTOFF)

class Chorus
{
    public:
    Chorus()    {};
    ~Chorus()    {};
    void Init(float sampleRate)
    {
        fs = sampleRate;
        delay[0].Init(fs);
        delay[1].Init(fs);
        lfo.Init(fs);
        noise.Init(-1.f, 2.f);
        lowpass.Init(fs);
        lowpass.State(2);
        lowpass.Q(4);
    }
    inline void Mod(int mod_state)
    {
        _mod_state = mod_state;
    }
    inline void Time(float deltime)
    {
        _deltime = fclamp(30.f + deltime * 120.f, 30.f, 150.f);
        fonepole(curr_deltime, _deltime, 0.000566f);
    }
    inline void Offset(float offset)
    {
        _offset = MIN_OFFSET + (offset * MULT_OFFSET);
        fonepole(curr_offset, _offset, 0.00113f);
    }
    inline void Rate(float rate)
    {
        _rate = rate * rate * MAX_LFO_RATE;
        fonepole(curr_rate, _rate, 0.00226f);
        lfo.setFreq(curr_rate);
        lowpass.Cutoff(MIN_CUTOFF + (rate * rate * MAX_CUTOFF));
    }
    inline void Depth(float depth)
    {
        _depth = depth * depth * MAX_DEPTH;
    }
    inline void Feedback(float feedback)
    {
        _feedback = fclamp(feedback * feedback, 0.f, 0.85f);
    }
    inline void Expression(float expression)
    {
        _expression = expression;
    }
    inline void Blend(float blend)
    {
        _blend = 0.3f + (blend * 0.6f);
    }
    float Process(float x)
    {
        handleMod(_mod_state);
        delay[0].Write(x + (y * -_feedback));
        delay[1].Write(x + (y * -_feedback));
        Ax = delay[0].ReadMS(curr_deltime + MOD);
        Bx = delay[1].ReadMS(curr_offset + curr_deltime + MODh[1]);
        float AB = .5f*(Ax + Bx);
        y = (AB * _blend) + (x * (1.f - _blend));
        return fclamp(y, -1.f, 1.f);
    }

    private:
    void handleMod(int state)
    {
        if(state == 0)
        {
            MOD = lfo.Process(0) * _depth;
        }
        else if(state == 1)
        {
            MOD = lowpass.Process(noise.Process()) * _depth;
            lfo.Process(0);
        }
        MODh[1] = MODh[0]; 
        MODh[0] = MOD;
    }
    float fs;
    int _mod_state;
    float Ax, Bx, y;
    float MOD;
    float MODh[2] = {0.f, 0.f};
    float _deltime, _rate, _noise_lowpass, _depth, _offset, _feedback, _blend, _expression;
    float curr_deltime, curr_offset, curr_rate;
    typedef DelayLine<MAX_DELAY> delayline;
    delayline delay[2];
    QuadratureOsc lfo;
    GuassNoise noise;
    SV_filter lowpass;
};