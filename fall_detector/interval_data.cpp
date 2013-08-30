#include "interval_data.h"

using namespace boost;

namespace FallDetector
{
IntervalData::IntervalData(boost::posix_time::ptime currentTime, double fps, Parameters features, bool fallDetected,
                           HumanStateType state, std::vector<optional<double> > angles)
{
    mCurrentTime = currentTime;
    mFps = fps;
    mFeatures = features;
    mFallDetected = fallDetected;
    mState = state;
    mAngles = angles;
}
}
