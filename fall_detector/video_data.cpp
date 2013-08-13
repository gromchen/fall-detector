#include "video_data.h"

namespace FallDetector
{
VideoData::VideoData(boost::posix_time::ptime currentTime,
                     double fps,
                     double standardDeviatioinOfOrientation,
                     cv::RotatedRect rotatedRectangle)
{
    mCurrentTime = currentTime;
    mFps = fps;
    mStandardDeviationOfOrientation = standardDeviatioinOfOrientation;
    mRatio = rotatedRectangle.size.height / rotatedRectangle.size.width;
}
}
