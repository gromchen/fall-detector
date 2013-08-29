#include "interval_data.h"

namespace FallDetector
{
IntervalData::IntervalData(boost::posix_time::ptime currentTime, double fps, Features features)
{
    mCurrentTime = currentTime;
    mFps = fps;
    mFeatures = features;
}
}
