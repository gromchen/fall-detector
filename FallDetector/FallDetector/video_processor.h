#ifndef FALL_DETECTOR_VIDEO_PROCESSOR_H
#define FALL_DETECTOR_VIDEO_PROCESSOR_H

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
        /// <returns>Nothing</returns>
        void SetFrameProcessor(void (*frameProcessingCallback)(const cv::Mat&, cv::Mat&));

        /// <summary>
        /// Sets key value which is processed as input
        /// </summary>
        /// <param name="key">Key</param>
        /// <returns>Nothing</returns>
        void SetKey(char key);

        /// <summary>
        /// Gets height of bounding rectangle
        /// </summary>
        /// <returns>Height</returns>
        int GetHeight();

        /// <summary>
        /// Grabs and processes the frames of the sequence
        /// </summary>
        /// <returns>Nothing</returns>
        void Run();

    private:
        /// <summary>
        /// Start display frames
        /// </summary>
        /// <returns>Nothing</returns>
        void startDisplayUI();

        /// <summary>
        /// Stop display frames
        /// </summary>
        /// <returns>Nothing</returns>
        void stopDisplayUI();

        /// <summary>
        /// Updates user input
        /// </summary>
        /// <returns>Nothing</returns>
        void updateInput();

        /// <summary>
        /// Updates UI
        /// </summary>
        /// <returns>Nothing</returns>
        void updateUI();

        /// <summary>
        /// OpenCV video capture object
        /// </summary>
        cv::VideoCapture _videoCapture;

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
        bool _stop;

        /// <summary>
        /// Height of bounding rectangle
        /// </summary>
        int _heightOfRectangle;

        /// <summary>
        /// Pressed key
        /// </summary>
        char _key;

        /// <summary>
        /// Determine whether the user interface is shown
        /// </summary>
        bool _showUI;

        /// <summary>
        /// Frames per second
        /// </summary>
        double _fps;

        /// <summary>
        /// Protects input
        /// </summary>
        //std::mutex _mutexInput;

        /// <summary>
        /// Protects output
        /// </summary>
        //std::mutex _mutexOutput;
    };
}

#endif // !FALL_DETECTOR_VIDEO_PROCESSOR_H
