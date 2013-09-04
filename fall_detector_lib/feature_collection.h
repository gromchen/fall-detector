#ifndef FALL_DETECTOR_FEATURE_COLLECTION_H
#define FALL_DETECTOR_FEATURE_COLLECTION_H

#include "feature.h"

namespace FallDetector
{
class FeatureCollection
{
public:
    FeatureCollection();

    void Add(double coefficientOfMotion, double orientation, double ratio, double positionX, double positionY,
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

#endif // FALL_DETECTOR_FEATURE_COLLECTION_H
