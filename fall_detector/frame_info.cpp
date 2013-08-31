#include "frame_info.h"

using namespace cv;

namespace FallDetector
{
FrameInfo::FrameInfo(double coefficientOfMotion,
                     cv::RotatedRect rotatedRectangle,
                     bool objectFound)
{
    mCoefficientOfMotion = coefficientOfMotion;
    mRotatedRectangle = rotatedRectangle;
    mObjectFound = objectFound;
}
}
