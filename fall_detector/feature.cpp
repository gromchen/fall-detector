#include "feature.h"

#include <cmath>

namespace FallDetector
{
Feature::Feature()
{
    mSum = 0;
    mStandardDeviation = 0;
}

void Feature::AddSummand(double summand)
{
    mSummands.push_back(summand);
    mSum += summand;
}

void Feature::CalculateStandardDeviation()
{
    unsigned int summandsSize = mSummands.size();

    double average = mSum / summandsSize;
    double meanSquareSum = 0;

    for(unsigned int iSummand = 0; iSummand < summandsSize; iSummand++)
    {
        meanSquareSum += pow(mSummands[iSummand] - average, 2);
    }

    mStandardDeviation = sqrt(meanSquareSum / summandsSize);
}
}
