#include "interval_info.h"

using namespace boost;

namespace FallDetector
{
IntervalInfo::IntervalInfo(boost::posix_time::ptime currentTime, double fps, FeatureCollection featuresCollection,
                           HumanState humanState, std::vector<optional<double> > angles)
{
    mCurrentTime = currentTime;
    mFps = fps;
    mFeatureCollection = featuresCollection;
    mHumanState = humanState;
    mAngles = angles;
}
}
