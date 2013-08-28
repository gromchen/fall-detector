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

    void SetCamera(unsigned int camera_number) { mCameraNumber = camera_number; }
    unsigned int GetCamera() { return mCameraNumber; }

    void CreateNewBackgroundSubtractor(int history, float threashold);
    int GetHistory() { return mHistory; }
    float GetThreshold() { return mThreshold; }

private:
    void processFrame();
    double calculateCoefficientOfMotion(cv::Mat &silhouette, cv::Mat &mhiMask);
    void clearStop();

    cv::VideoCapture mVideoCapture;
    unsigned int mCameraNumber;

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
