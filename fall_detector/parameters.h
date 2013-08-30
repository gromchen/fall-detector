#ifndef FEATURES_H
#define FEATURES_H

#include "feature.h"

namespace FallDetector
{
class Parameters
{
public:
    Parameters();

    void AddSummands(double coefficientOfMotion, double orientation, double ratio, double positionX, double positionY,
                     double axisA, double axisB);
    unsigned int GetNumberOfSummands() { return mNumberOfSummands; }

    void CalculateStandardDeviation();

    Feature GetCoefficientOfMotion() {return mFeatures[0]; }
    Feature GetOrientation() { return mFeatures[1]; }
    Feature GetRatio() { return mFeatures[2]; }
    Feature GetPositionX() { return mFeatures[3]; }
    Feature GetPositionY() { return mFeatures[4]; }
    Feature GetAxisA() { return mFeatures[5]; }
    Feature GetAxisB() { return mFeatures[6]; }

private:
    const static unsigned int mcsNumberOfFeatures = 7;
    std::vector<Feature> mFeatures;
    unsigned int mNumberOfSummands;
};
}

#endif // FEATURES_H
