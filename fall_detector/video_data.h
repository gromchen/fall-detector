#ifndef VIDEO_DATA_H
#define VIDEO_DATA_H

#include <boost/date_time.hpp>

namespace FallDetector
{
class VideoData
{
public:
    VideoData(boost::posix_time::ptime currentTime,
              double fps,
              double standardDeviatioinOfOrientation,
              double standardDeviationOfRatio,
              bool objectDetected);

    boost::posix_time::ptime mCurrentTime;
    double mFps;
    float mStandardDeviationOfOrientation;
    float mStandardDeviationOfRatio;
    bool mObjectDetected;
};
}
#endif // VIDEO_DATA_H
