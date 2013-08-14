#ifndef FRAME_DATA_H
#define FRAME_DATA_H

#include <opencv2/opencv.hpp>

namespace FallDetector
{
class FrameData
{
public:
    FrameData(cv::RotatedRect object, bool objectFound);

    bool ObjectFound() { return mObjectFound; }
    cv::RotatedRect GetObject() { return mObject; }

private:
    cv::RotatedRect mObject;
    bool mObjectFound;
};
}

#endif // FRAME_DATA_H
