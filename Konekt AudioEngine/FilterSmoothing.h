#include <iostream> 
#include "AudioFilters.h"

class SmoothedParameter
{
private:
    double currentValue = 0.0;
    double targetValue = 0.0;
    double alpha = 0.0;

public:

    void init(AudioFilter& af)
    {
        const double updateRate = af.samplesRate / 
    }
};