#ifndef FRAME_DATA_H
#define FRAME_DATA_H

#include <opencv2/opencv.hpp>

namespace FallDetector
{
class FrameData
{
public:
    FrameData(double coefficientOfMotion,
              cv::RotatedRect object,
              bool objectFound);

    double GetCoefficientOfMotion() { return mCoefficientOfMotion; }
    bool ObjectFound() { return mObjectFound; }
    cv::RotatedRect GetObject() { return mObject; }

private:
    double mCoefficientOfMotion;
    cv::RotatedRect mObject;
    bool mObjectFound;
};
}

#endif // FRAME_DATA_H
