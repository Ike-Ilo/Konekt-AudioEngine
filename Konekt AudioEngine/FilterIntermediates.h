#pragma once
#include <numbers>
#include <cmath>

struct FilterIntermediates
{
    u_int32_t sr = 48000;
    double fc = 1000.0;
    double w0 = 0.0;
    double cosw0 = 0.0;
    double sinw0 = 0.0;
    double qCase = 0.0;
    double sCase = 0.0;
    double bwCase = 0.0;
    double gainDb = 0.0;
    double BW = 1.0;
    double A = 1.0;
    double S = 1.0;
    double Q = 0.707;


};