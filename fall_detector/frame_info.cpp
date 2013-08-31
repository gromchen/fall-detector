#include "frame_info.h"

using namespace cv;

namespace FallDetector
{
FrameInfo::FrameInfo(double coefficientOfMotion, boost::optional<RotatedRect> optionalRotatedRectangle)
{
    mCoefficientOfMotion = coefficientOfMotion;
    mOptionalRotatedRectangle = optionalRotatedRectangle;
}
}
