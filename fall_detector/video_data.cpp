#include "video_data.h"

namespace FallDetector
{
VideoData::VideoData(boost::posix_time::ptime currentTime, double fps, float ellipseHeight)
{
    mCurrentTime = currentTime;
    mFps = fps;
    mEllipseHeight = ellipseHeight;
}
}
