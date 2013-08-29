#ifndef FEATURE_H
#define FEATURE_H

#include <vector>

namespace FallDetector
{
class Feature
{
public:
    Feature();

    void AddSummand(double summand);
    void CalculateAverage();
    void AddSummandToMeanSquareSum(unsigned int iSummand);
    void CalculateStandardDeviation();
    double GetStandardDeviation() { return mStandardDeviation; }

private:
    std::vector<double> mSummands;
    double mSum;
    double mAverage;
    double mMeanSquareSum;
    double mStandardDeviation;
};
}
#endif // FEATURE_H
