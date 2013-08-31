#include "interval_info.h"

using namespace boost;

namespace FallDetector
{
IntervalInfo::IntervalInfo(boost::posix_time::ptime currentTime, double fps, FeatureCollection featuresCollection,
                           bool fallDetected, HumanState humanState, std::vector<optional<double> > angles)
{
    mCurrentTime = currentTime;
    mFps = fps;
    mFeatureCollection = featuresCollection;
    mFallDetected = fallDetected;
    mHumanState = humanState;
    mAngles = angles;
}
}
