#pragma once
#include "Biquad.h"
#include "FilterIntermediates.h"
#include "FilterDSP.h"


static inline void peakingFilter(Biquad& ce, const FilterIntermediates& m)
{
    const double A = dbToA(m.gainDb);

    
    double b0 = 1.0 + m.qCase * A;
    double b1 = -2.0 * m. cosw0;
    double b2 = 1.0 - m.qCase * A;
    double a0 = 1.0 + m.qCase / A;
    double a1 = -2.0 * m.cosw0;
    double a2 = 1.0 - m.qCase / A;
    
    ce.setNormalized(b0, b1, b2, a0, a1, a2);
    
}