#pragma once
#include "Biquad.h"
#include "AudioFilters.h"
#include "FilterIntermediates.h"

static inline void bandPassFilterCPG(Biquad& ce, FilterIntermediates& m)
//    Constand 0dB peak gain
{
    double b0 = m.qCase;
    double b1 = 0.0;
    double b2 = -b0;
    double a0 = 1.0 + m.qCase;
    double a1 = -(2.0 * m.cosw0);
    double a2 = 1.0 - m.qCase;
    
    ce.setNormalized(b0, b1, b2, a0, a1, a2);
    }
