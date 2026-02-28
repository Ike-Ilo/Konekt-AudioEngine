//
//  main.cpp
//  Konekt AudioEngine
//
//  Created by ike iloegbu on 2/21/26.
//

#include <iostream>
#include <numbers>
#include <cstdint>
#include <cmath>
#include <algorithm>

struct Biquad {
    double b0 = 1.0, b1 = 0.0, b2 = 0.0;
    double a1 = 0.0, a2 = 0.0;
    
    double z1 = 0.0, z2 = 0.0;
    
    // Direct Form II Transposed. Uses 2 delay states
    inline float process(float x)
    {
        double y = b0 * x + z1;
        z1 = b1 * x - a1 * y + z2;
        z2 = b2 * x - a2 * y;
        return static_cast<float>(y);
    }
    
    inline void reset()
    {
        z1 = z2 = 0.0;
    }
};

class AudioFilter {
private:
    double w0 = 0.0;
    double cosw0 = 1.0;
    double sinw0 = 1.0;
    double qCase = 0.0;
    double sCase = 0.0;
    double bwCase = 0.0;
    
    double gain = 0.0;
    double center = 1000.0;
    uint32_t sampleRate = 44100;
    double Q = 0.707;
    double S = 1.0;
    double BW = 1.0;
    
    static inline double dbToA(double gainDb)
    {
        return std::pow(10, gainDb / 40.0);
    }
    
    static inline void normalize(double& b0, double& b1, double& b2,
                                 double& a0, double& a1, double& a2)
    {
        const double invA0 = 1.0 / std::max(std::abs(a0), 1e-12);
        
        b0 *= invA0; b1 *= invA0; b2 *= invA0; a1 *= invA0; a2 *= invA0; a0 = 1.0;
    }
    
    static inline double clamped(double x, double lo, double hi){
        return std::max(lo, std::min(x, hi));
    }
    
    inline void computerIntermediates() {
        const double sr = std::max<uint32_t>(sampleRate, 1u);
        center = clamped(center, 1.0, 0.49 * sr);
        
        const double A = dbToA(gain);
        w0 = 2 * std::numbers::pi * (double(center)/double(sampleRate));
        cosw0 = std::cos(w0);
        Q = std::max(Q, 1e-6);
        S = std::max(S, 1e-6);
        sinw0 = std::sin(w0);
        const double sinw0_safe = (std::abs(sinw0) < 1e-12) ? 1e-12 : sinw0;
        qCase = sinw0_safe/(2*Q);
        sCase = (sinw0_safe/2.0) * std::sqrt(((A + 1.0 /A) * (1.0 /S - 1.0)) + 2);
        bwCase = sinw0_safe * std::sinh((std::log(2.0)/2.0)*BW*(w0/sinw0_safe));

        
    }
    
    inline double smoothing () {

        const double updateRate = double(sampleRate) / 64.0;
        const double T = 1.0 / updateRate;
        const double smoothingTime = 0.020;
        const double alpha = 1.0 - std::exp(-T / smoothingTime);
        
        return alpha;
        
    }
    
    public: AudioFilter()
    : gain(0), center(0), sampleRate(0), Q(0), S(0), BW(0)
    {}
    
    AudioFilter(double g, double c, uint32_t sR, double q, double s, double bW)
    : gain(g), center(c), sampleRate(sR), Q(q), S(s), BW(bW)
    {
        computerIntermediates();
        
    }
    
    
    inline void setSampleRate(uint32_t sr) {sampleRate = sr; computerIntermediates();}
    inline void setCenter(double hz)       {center     = hz; computerIntermediates();}
    inline void setGainDb(double dB)       {gain       = dB; computerIntermediates();}
    inline void setQ(double q)             {Q          = q;  computerIntermediates();}
    inline void setS(double s)             {S          = s;  computerIntermediates();}
    inline void setBW(double bw)           {BW         = bw; computerIntermediates();}



    
    static inline void lowPassFilter(Biquad& ce, const AudioFilter& a)
    {
        double b0 = (1.0 - a.cosw0)/2.0;
        double b1 = (1.0 - a.cosw0);
        double b2 = b0;
        double a0 = 1.0 + a.qCase;
        double a1 = -2.0 * a.cosw0;
        double a2 = 1.0 - a.qCase;
        
        normalize(b0, b1, b2, a0, a1, a2);
        
        ce.b0 = b0; ce.b1 = b1; ce.b2 = b2; ce.a1 = a1; ce.a2 = a2;
        
    }
    
    static inline void hiPassFilter(Biquad& ce, const AudioFilter& a)
    {
        double b0 = (1.0 + a.cosw0)/2.0;
        double b1 = -(1.0 + a.cosw0);
        double b2 = b0;
        double a0 = 1.0 + a.qCase;
        double a1 = -2.0 * a.cosw0;
        double a2 = 1.0 - a.qCase;
        
        normalize(b0, b1, b2, a0, a1, a2);
        
        ce.b0 = b0; ce.b1 = b1; ce.b2 = b2; ce.a1 = a1; ce.a2 = a2;
    }
    
    static inline void bandPassFilterSG(Biquad& ce, const AudioFilter& a)
//    Constant skirt gain, peak gain = Q
    
    {
        double b0 = a.sinw0 / 2;
        double b1 = 0.0;
        double b2 = - a.sinw0 / 2;
        double a0 = 1.0 + a.qCase;
        double a1 = -(2.0 * a.cosw0);
        double a2 = 1.0 - a.qCase;
        
        normalize(b0, b1, b2, a0, a1, a2);
        
        ce.b0 = b0; ce.b1 = b1; ce.b2 = b2; ce.a1 = a1; ce.a2 = a2;
    }
    
    static inline void bandPassFilterCPG(Biquad& ce, const AudioFilter& a)
//    Constand 0dB peak gain
    {
        double b0 = a.qCase;
        double b1 = 0.0;
        double b2 = -b0;
        double a0 = 1.0 + a.qCase;
        double a1 = -(2.0 * a.cosw0);
        double a2 = 1.0 - a.qCase;
        
        normalize(b0, b1, b2, a0, a1, a2);
        
        ce.b0 = b0; ce.b1 = b1; ce.b2 = b2; ce.a1 = a1; ce.a2 = a2;
    }
    
    static inline void notchFilter(Biquad& ce, const AudioFilter& a)
    {
        double b0 = 1.0;
        double b1 = -(2.0 * a.cosw0);
        double b2 = b0;
        double a0 = 1.0 + a.qCase;
        double a1 = -(2.0 * a.cosw0);
        double a2 = 1.0 - a.qCase;
        
        normalize(b0, b1, b2, a0, a1, a2);
        
        ce.b0 = b0; ce.b1 = b1; ce.b2 = b2; ce.a1 = a1; ce.a2 = a2;
    }
    
    static inline void allPassFilterCPG(Biquad& ce, const AudioFilter& a)
    {
        double b0 = 1.0 - a.qCase;
        double b1 = -(2.0 * a. cosw0);
        double b2 = 1.0 + a.qCase;
        double a0 = b2;
        double a1 = b1;
        double a2 = 1.0 - a.qCase;
        
        normalize(b0, b1, b2, a0, a1, a2);
        
        ce.b0 = b0; ce.b1 = b1; ce.b2 = b2; ce.a1 = a1; ce.a2 = a2;
    }
    
    static inline void peakingFilter(Biquad& ce, const AudioFilter& a)
    {
        const double A = dbToA(a.gain);

        
        double b0 = 1.0 + a.qCase * A;
        double b1 = -2.0 * a. cosw0;
        double b2 = 1.0 - a.qCase * A;
        double a0 = 1.0 + a.qCase / A;
        double a1 = -2.0 * a.cosw0;
        double a2 = 1.0 - a.qCase / A;
        
        normalize(b0, b1, b2, a0, a1, a2);
        
        ce.b0 = b0; ce.b1 = b1; ce.b2 = b2; ce.a1 = a1; ce.a2 = a2;
    }
    
    static inline void lowShelfFilter(Biquad& ce, const AudioFilter& a)
    {
        const double A = dbToA(a.gain);
        const double sqrtA = std::sqrt(A);
        const double sCase = a.sCase;

        
        double b0 = A * ((A + 1.0) - (A - 1.0) * a.cosw0 + 2.0 * (sqrtA * sCase));
        double b1 = 2.0 * A * ((A -1.0) - (A + 1.0) * a.cosw0);
        double b2 = A * (((A + 1.0) - (A - 1.0) * a.cosw0 - 2.0 * (sqrtA * sCase)));
        double a0 = (A + 1.0) + (A - 1.0) * a.cosw0 + 2.0 * (sqrtA * sCase);
        double a1 = -2.0 * ((A - 1.0) + (A + 1.0) * a.cosw0);
        double a2 = (A + 1.0) + (A - 1.0) * a.cosw0 - 2.0 * (sqrtA * sCase);
        
        normalize(b0, b1, b2, a0, a1, a2);
        
        ce.b0 = b0; ce.b1 = b1; ce.b2 = b2; ce.a1 = a1; ce.a2 = a2;
    }
    
    static inline void hiShelfFilter(Biquad& ce, const AudioFilter& a)
    {
        const double A = dbToA(a.gain);
        const double sqrtA = std::sqrt(A);
        const double sCase = a.sCase;
        
        double b0 = A * ((A + 1.0) + (A - 1.0) * a.cosw0 + 2.0 * (sqrtA * sCase));
        double b1 = -2.0 * A * ((A - 1.0) + (A + 1.0) * a.cosw0);
        double b2 = A * (((A + 1.0) + (A - 1.0) * a.cosw0 - 2.0 * (sqrtA * sCase)));
        double a0 = (A + 1.0) - (A - 1.0) * a.cosw0 + 2.0 * (sqrtA * sCase);
        double a1 = 2.0 * ((A - 1.0) - (A + 1.0) * a.cosw0);
        double a2 = (A + 1.0) - (A - 1.0) * a.cosw0 - 2.0 * (sqrtA * sCase);
        
        normalize(b0, b1, b2, a0, a1, a2);
        
        ce.b0 = b0; ce.b1 = b1; ce.b2 = b2; ce.a1 = a1; ce.a2 = a2;
    }
    
};

//int main(int argc, const char * argv[]) {
////    AudioFilter audioFilter(8.0, 1000.0, 96000.0, .707, .5, 1.0);
////    std::cout << audioFilter.peakAndShelving() << std::endl;
//    return 0;
//}
