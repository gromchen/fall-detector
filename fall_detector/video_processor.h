#ifndef FALL_DETECTOR_VIDEO_PROCESSOR_H
#define FALL_DETECTOR_VIDEO_PROCESSOR_H

#include <opencv2/opencv.hpp>

namespace FallDetector
{
class VideoProcessor
{
public:
    VideoProcessor();
    ~VideoProcessor();

    void RunWithoutGui();
    void RunWithGui();

    void SetResolution(int width, int height);
    std::string PrintResolution();

private:
    void processFrame();
    int log();

    cv::VideoCapture mVideoCapture;
    cv::BackgroundSubtractorMOG2 mBackgroundSubtractor;

    cv::Mat mOriginalFrame;
    cv::Mat mForegroundMask;
    cv::Mat mThresholdMask;
    cv::Mat mMorphologicalMask;
    //cv::Mat mErodeMask;
    //cv::Mat mDilateMask;
    cv::Mat mContoursMask;

    double mFps;

    int mResolutionWidth;
    int mResolutionHeight;
};
}

#endif // FALL_DETECTOR_VIDEO_PROCESSOR_H
