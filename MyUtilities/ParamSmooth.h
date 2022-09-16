#pragma once

class ParamSmooth
{
    public:
    ParamSmooth()  {};
    ~ParamSmooth() {};
    void Init(float smoothingMS, float sampleRate)
    {
        const float c_twoPi = 6.283185307179586476925286766559f;

        a = exp(-c_twoPi / (smoothingMS * 0.001f * sampleRate));
        b = 1.0f - a;
        z = 0.0f;
    };
    inline float Process(float in)
    {
        z = (in * b) + (z * a);
        return z;
    };
    private:
    float a, b, z;
};