#ifndef FALL_DETECTOR_VIDEO_PROCESSOR_H
#define FALL_DETECTOR_VIDEO_PROCESSOR_H

#include <vector>
#include <boost/thread.hpp>
#include <opencv2/opencv.hpp>

#include "helpers.h"

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

    void CreateNewBackgroundSubtractor(int history);
    int GetHistory();

private:
    void processFrame();
    void log(std::string fileName, std::vector<VideoData> data);
    void clearStop();

    cv::VideoCapture mVideoCapture;

    cv::BackgroundSubtractorMOG2* mpBackgroundSubtractor;
    int mHistory;
    //int mThreshold;
    //int mThresholdGen;

    cv::Mat mOriginalFrame;
    cv::Mat mForegroundMask;
    cv::Mat mThresholdMask;
    cv::Mat mErodeMask;
    cv::Mat mDilateMask;
    cv::Mat mContoursMask;

    int mDilateElementSize;
    int mDilateIterations;

    double mFps;

    int mResolutionWidth;
    int mResolutionHeight;

    std::vector<boost::thread*> mWriteProcesses;
    boost::mutex mFileMutex;
    std::vector<VideoData> mVideoDataCollection;
};
}

#endif // FALL_DETECTOR_VIDEO_PROCESSOR_H
