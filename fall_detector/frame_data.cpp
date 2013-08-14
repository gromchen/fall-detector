#include "frame_data.h"

using namespace cv;

namespace FallDetector
{
FrameData::FrameData(cv::RotatedRect object, bool objectFound)
{
    mObject = object;
    mObjectFound = objectFound;
}
}
