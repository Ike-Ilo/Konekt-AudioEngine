#pragma once
#include "Biquad.h"
#include "FilterIntermediates.h"

    static inline void bandPassFilterSG(Biquad& ce, const FilterIntermediates& m)
//    Constant skirt gain, peak gain = Q
    
    {
        double b0 = m.sinw0 / 2;
        double b1 = 0.0;
        double b2 = - m.sinw0 / 2;
        double a0 = 1.0 + m.qCase;
        double a1 = -(2.0 * m.cosw0);
        double a2 = 1.0 - m.qCase;
        
        ce.setNormalized(b0, b1, b2, a0, a1, a2);
        
        ce.b0 = b0; ce.b1 = b1; ce.b2 = b2; ce.a1 = a1; ce.a2 = a2;
    }