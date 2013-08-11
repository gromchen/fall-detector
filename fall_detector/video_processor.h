#ifndef FALL_DETECTOR_VIDEO_PROCESSOR_H
#define FALL_DETECTOR_VIDEO_PROCESSOR_H

#include <vector>
#include <boost/thread.hpp>
#include <opencv2/opencv.hpp>

#include "helpers.h"
#include "video_data.h"

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
    void collectData();
    void writeToFile();
    void clearStop();
    void log(std::string fileName, std::vector<VideoData> data);

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

    int mErodeElementSize;
    int mErodeIterations;
    int mDilateElementSize;
    int mDilateIterations;

    double mFps;

    int mResolutionWidth;
    int mResolutionHeight;

    std::vector<boost::thread*> mWriteProcesses;
    boost::mutex mFileMutex;

    cv::RotatedRect mEllipse;
    bool mHasEllipse;
    std::vector<VideoData> mVideoDataCollection;
};
}

#endif // FALL_DETECTOR_VIDEO_PROCESSOR_H
