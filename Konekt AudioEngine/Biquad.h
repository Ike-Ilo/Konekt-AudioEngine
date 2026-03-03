//
//  Biquad.h
//  
//
//  Created by ike iloegbu on 2/28/26.
//
#ifndef BIQUAD_H
#define BIQUAD_H
#include <iostream>
#include <algorithm>
#include <cmath>


struct Biquad
{
    double b0, b1, b2;
    double a1, a2;
    
    double z1, z2;
    
    inline void setNormalized(double nb0, 
                            double nb1, 
                            double nb2,
                            double a0,
                            double na1,
                            double na2)
    {
        const double invA0 = 1.0 / std::max(std::abs(a0), 1e-12);
        b0 = nb0 * invA0;
        b1 = nb1 * invA0;
        b2 = nb2 * invA0;
        a1 = na1 * invA0;
        a2 = na2 * invA0;

    }
    
    float process(float input)
    {
        double y = b0 * input + z1;
        z1 = b1 * input - a1 * y + z2 ;
        z2 = b2 * input - a2 * y;
        return static_cast<float>(y);
    }
    
    void reset()
    {
        z1 = z2 = 0.0;
    }

};

#endif
