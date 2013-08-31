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
