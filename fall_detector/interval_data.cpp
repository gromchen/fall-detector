#include "interval_data.h"

namespace FallDetector
{
IntervalData::IntervalData(boost::posix_time::ptime currentTime, double fps, Parameters features, bool fallDetected,
                           HumanStateType state)
{
    mCurrentTime = currentTime;
    mFps = fps;
    mFeatures = features;
    mFallDetected = fallDetected;
    mState = state;
}
}
