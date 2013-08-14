#include "interval_data.h"

namespace FallDetector
{
IntervalData::IntervalData(boost::posix_time::ptime currentTime,
                     double fps,
                     double standardDeviatioinOfOrientation,
                     double standardDeviationOfRatio,
                     int numberOfFoundObjects)
{
    mCurrentTime = currentTime;
    mFps = fps;
    mStandardDeviationOfOrientation = standardDeviatioinOfOrientation;
    mStandardDeviationOfRatio = standardDeviationOfRatio;
    mNumberOfFoundObjects = numberOfFoundObjects;
}
}
