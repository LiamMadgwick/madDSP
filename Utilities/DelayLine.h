/*

    DelayLine
    January 2013
    Liam Madgwick

*/

#pragma once
#include <stdint.h>

template<size_t max_size>
class DelayLine
{
public:
    DelayLine() {};
    ~DelayLine() {};

    void Init(int sampleRate) { FS = sampleRate; ms_ = FS * 0.001f; Reset(); }
    void Reset()
    {
        for (size_t i = 0; i < max_size; i++)
        {
            line_[i] = 0.0f;
        }
        write_ptr_ = 0;
        delay_ = 1;
    }
    inline void Write(float sample)
    {
        line_[write_ptr_] = sample;
        write_ptr_ = (write_ptr_ - 1 + max_size) % max_size;
    }
    inline const float Read(float delay)
    {
        SetDelay(delay);
        float a = line_[(write_ptr_ + delay_) % max_size];
        float b = line_[(write_ptr_ + delay_ + 1) % max_size];
        float x = 0.0f;
        x = a + (b - a) * frac_; //linear interp...
        return x;
    }
    inline const float ReadMS(float ms)
    {
        SetDelayMS(ms);
        float a = line_[(write_ptr_ + delay_) % max_size];
        float b = line_[(write_ptr_ + delay_ + 1) % max_size];
        float x = 0.0f;
        x = a + (b - a) * frac_; //linear interp..
        return x;
    }
private:
    inline void SetDelay(float delay)
    {
        int32_t int_delay = static_cast<int32_t>(delay);
        frac_ = delay - static_cast<float>(int_delay);
        delay_ = static_cast<size_t>(int_delay) < max_size ? int_delay : max_size - 1;
    }
    inline void SetDelayMS(float delayms)
    {
        SetDelay(delayms * ms_);
    }
    float frac_;
    size_t write_ptr_;
    size_t delay_;
    float line_[max_size];
    float FS = 0.f; 
    float ms_ = 0.f;
};