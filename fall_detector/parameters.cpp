#include "parameters.h"

namespace FallDetector
{
Parameters::Parameters()
{
    for(unsigned int i_feature = 0; i_feature < mcsNumberOfFeatures; i_feature++)
    {
        mFeatures.push_back(Feature());
    }

    mNumberOfSummands = 0;
}

void Parameters::AddSummands(double coefficientOfMotion, double orientation, double ratio, double positionX,
                             double positionY, double axisA, double axisB)
{
    mFeatures[0].AddSummand(coefficientOfMotion);
    mFeatures[1].AddSummand(orientation);
    mFeatures[2].AddSummand(ratio);
    mFeatures[3].AddSummand(positionX);
    mFeatures[4].AddSummand(positionY);
    mFeatures[5].AddSummand(axisA);
    mFeatures[6].AddSummand(axisB);

    mNumberOfSummands++;
}

void Parameters::CalculateStandardDeviation()
{
    for(unsigned int iFeature = 0; iFeature < mcsNumberOfFeatures; iFeature++)
    {
        mFeatures[iFeature].CalculateStandardDeviation();
    }
}
}
