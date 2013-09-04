#ifndef FALL_DETECTOR_FEATURE_H
#define FALL_DETECTOR_FEATURE_H

#include <vector>

namespace FallDetector
{
class Feature
{
public:
    Feature();

    void Add(double summand);
    void CalculateStandardDeviation();
    double GetStandardDeviation() { return mStandardDeviation; }

private:
    std::vector<double> mSummands;
    double mSum;
    double mStandardDeviation;
};
}
#endif // FALL_DETECTOR_FEATURE_H
