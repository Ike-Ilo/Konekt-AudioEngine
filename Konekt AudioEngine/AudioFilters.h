//
//  AudioFilters.h
//  
//
//  Created by ike iloegbu on 2/28/26.
#pragma once
#ifndef AUDIOFILER_H
#define AUDIOFILTER_H

#include <iostream>
#include <numbers>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include "Biquad.h"
#include <iomanip>
constexpr double PI = 3.14159265358979323846;


class AudioFilter
{
private:
//  variables use to calculate the coefficents

    double targetQ = 0.0;
    double targetGain = 0.0;
    double targetCenter = 0.0;
    double targetS = 0.0;


    double currentQ = 0.0;
    double currentGain = 0.0;
    double currentCenter = 0.0;

    double alpha = 0.0;

    uint32_t sampleRate = 48000;

    void computeIntermediates();
    void computeSmoothing();

    inline double dbToA(double gainDb)
    {
        return std::pow(10, gainDb / 40.0);
    }
// staying in a safe mathematical range preventing instability, division by 0 and invaild frequencies
    inline double clampled(double x, double lo, double hi) {
        return std::max(lo, std::min(x,hi));
    }


    inline  void computeIntermediates()
    {
        double w0;
        double cosw0;
        double sinw0;
        double qCase;
        double sCase;
        double bWcase;
        double BW = 1.0;
        double S;


        const double sr = std::max<uint32_t>(sampleRate, 1u);
        targetCenter = clampled(targetCenter, 1.0, 0.49 * sr);

        const double A = dbToA(targetGain);
        w0 = 2 * PI * (double(targetCenter)/double(sr));
        cosw0 = std::cos(w0);
        targetQ = std::max(targetQ, 1e-6);
        S = std::max(targetS, 1e-6);
        sinw0 = std::sin(w0);
        const double sinw0_safe = (std::abs(sinw0) < 1e-12) ? 1e-12 : sinw0;
        qCase = sinw0_safe/(2*targetQ);
        sCase = (sinw0_safe/2.0) * std::sqrt(((A + 1.0 /A) * (1.0 /S - 1.0)) + 2);
        bWcase = sinw0_safe * std::sinh((std::log(2.0)/2.0)*BW*(w0/sinw0_safe));
    }

    
    float process(float input);

    void reset();
public:

    inline void setSampleRate(uint32_t sr)  {sampleRate = sr; computeSmoothing();}
    inline void setCenter(double hz)        {targetCenter = hz;}
    inline void setGainDb(double dB)        {targetGain = dB;}
    inline void setQ(double q)              {targetQ = q;}
    inline void setS(double s)              {targetS = s;}

    void updateSmoothedParameters();
    

    inline void normalize(Biquad& f, double a0)
    {
        // avoids division by zero error. Using multiplication rather than division for speed. 
        const double invA0 = 1.0 / std::max(std::abs(a0), 1e-12);
        
        f.b0 *= invA0; 
        f.b1 *= invA0; 
        f.b2 *= invA0; 
        f.a1 *= invA0; 
        f.a2 *= invA0; 

        a0 = 1.0;
    }

};


#endif
