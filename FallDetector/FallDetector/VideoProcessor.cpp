#include "VideoProcessor.h"

#include <chrono>

//#ifdef _WIN32
//#include <conio.h>
//#endif
//
//#ifdef __linux__
//#include <curses.h>
//#endif


#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>

using namespace std;
using namespace std::chrono;
using namespace cv;

namespace FallDetector
{
    VideoProcessor::VideoProcessor()
        : _callProcessCallback(true)
    {
        if (!_videoCapture.open(0))
        {
            throw runtime_error{ "Could not open video device" };
        }

        _videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
        _videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

        _stop = false;
        _key = 0;
        _showUI = false;

        _nameOfInputWindow = "Original Video";
        _nameOfOutputWindow = "Output Video";
    }

    VideoProcessor::~VideoProcessor()
    {
    }

    void VideoProcessor::SetFrameProcessor(void (*frameProcessingCallback)(const Mat&, Mat&))
    {
        process = frameProcessingCallback;
    }

    void VideoProcessor::SetKey(char key)
    {
        unique_lock<mutex> lck { _mutexInput };
        _key = key;
        //_conditionInput.notify_one();
        lck.unlock();
    }

    void VideoProcessor::Run()
    {
        if (!_videoCapture.isOpened())
        {
            throw runtime_error{ "Video device is not opened." };
        }
        
        BackgroundSubtractorMOG2 bg_subtractor(1000, 15);
        vector<vector<Point>> contours;
        _stop = false;

        while (!_stop)
        {
            auto timeOfProcessingStart = high_resolution_clock::now();

            updateInput();

            if (!_videoCapture.read(_originalFrame))
            {
                throw runtime_error{ "Could not read new frame" };
            }

            cvtColor(_originalFrame, _grayFrame, CV_BGR2GRAY);
            //GaussianBlur(gray, blur, Size(5, 5), 0);
            //Canny(blur, canny, 10, hey);

            bg_subtractor(_grayFrame, _frameWithForeground, 0.01);
            //erode(foreground, foreground, Mat());
            //dilate(foreground, foreground, Mat());

            findContours(_frameWithForeground, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

            unsigned int cmin = 50;
            unsigned int cmax = 1000;

            vector<vector<Point>>::iterator itc = contours.begin();

            while (itc != contours.end())
            {
                if (itc->size() > cmin && itc->size() < cmax)
                {
                    vector<Point> pts = *itc;
                    Mat pointsMatrix = Mat(pts);
                    Scalar color(0, 255, 0);

                    Rect boundingRectangle = boundingRect(pointsMatrix);
                    _heightOfRectangle = boundingRectangle.height;
                    rectangle(_originalFrame, boundingRectangle, color, 2);

                    ++itc;
                }
                else
                {
                    ++itc;
                }
            }

            updateUI();
            waitKey(1);
            _fps = 1000.0 / duration_cast<milliseconds>(high_resolution_clock::now() - timeOfProcessingStart).count();
        }
    }

    void VideoProcessor::startDisplayUI()
    {
        _showUI = true;
        namedWindow(_nameOfInputWindow, CV_WINDOW_AUTOSIZE);
        namedWindow(_nameOfOutputWindow, CV_WINDOW_AUTOSIZE);
    }

    void VideoProcessor::stopDisplayUI()
    {
        _showUI = false;
        destroyWindow(_nameOfInputWindow);
        destroyWindow(_nameOfOutputWindow);
    }

    void VideoProcessor::updateInput()
    {
        unique_lock<mutex> lck{ _mutexInput };
        //_conditionInput.wait(lck);

        switch (_key)
        {
        case 'q':
            _stop = true;
            _key = 0;
            break;
        case 's':
            startDisplayUI();
            _key = 0;
            break;
        case 'd':
            stopDisplayUI();
            _key = 0;
            break;
        default:
            break;
        }

        //_key = 0;
        lck.unlock();
    }

    void VideoProcessor::updateUI()
    {
        if (_showUI)
        {
            imshow(_nameOfInputWindow, _originalFrame);
            //imshow(processed_video, gray);
            //imshow(blurred_video, blur);
            //imshow(canny_video, canny);
            imshow(_nameOfOutputWindow, _frameWithForeground);
        }
    }
}