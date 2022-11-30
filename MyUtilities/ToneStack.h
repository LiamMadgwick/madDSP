/*
  ==============================================================================

    ToneStack.h
    Author:  Liam Madgwick
    Heavily borrowed from Faust Library by Guitarix

  ==============================================================================
*/

class ToneStack
{
    public:
    ToneStack() {};
    ~ToneStack() {};
    void Init(float sampleRate) {FS = sampleRate;}
    void setEnvironment(float _r1, float _r2, float _r3, float _r4, float _c1, float _c2, float _c3)
    {
        r1 = _r1 * k;
        r2 = _r2 * k;
        r3 = _r3 * k;
        r4 = _r4 * k;

        c1 = _c1 * pF;
        c2 = _c2 * nF;
        c3 = _c3 * nF;
    }
    
    //treble, mid, bass
    void setTone(float treble, float mids, float bass)
    {
        if(t != treble || m != mids || l != bass){t = treble; m = mids; l = bass ; compute_coeffs();}
    }
    float Process(float in)
    {
        
        x[0] = in - A1/A0*x[1] - A2/A0*x[2] - A3/A0*x[3];
        
        float y;
        
        y = B0*x[0] + B1*x[1] + B2*x[2] + B3*x[3];
        
        x[3] = x[2];
        x[2] = x[1];
        x[1] = x[0];
        
        return y * (1/A0);
    }
    
    private:

    void compute_coeffs()
    {
        b1 = t*c1*r1 + m*c3*r3 + l*(c1*r2 + c2*r2) + (c1*r3 + c2*r3);
        
        b2 = t*(c1*c2*r1*r4 + c1*c3*r1*r4) - m*m*(c1*c3*r3*r3 + c2*c3*r3*r3)
        + m*(c1*c3*r1*r3 + c1*c3*r3*r3 + c2*c3*r3*r3)
        + l*(c1*c2*r1*r2 + c1*c2*r2*r4 + c1*c3*r2*r4)
        + l*m*(c1*c3*r2*r3 + c2*c3*r2*r3)
        + (c1*c2*r1*r3 + c1*c2*r3*r4 + c1*c3*r3*r4);
        
        b3 = l*m*(c1*c2*c3*r1*r2*r3 + c1*c2*c3*r2*r3*r4)
        - m*m*(c1*c2*c3*r1*r3*r3 + c1*c2*c3*r3*r3*r4)
        + m*(c1*c2*c3*r1*r3*r3 + c1*c2*c3*r3*r3*r4)
        + t*c1*c2*c3*r1*r3*r4 - t*m*c1*c2*c3*r1*r3*r4
        + t*l*c1*c2*c3*r1*r2*r4;
        
        a0 = 1;
        
        a1 = (c1*r1 + c1*r3 + c2*r3 + c2*r4 + c3*r4)
        + m*c3*r3 + l*(c1*r2 + c2*r2);
        
        a2 = m*(c1*c3*r1*r3 - c2*c3*r3*r4 + c1*c3*r3*r3 + c2*c3*r3*r3)
        + l*m*(c1*c3*r2*r3 + c2*c3*r2*r3) - m*m*(c1*c3*r3*r3 + c2*c3*r3*r3)
        + l*(c1*c2*r2*r4 + c1*c2*r1*r2 + c1*c3*r2*r4 + c2*c3*r2*r4)
        + (c1*c2*r1*r4 + c1*c3*r1*r4 + c1*c2*r3*r4 + c1*c2*r1*r3 + c1*c3*r3*r4 + c2*c3*r3*r4);
        
        a3 = l*m*(c1*c2*c3*r1*r2*r3 + c1*c2*c3*r2*r3*r4)
        - m*m*(c1*c2*c3*r1*r3*r3 + c1*c2*c3*r3*r3*r4)
        + m*(c1*c2*c3*r3*r3*r4 + c1*c2*c3*r1*r3*r3 - c1*c2*c3*r1*r3*r4)
        + l*c1*c2*c3*r1*r2*r4
        + c1*c2*c3*r1*r3*r4;
        
        c = 2*FS;
        
        B0 = -b1*c - b2*pow(c,2) - b3*pow(c,3);
        B1 = -b1*c + b2*pow(c,2) + 3*b3*pow(c,3);
        B2 = b1*c + b2*pow(c,2) - 3*b3*pow(c,3);
        B3 = b1*c - b2*pow(c,2) + b3*pow(c,3);
        A0 = -a0 - a1*c - a2*pow(c,2) - a3*pow(c,3);
        A1 = -3*a0 - a1*c + a2*pow(c,2) + 3*a3*pow(c,3);
        A2 = -3*a0 + a1*c + a2*pow(c,2) - 3*a3*pow(c,3);
        A3 = -a0 + a1*c - a2*pow(c,2) + a3*pow(c,3);
    }

    float x[4] = {0.f, 0.f, 0.f, 0.f};
    float FS;
    float b0, b1, b2, b3;
    float a0, a1, a2, a3;
    float B0, B1, B2, B3;
    float A0, A1, A2, A3;
    float c;

    float t, m, l;

    float k = 1000;
    float M = 1000000;
    float nF = 0.000000001;
    float pF = 0.000000000001;

    float r1, r2, r3, r4, c1, c2, c3;
};
