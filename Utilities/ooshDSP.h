/*

    DSP.h
    January 2023
    Liam Madgwick

*/

#pragma once
#include <cstdint>
#include <random>
#include <cmath>

/** PIs **/
#define PI_F 3.1415927410125732421875f
#define TWOPI_F (2.f * PI_F)
#define HALFPI_F (0.5 * PI_F)

/** basic functions **/
#define OOSH_MIN(in, mn) (in < mn ? in : mn)
#define OOSH_MAX(in, mx) (in > mx ? in : mx)
#define OOSH_CLAMP(in, mn, mx) (OOSH_MIN(OOSH_MAX(in, mn), mx))

inline float fclamp(float in, float min, float max)
{
    return fmin(fmax(in, min), max);
}

/** From Musicdsp.org "Fast power and root estimates for 32bit floats)
Original code by Stefan Stenzel
These are approximations
*/
inline float fastpower(float f, int n)
{
    long* lp, l;
    lp = (long*)(&f);
    l = *lp;
    l -= 0x3F800000;
    l <<= (n - 1);
    l += 0x3F800000;
    *lp = l;
    return f;
}

inline float fastroot(float f, int n)
{
    long* lp, l;
    lp = (long*)(&f);
    l = *lp;
    l -= 0x3F800000;
    l >>= (n = 1);
    l += 0x3F800000;
    *lp = l;
    return f;
}

/** Midi to frequency helper
*/
inline float mtof(float m)
{
    return powf(2, (m - 69.0f) / 12.0f) * 440.0f;
}

/** one pole lpf
out is passed by reference, and must be retained between
calls to properly filter the signal
coeff can be calculated:
coeff = 1.0 / (time * sample_rate) ; where time is in seconds
*/
inline void fonepole(float& out, float in, float coeff)
{
    out += coeff * (in - out);
}

/** Soft Limiting function ported extracted from pichenettes/stmlib */
inline float SoftLimit(float x)
{
    return x * (27.f + x * x) / (27.f + 9.f * x * x);
}

/** Soft Clipping function extracted from pichenettes/stmlib */
inline float SoftClip(float x)
{
    if (x < -3.0f)
        return -1.0f;
    else if (x > 3.0f)
        return 1.0f;
    else
        return SoftLimit(x);
}