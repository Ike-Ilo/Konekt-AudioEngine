//
//  AudioFilters.h
//  
//
//  Created by ike iloegbu on 2/28/26.
//

#ifndef AUDIOFILER_H
#define AUDIOFILTER_H

#include <iostream>
#include <cstdint>
#include "Biquad.h"

class AudioFilter()
{
private:
    uInt32_t samplesRate;
    double cutoff;
    double Q;
    Biquad biquad;
    
public:
    
    AudioFilter(uint32_t sampleRate);
    
    void setLowPass(double cutoff, double Q);
    float processSample(float input)
    
    
}
