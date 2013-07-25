#ifndef FALL_DETECTOR_VIDEO_PROCESSOR_H
#define FALL_DETECTOR_VIDEO_PROCESSOR_H

#include <mutex>

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
        /// Sets the callback function that will be called for each frame
        /// </summary>
        /// <param name="frameProcessingCallback">Frame processing callback</param>
        void SetFrameProcessor(void (*frameProcessingCallback)(const cv::Mat&, cv::Mat&));

        /// <summary>
        /// Sets key value which is processed as input
        /// </summary>
        /// <param name="key">Key</param>
        void SetKey(char key);

        /// <summary>
        /// Sets resolution of the frame
        /// </summary>
        /// <param name="width">Frame width</param>
        /// <param name="height">Frame height</param>
        void SetResolution(int width, int height);

        /// <summary>
        /// Gets height of bounding rectangle
        /// </summary>
        /// <returns>Height</returns>
        int GetHeight();

        /// <summary>
        /// Grabs and processes the frames of the sequence
        /// </summary>
        void Run();

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
        /// Updates user input
        /// </summary>
        void updateInput();

        /// <summary>
        /// Updates UI
        /// </summary>
        void updateUI();

        /// <summary>
        /// OpenCV video capture object
        /// </summary>
        cv::VideoCapture m_videoCapture;

        /// <summary>
        /// Callback function to be called for the processing of each frame
        /// </summary>
        void (*process)(const cv::Mat&, cv::Mat&);

        /// <summary>
        /// Determine if the process callback will be called
        /// </summary>
        bool _callProcessCallback;

        /// <summary>
        /// Input display window name
        /// </summary>
        std::string _nameOfInputWindow;

        /// <summary>
        /// Output display window name
        /// </summary>
        std::string _nameOfOutputWindow;

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
        bool m_stop;

        /// <summary>
        /// Height of bounding rectangle
        /// </summary>
        int _heightOfRectangle;

        /// <summary>
        /// Pressed key
        /// </summary>
        char m_key;

        /// <summary>
        /// Determine whether the user interface is shown
        /// </summary>
        bool m_showUI;

        /// <summary>
        /// Frames per second
        /// </summary>
        double _fps;

        /// <summary>
        /// Protects input
        /// </summary>
        std::mutex m_mutexInput;

        /// <summary>
        /// Protects output
        /// </summary>
        //std::mutex _mutexOutput;

        /// <summary>
        /// Protects video capture
        /// </summary>
        std::mutex  m_mutexForVideoCapture;
    };
}

#endif // !FALL_DETECTOR_VIDEO_PROCESSOR_H
