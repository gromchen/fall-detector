#include "frame_data.h"

using namespace cv;

namespace FallDetector
{
FrameData::FrameData(double coefficientOfMotion,
                     cv::RotatedRect object,
                     bool objectFound)
{
    mCoefficientOfMotion = coefficientOfMotion;
    mObject = object;
    mObjectFound = objectFound;
}
}
