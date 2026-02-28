//
//  Biquad.h
//  
//
//  Created by ike iloegbu on 2/28/26.
//
#ifndef BIQUAD_H
#define BIQUAD_H
#include <iostream>


struct Biquad
{
    double b0, b1, b2;
    double a1, a2;
    
    double z1, z2;
    
    Biquad();
    
    float process(float input);
    
    void reset();
};

#endif
