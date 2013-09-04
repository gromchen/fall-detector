#ifndef FALL_DETECTOR_INTERVAL_INFO_H
#define FALL_DETECTOR_INTERVAL_INFO_H

#include <boost/date_time.hpp>
#include <boost/optional/optional.hpp>

#include "helpers.h"
#include "feature_collection.h"

namespace FallDetector
{
class IntervalInfo
{
public:
    IntervalInfo(boost::posix_time::ptime currentTime, double fps, FeatureCollection featureCollection,
                 HumanState humanState, std::vector<boost::optional<double> > angles);

    boost::posix_time::ptime GetCurrentTime() { return mCurrentTime; }
    double GetFps() { return mFps; }
    FeatureCollection GetFeatureCollection() { return mFeatureCollection; }
    int GetNumberOfFoundObjects() { return mFeatureCollection.GetNumberOfSummands(); }
    HumanState GetHumanState() { return mHumanState; }
    std::vector<boost::optional<double> > GetAngles() { return mAngles; }

private:
    boost::posix_time::ptime mCurrentTime;
    double mFps;
    FeatureCollection mFeatureCollection;
    HumanState mHumanState;
    std::vector<boost::optional<double> > mAngles;
};
}
#endif // FALL_DETECTOR_INTERVAL_INFO_H
