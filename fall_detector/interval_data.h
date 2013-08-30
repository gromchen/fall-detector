#ifndef VIDEO_DATA_H
#define VIDEO_DATA_H

#include <boost/date_time.hpp>

#include "helpers.h"
#include "features.h"

namespace FallDetector
{
class IntervalData
{
public:
    IntervalData(boost::posix_time::ptime currentTime, double fps, Features features, bool fallDetected,
                 HumanStateType state);

    boost::posix_time::ptime GetCurrentTime() { return mCurrentTime; }
    double GetFps() { return mFps; }
    Features GetFeatures() { return mFeatures; }
    int GetNumberOfFoundObjects() { return mFeatures.GetNumberOfSummands(); }
    bool FallDetected() { return mFallDetected; }
    HumanStateType GetState() { return mState; }

private:
    boost::posix_time::ptime mCurrentTime;
    double mFps;
    Features mFeatures;
    bool mFallDetected;
    HumanStateType mState;
};
}
#endif // VIDEO_DATA_H
