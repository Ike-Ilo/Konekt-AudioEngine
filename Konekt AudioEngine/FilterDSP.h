//  main.cpp
//  Konekt AudioEngine
//  Created by ike iloegbu on 2/21/26.

#include <iostream>
#include <numbers>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include "AudioFilters.h"
#include "FilterIntermediates.h"

    

void AudioFilter::computeSmoothing()
{
    const double updateRate = double(sampleRate) / 64.0;
    const double T = 1.0 / updateRate;
    const double smoothingTime = 0.020;
    const double alpha = 1.0 - std::exp(-T / smoothingTime);        
}

void AudioFilter::updateSmoothedParameters()
{
    currentCenter += alpha * (targetCenter - currentCenter);
    currentGain   += alpha * (targetGain   - currentGain);

    computeCoefficients(); 
}

inline double dbToA(double gainDb)
{
    return std::pow(10, gainDb / 40.0);
}

inline double clampd(double x, double lo, double hi) {
    return std::max(lo, std::min(x,hi));
}

inline void computeCoefficents(FilterIntermediates& m, 
                            u_int32_t sampleRate,
                            double centerHz,
                            double gainDb,
                            double Q,
                            double S,
                            double BW,
                            double A)
{
    m.sr = std::max<u_int32_t>(sampleRate, 1u);
    m.fc = clampd(centerHz, 1.0, 0.49 * double(m.sr));
    m.A = dbToA(m.gainDb);
    m.gainDb = gainDb;
    m.w0 = 2 * PI * (double(m.fc)/double(m.sr));
    m.cosw0 = std::cos(m.w0);
    m.Q = std::max(Q, 1e-6);
    m.S = std::max(S, 1e-6);
    m.sinw0 = std::sin(m.w0);
    m.BW = BW;
    const double sinw0_safe = (std::abs(m.sinw0) < 1e-12) ? 1e-12 : m.sinw0;
    m.qCase = sinw0_safe / (2 * m.Q);
    m.sCase = (sinw0_safe / 2.0) * std::sqrt(((A + 1.0 /A) * (1.0 /S - 1.0)) + 2);
    m.bwCase = sinw0_safe * std::sinh((std::log(2.0)/2.0)*m.BW*(m.w0/sinw0_safe));
}

// staying in a safe mathematical range preventing instability, division by 0 and invaild frequencies

