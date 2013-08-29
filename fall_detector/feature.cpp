#include "feature.h"

#include <cmath>

namespace FallDetector
{
Feature::Feature()
{
    mSum = 0;
    mAverage = 0;
    mMeanSquareSum = 0;
    mStandardDeviation = 0;
}

void Feature::AddSummand(double summand)
{
    mSummands.push_back(summand);
    mSum += summand;
}

void Feature::CalculateAverage()
{
    mAverage = mSum / mSummands.size();
}

void Feature::AddSummandToMeanSquareSum(unsigned int iSummand)
{
    mMeanSquareSum += pow(mSummands[iSummand] - mAverage, 2);
}

void Feature::CalculateStandardDeviation()
{
    mStandardDeviation = sqrt(mMeanSquareSum / mSummands.size());
}
}
