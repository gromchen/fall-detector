#ifndef FALL_DETECTOR_VIDEO_PROCESSOR_H
#define FALL_DETECTOR_VIDEO_PROCESSOR_H

#include "interval_processor.h"

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

    void CreateNewBackgroundSubtractor(int history, float threashold);
    int GetHistory();
    float GetThreshold();

private:
    void processFrame();
    void clearStop();

    cv::VideoCapture mVideoCapture;

    cv::BackgroundSubtractorMOG2* mpBackgroundSubtractor;
    int mHistory;
    float mThreshold;

    cv::Mat mOriginalFrame;
    cv::Mat mForegroundMask;
    cv::Mat mThresholdMask;
    cv::Mat mErodeMask;
    cv::Mat mDilateMask;
    cv::Mat mContoursMask;

    cv::Mat mMhiMask;

    int mErodeElementSize;
    int mErodeIterations;
    int mDilateElementSize;
    int mDilateIterations;

    int mFrameWidth;
    int mFrameHeight;

    cv::RotatedRect mEllipse;
    bool mObjectFound;
    static const unsigned int mcMaxNumberOfObjects = 50;
    static const unsigned int mcMinAreaOfObject = 20*20;
    int mMaxAreaOfObject;

    FallDetector::IntervalProcessor mIntervalProcessor;
};
}

#endif // FALL_DETECTOR_VIDEO_PROCESSOR_H
