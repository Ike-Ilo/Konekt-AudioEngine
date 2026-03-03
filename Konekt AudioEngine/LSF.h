#pragma once
#include "Biquad.h"
#include "FilterIntermediates.h"

static inline void lowShelfFilter(Biquad& ce, const FilterIntermediates& m)
{
    const double sqrtA = std::sqrt(m.A);

    
    double b0 = m.A * ((m.A + 1.0) - (m.A - 1.0) * m.cosw0 + 2.0 * (sqrtA * m.sCase));
    double b1 = 2.0 * m.A * ((m.A -1.0) - (m.A + 1.0) * m.cosw0);
    double b2 = m.A  * (((m.A  + 1.0) - (m.A - 1.0) * m.cosw0 - 2.0 * (sqrtA * m.sCase)));
    double a0 = (m.A  + 1.0) + (m.A  - 1.0) * m.cosw0 + 2.0 * (sqrtA * m.sCase);
    double a1 = -2.0 * ((m.A - 1.0) + (m.A  + 1.0) * m.cosw0);
    double a2 = (m.A + 1.0) + (m.A - 1.0) * m.cosw0 - 2.0 * (sqrtA * m.sCase);
    
    ce.setNormalized(b0, b1, b2, a0, a1, a2);
    
}