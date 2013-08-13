#ifndef VIDEO_DATA_H
#define VIDEO_DATA_H

#include <boost/date_time.hpp>
#include <opencv2/opencv.hpp>

namespace FallDetector
{
class VideoData
{
public:
    VideoData(boost::posix_time::ptime currentTime,
              double fps,
              double standardDeviatioinOfOrientation,
              cv::RotatedRect rotatedRectangle);

    boost::posix_time::ptime mCurrentTime;
    double mFps;
    float mStandardDeviationOfOrientation;
    float mRatio;
};
}
#endif // VIDEO_DATA_H
