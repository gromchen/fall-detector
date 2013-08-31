#ifndef FALL_DETECTOR_FRAME_INFO_H
#define FALL_DETECTOR_FRAME_INFO_H

#include <opencv2/opencv.hpp>

namespace FallDetector
{
class FrameInfo
{
public:
    FrameInfo(double coefficientOfMotion, cv::RotatedRect rotatedRectangle, bool objectFound);

    double GetCoefficientOfMotion() { return mCoefficientOfMotion; }
    bool IsObjectFound() { return mObjectFound; }
    cv::RotatedRect GetRotatedRectangle() { return mRotatedRectangle; }

private:
    double mCoefficientOfMotion;
    cv::RotatedRect mRotatedRectangle;
    bool mObjectFound;
};
}

#endif // FALL_DETECTOR_FRAME_INFO_H
