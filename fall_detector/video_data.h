#ifndef VIDEO_DATA_H
#define VIDEO_DATA_H

#include <boost/date_time.hpp>

namespace FallDetector
{
class VideoData
{
public:
    VideoData(boost::posix_time::ptime currentTime, double fps, float ellipseHeight);

    boost::posix_time::ptime mCurrentTime;
    double mFps;
    float mEllipseHeight;
};
}
#endif // VIDEO_DATA_H
