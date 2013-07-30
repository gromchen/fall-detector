#ifndef FALL_DETECTOR_VIDEO_PROCESSOR_H
#define FALL_DETECTOR_VIDEO_PROCESSOR_H

#include <mutex>
#include <atomic>

#include <opencv2/opencv.hpp>

namespace FallDetector
{
    /// <summary>
    /// Controls different aspects of the video frame processing
    /// </summary>
    class VideoProcessor
    {
    public:
        /// <summary>
        /// Default constructor
        /// </summary>
        VideoProcessor();

        /// <summary>
        /// Destructor
        /// </summary>
        ~VideoProcessor();

        /// <summary>
        /// Sets resolution of the frame
        /// </summary>
        /// <param name="width">Frame width</param>
        /// <param name="height">Frame height</param>
        void SetResolution(int width, int height);

        /// <summary>
        /// Shows or hides GUI depending on current state
        /// </summary>
        void ShowHideGui();

        /// <summary>
        /// Gets height of bounding rectangle
        /// </summary>
        /// <returns>Height</returns>
        int GetHeight(); // Obtain graph

        /// <summary>
        /// Grabs and processes the frames of the sequence
        /// </summary>
        void Run();

        /// <summary>
        /// Stops run
        /// </summary>
        void Stop();

    private:
        /// <summary>
        /// Start display frames
        /// </summary>
        void startDisplayUI();

        /// <summary>
        /// Stop display frames
        /// </summary>
        void stopDisplayUI();

        /// <summary>
        /// OpenCV video capture object
        /// </summary>
        cv::VideoCapture mVideoCapture;

        /// <summary>
        /// Input display window name
        /// </summary>
        std::string m_nameOfInputWindow;

        /// <summary>
        /// Output display window name
        /// </summary>
        std::string m_nameOfOutputWindow;

        /// <summary>
        /// Original frame from camera
        /// </summary>
        cv::Mat _originalFrame;

        /// <summary>
        /// One channel frame
        /// </summary>
        cv::Mat _grayFrame;

        /// <summary>
        /// Frame with extracted foreground
        /// </summary>
        cv::Mat _frameWithForeground;

        /// <summary>
        /// Stop the processing
        /// </summary>
        std::atomic<bool> m_stop;

        /// <summary>
        /// Height of bounding rectangle
        /// </summary>
        int _heightOfRectangle;

        /// <summary>
        /// Determine whether the user interface is shown
        /// </summary>
        std::atomic<bool> m_showUI;

        /// <summary>
        /// Determine whether windows of GUI were created
        /// </summary>
        bool m_windowsAreCreated;

        /// <summary>
        /// Frames per second
        /// </summary>
        double _fps;

        /// <summary>
        /// Protects video capture
        /// </summary>
        std::mutex  m_mutexForVideoCapture;

        std::atomic<bool> mChangeResolution;
        int mWidth;
        int mHeight;
    };
}

#endif // !FALL_DETECTOR_VIDEO_PROCESSOR_H
