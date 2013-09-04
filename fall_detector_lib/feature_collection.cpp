#include "feature_collection.h"

namespace FallDetector
{
FeatureCollection::FeatureCollection()
{
    for(unsigned int i_feature = 0; i_feature < mcsNumberOfFeatures; i_feature++)
    {
        mFeatures.push_back(Feature());
    }

    mNumberOfSummands = 0;
}

void FeatureCollection::Add(double coefficientOfMotion, double orientation, double ratio, double positionX,
                            double positionY, double axisA, double axisB)
{
    mFeatures[0].Add(coefficientOfMotion);
    mFeatures[1].Add(orientation);
    mFeatures[2].Add(ratio);
    mFeatures[3].Add(positionX);
    mFeatures[4].Add(positionY);
    mFeatures[5].Add(axisA);
    mFeatures[6].Add(axisB);

    mNumberOfSummands++;
}

void FeatureCollection::CalculateStandardDeviation()
{
    for(unsigned int i_feature = 0; i_feature < mcsNumberOfFeatures; i_feature++)
    {
        mFeatures[i_feature].CalculateStandardDeviation();
    }
}
}
