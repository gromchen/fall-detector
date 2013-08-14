#include "video_data.h"

namespace FallDetector
{
VideoData::VideoData(boost::posix_time::ptime currentTime,
                     double fps,
                     double standardDeviatioinOfOrientation,
                     double standardDeviationOfRatio,
                     bool objectDetected)
{
    mCurrentTime = currentTime;
    mFps = fps;
    mStandardDeviationOfOrientation = standardDeviatioinOfOrientation;
    mStandardDeviationOfRatio = standardDeviationOfRatio;
    mObjectDetected = objectDetected;
}
}
