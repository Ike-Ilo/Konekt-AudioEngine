//  AudioFilters.h
//  Created by ike iloegbu on 2/28/26.

#pragma once
#include <iostream>
#include <numbers>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include "Biquad.h"
#include <iomanip>
constexpr double PI = 3.14159265358979323846;

class AudioFilter{
private:
//  variables use to calculate the coefficents

    double targetQ = 0.0;
    double targetGain = 0.0;
    double targetCenter = 0.0;
    double targetS = 0.0;
    double targetBW = 0.0;

    double currentQ = 0.707;
    double currentGain = 1.0;
    double alpha = 0.0;

    u_int32_t sampleRate = 48000;
    double currentCenter = 1000.0;
    double w0 = 0.0;
    double cosw0 = 0.0;
    double sinw0 = 0.0;
    double qCase = 0.0;
    double sCase = 0.0;
    double bwCase = 0.0;
    double BW = 1.0;
    double A = 1.0;
    double S = 1.0;

    void computeCoefficients();
    void computeSmoothing();

    double dbToA(double gainDb);

    inline double clampd(double x, double lo, double hi); 
    
    float process(float input);

    void reset();
public:

    AudioFilter()
    {
        computeCoefficients();
        computeSmoothing();
    }

    inline void setSampleRate(u_int32_t sr)  {sampleRate = sr; computeSmoothing();}
    inline void setCenter(double hz)        {targetCenter = hz;}
    inline void setGainDb(double dB)        {targetGain = dB;}
    inline void setQ(double q)              {targetQ = q;}
    inline void setS(double s)              {targetS = s;}
    inline void setBandwidth(double bw)     {targetBW = bw; }

    void updateSmoothedParameters();
    
    inline void normalize(Biquad& f, double a0);

};


