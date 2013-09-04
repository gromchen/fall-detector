#ifndef FALL_DETECTOR_FRAME_INFO_H
#define FALL_DETECTOR_FRAME_INFO_H

#include <boost/optional/optional.hpp>
#include <opencv2/opencv.hpp>

namespace FallDetector
{
class FrameInfo
{
public:
    FrameInfo(double coefficientOfMotion, boost::optional<cv::RotatedRect> optionalRotatedRectangle);

    double GetCoefficientOfMotion() { return mCoefficientOfMotion; }
    boost::optional<cv::RotatedRect> GetOptionalRotatedRectangle() { return mOptionalRotatedRectangle; }

private:
    double mCoefficientOfMotion;
    boost::optional<cv::RotatedRect> mOptionalRotatedRectangle;
};
}

#endif // FALL_DETECTOR_FRAME_INFO_H
