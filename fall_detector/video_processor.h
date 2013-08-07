#ifndef FALL_DETECTOR_VIDEO_PROCESSOR_H
#define FALL_DETECTOR_VIDEO_PROCESSOR_H

#include <boost/thread/mutex.hpp>
#include <boost/atomic.hpp>

#include <opencv2/opencv.hpp>

namespace FallDetector
{
    class VideoProcessor
    {
    public:
        VideoProcessor();
        ~VideoProcessor();

        void SetResolution(int width, int height);
        void ShowHideGui();
        int GetHeight(); // TODO: Obtain graph
        void Run();
        void Stop();

    private:
        void startDisplayUI();
        void stopDisplayUI();

        cv::VideoCapture mVideoCapture;

        std::string m_nameOfInputWindow;
        std::string mNameOfBlurredWindow;
        std::string m_nameOfOutputWindow;

        cv::Mat _originalFrame;
        cv::Mat _grayFrame;
        cv::Mat _frameWithForeground;

        boost::atomic<bool> m_stop;
        boost::atomic<bool> m_showUI;
        boost::atomic<bool> mChangeResolution;
        boost::mutex  m_mutexForVideoCapture;

        int _heightOfRectangle;

        bool m_windowsAreCreated;
        double _fps;

        int mWidth;
        int mHeight;
    };
}

#endif // FALL_DETECTOR_VIDEO_PROCESSOR_H
