#include "video_processor.h"

#include <boost/chrono.hpp>

using namespace std;
using namespace boost;
using namespace boost::chrono;
using namespace cv;

namespace FallDetector
{
    VideoProcessor::VideoProcessor()
    {
        if (!mVideoCapture.open(0))
        {
            throw runtime_error("Could not open video device");
        }

        mVideoCapture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
        mVideoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

        //mVideoCapture.set(CV_CAP_PROP_FOCUS, 0);
        //double focus = mVideoCapture.get(CV_CAP_PROP_FOCUS);

        m_stop = false;
        m_showUI = false;
        m_windowsAreCreated = false;
        mChangeResolution = false;

        m_nameOfInputWindow = "Original Video";
        m_nameOfOutputWindow = "Output Video";
    }

    VideoProcessor::~VideoProcessor()
    {
    }

    void VideoProcessor::SetResolution(int width, int height)
    {
        unique_lock<mutex> lockForVideoCapture(m_mutexForVideoCapture);
        mChangeResolution = true;
        mWidth = width;
        mHeight = height;
        lockForVideoCapture.unlock();
    }

    void VideoProcessor::ShowHideGui()
    {
        //unique_lock<mutex> lockForShowUI(m_mutexForShowUI);
        m_showUI = !m_showUI;
        //lockForShowUI.unlock();
    }

    int VideoProcessor::GetHeight()
    {
        return _heightOfRectangle;
    }

    void VideoProcessor::Run()
    {
        if (!mVideoCapture.isOpened())
        {
            throw runtime_error("Video device is not opened.");
        }

        BackgroundSubtractorMOG2 bg_subtractor(1000, 15);
        vector<Vec4i> hierarchy;
        vector<vector<Point> > contours;

        while (!m_stop)
        {
            high_resolution_clock::time_point timeOfProcessingStart = high_resolution_clock::now();

            unique_lock<mutex> lockForVideoCapture(m_mutexForVideoCapture);

            if(mChangeResolution)
            {
                mVideoCapture.set(CV_CAP_PROP_FRAME_WIDTH, mWidth);
                mVideoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, mHeight);
                mChangeResolution = false;
            }

            bool frameIsRead = mVideoCapture.read(_originalFrame);

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


//            findContours(_frameWithForeground, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
//
//            unsigned int cmin = 100;
//            unsigned int cmax = 1000;
//
//            vector<vector<Point> >::const_iterator itc = contours.begin();
//            vector<Point> pts;
//            while (itc != contours.end())
//            {
//                if (itc->size() > cmin && itc->size() < cmax)
//                {
//                    vector<Point>::const_iterator pnt = (*itc).begin();
//                    while (pnt != (*itc).end())
//                    {
//                        pts.push_back(*pnt);
//                    }
//
//                    ++itc;
//                }
//                else
//                {
//                    ++itc;
//                }
//            }
//
//            if (pts.size() != 0)
//            {
//                Mat pointsMatrix = Mat(pts);
//                Scalar color(0, 255, 0);
//
//                Rect boundingRectangle = boundingRect(pointsMatrix);
//
//                _heightOfRectangle = boundingRectangle.height;
//
//                rectangle(_originalFrame, boundingRectangle, color, 2);
//            }

            /*bool showUI = false;

            unique_lock<mutex> lockForShowUI(m_mutexForShowUI);
            if (m_showUI)
            {
            showUI = m_showUI;
            }
            lockForShowUI.unlock();*/

            if (m_showUI)
            {
                if(m_windowsAreCreated == false)
                {
                    startWindowThread();
                    namedWindow(m_nameOfInputWindow, CV_WINDOW_AUTOSIZE);
                    namedWindow(m_nameOfOutputWindow, CV_WINDOW_AUTOSIZE);
                    m_windowsAreCreated = true;
                }

                imshow(m_nameOfInputWindow, _originalFrame);
                //imshow(processed_video, gray);
                //imshow(blurred_video, blur);
                //imshow(canny_video, canny);
                imshow(m_nameOfOutputWindow, _frameWithForeground);

                waitKey(25);
            }
            else
            {
                if (m_windowsAreCreated)
                {
                    destroyWindow(m_nameOfInputWindow);
                    destroyWindow(m_nameOfOutputWindow);
                    m_windowsAreCreated = false;
                }
            }

            _fps = 1000.0 / duration_cast<milliseconds>(high_resolution_clock::now() - timeOfProcessingStart).count();
        }
    }

    void VideoProcessor::Stop()
    {
        m_stop = true;
    }
}
