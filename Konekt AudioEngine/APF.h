#pragma once
#include "Biquad.h"
#include "AudioFilters.h"

    static inline void allPassFilterCPG(Biquad& ce, const AudioFilter& a)
    {
        double b0 = 1.0 - a.qCase;
        double b1 = -(2.0 * a. cosw0);
        double b2 = 1.0 + a.qCase;
        double a0 = b2;
        double a1 = b1;
        double a2 = 1.0 - a.qCase;
        
        normalize(b0, b1, b2, a0, a1, a2);
        
        ce.b0 = b0; ce.b1 = b1; ce.b2 = b2; ce.a1 = a1; ce.a2 = a2;
    }
    