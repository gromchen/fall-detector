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
    std::string GetResolution();

    void SetCamera(unsigned int camera_number) { mCameraNumber = camera_number; }
    unsigned int GetCamera() { return mCameraNumber; }

    void CreateNewBackgroundSubtractor(int history, float threashold);
    int GetHistory() { return mHistory; }
    float GetThreshold() { return mThreshold; }

private:
    void ProcessFrame();
    double CalculateCoefficientOfMotion(cv::Mat &silhouette, cv::Mat &mhiMask);

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

    bool mShowAdditionalGui;

    cv::Mat erode_element;
    int mErodeElementSize;
    int mErodeIterations;

    cv::Mat dilate_element;
    int mDilateElementSize;
    int mDilateIterations;

    int mFrameWidth;
    int mFrameHeight;

    cv::RotatedRect mRotatedRectangle;
    bool mObjectFound;
    static const unsigned int mcMaxNumberOfObjects = 50;
    static const unsigned int mcMinAreaOfObject = 40*40;
    int mMaxAreaOfObject;

    IntervalProcessor mIntervalProcessor;
};
}

#endif // FALL_DETECTOR_VIDEO_PROCESSOR_H
