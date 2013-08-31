#include "feature.h"

#include <cmath>

namespace FallDetector
{
Feature::Feature()
{
    mSum = 0;
    mStandardDeviation = 0;
}

void Feature::Add(double summand)
{
    mSummands.push_back(summand);
    mSum += summand;
}

void Feature::CalculateStandardDeviation()
{
    unsigned int size_of_summands = mSummands.size();

    double average = mSum / size_of_summands;
    double mean_square_sum = 0;

    for(unsigned int i_summand = 0; i_summand < size_of_summands; i_summand++)
    {
        mean_square_sum += pow(mSummands[i_summand] - average, 2);
    }

    mStandardDeviation = sqrt(mean_square_sum / size_of_summands);
}
}
