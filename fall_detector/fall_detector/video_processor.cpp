#include "video_processor.h"

#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace cv;

namespace FallDetector
{
    VideoProcessor::VideoProcessor()
        : _callProcessCallback(true)
    {
        if (!m_videoCapture.open(0))
        {
            throw runtime_error("Could not open video device");
        }

        m_videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
        m_videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

        m_stop = false;
        m_key = 0;
        m_showUI = false;

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
        unique_lock<mutex> lck(m_mutexInput);
        m_key = key;
        lck.unlock();
    }

    void VideoProcessor::SetResolution(int width, int height)
    {
        unique_lock<mutex> lockForVideoCapture(m_mutexForVideoCapture);
        m_videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, width);
        m_videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, height);
        lockForVideoCapture.unlock();
    }

    int VideoProcessor::GetHeight()
    {
        return _heightOfRectangle;
    }

    void VideoProcessor::Run()
    {
        if (!m_videoCapture.isOpened())
        {
            throw runtime_error("Video device is not opened.");
        }

        BackgroundSubtractorMOG2 bg_subtractor(1000, 15);
        vector<Vec4i> hierarchy;
        vector<vector<Point> > contours;

        while (!m_stop)
        {
            auto timeOfProcessingStart = high_resolution_clock::now();

            updateInput();

            unique_lock<mutex> lockForVideoCapture(m_mutexForVideoCapture);
            bool frameIsRead = m_videoCapture.read(_originalFrame);
            lockForVideoCapture.unlock();

            if (!frameIsRead)
            {
                throw runtime_error("Could not read new frame");
            }

            cvtColor(_originalFrame, _grayFrame, CV_BGR2GRAY); // TODO: bilateral smoothing
            //GaussianBlur(gray, blur, Size(5, 5), 0);
            //Canny(blur, canny, 10, hey);

            bg_subtractor(_grayFrame, _frameWithForeground, 0.01);
            //erode(foreground, foreground, Mat());
            //dilate(foreground, foreground, Mat());


            findContours(_frameWithForeground, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

            unsigned int cmin = 100;
            unsigned int cmax = 1000;

            auto itc = contours.begin();
            vector<Point> pts;
            while (itc != contours.end())
            {
                if (itc->size() > cmin && itc->size() < cmax)
                {
                    for (auto pnt : *itc)
                    {
                        pts.push_back(pnt);
                    }

                    ++itc;
                }
                else
                {
                    ++itc;
                }
            }

            if (pts.size() != 0)
            {
                Mat pointsMatrix = Mat(pts);
                Scalar color(0, 255, 0);

                Rect boundingRectangle = boundingRect(pointsMatrix);

                _heightOfRectangle = boundingRectangle.height;

                rectangle(_originalFrame, boundingRectangle, color, 2);
            }

            updateUI();

            if (m_showUI)
            {
                waitKey(25);
            }

            _fps = 1000.0 / duration_cast<milliseconds>(high_resolution_clock::now() - timeOfProcessingStart).count();
        }
    }

    void VideoProcessor::startDisplayUI()
    {
        m_showUI = true;
        namedWindow(_nameOfInputWindow, CV_WINDOW_AUTOSIZE);
        namedWindow(_nameOfOutputWindow, CV_WINDOW_AUTOSIZE);
    }

    void VideoProcessor::stopDisplayUI()
    {
        m_showUI = false;
        destroyWindow(_nameOfInputWindow);
        destroyWindow(_nameOfOutputWindow);
    }

    void VideoProcessor::updateInput()
    {
        unique_lock<mutex> lck(m_mutexInput);

        switch (m_key)
        {
        case 'q':
            m_stop = true;
            break;
        case 'i':
            if(m_showUI)
                stopDisplayUI();
            else
                startDisplayUI();
            break;
        case 'r':

            break;
        default:
            break;
        }
        m_key = 0;

        lck.unlock();
    }

    void VideoProcessor::updateUI()
    {
        if (m_showUI)
        {
            imshow(_nameOfInputWindow, _originalFrame);
            //imshow(processed_video, gray);
            //imshow(blurred_video, blur);
            //imshow(canny_video, canny);
            imshow(_nameOfOutputWindow, _frameWithForeground);
        }
    }
}