#include "video_processor.h"

#include <boost/chrono.hpp>

#include "background_subtractor_fd.h"

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

        BackgroundSubtractorFD bg_subtractor(21, 0.1);

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

            cvtColor(_originalFrame, _grayFrame, CV_BGR2GRAY);

            //Mat bilateral_filter_output;

            //blur(_grayFrame, bilateral_filter_output, Size(21, 21)); // NOTE: not big change
            //medianBlur(_grayFrame, bilateral_filter_output, 41); // NOTE: not big change
            //GaussianBlur(_grayFrame, bilateral_filter_output, Size(21, 21), 0); // NOTE: not big change
            //Canny(blur, canny, 10, hey);

            bg_subtractor(_grayFrame, _frameWithForeground);
            //erode(foreground, foreground, Mat());
            //dilate(foreground, foreground, Mat());

            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;

            findContours(_frameWithForeground, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
            // TODO: CV_RETR_TREE

            vector<RotatedRect> minEllipse(contours.size());

            for(int i = 0; i < contours.size(); i++)
            {
                if(contours[i].size() > 5)
                {
                    minEllipse[i] = fitEllipse(Mat(contours[i]));
                }
            }

            for(int i = 0; i < contours.size(); i++)
            {
                Scalar color = Scalar(0, 255, 0);

                if(hierarchy[i][3] == -1)
                {
                    color = Scalar(255, 0, 0);
                }

                if(hierarchy[i][2] == -1)
                {
                    color = Scalar(0, 0, 255);
                }

                ellipse(_originalFrame, minEllipse[i], color, 2, 8);
            }
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

            if (m_showUI)
            {
                if(m_windowsAreCreated == false)
                {
                    startWindowThread();
                    namedWindow(m_nameOfInputWindow, CV_WINDOW_AUTOSIZE);
                  //  namedWindow(mNameOfBlurredWindow, CV_WINDOW_AUTOSIZE);
                    namedWindow(m_nameOfOutputWindow, CV_WINDOW_AUTOSIZE);
                    m_windowsAreCreated = true;
                }

                imshow(m_nameOfInputWindow, _originalFrame);
                //imshow(processed_video, gray);
                //imshow(mNameOfBlurredWindow, bilateral_filter_output);
                //imshow(canny_video, canny);
                imshow(m_nameOfOutputWindow, _frameWithForeground);

                waitKey(25);
            }
            else
            {
                if (m_windowsAreCreated)
                {
                    destroyWindow(m_nameOfInputWindow);
                    //destroyWindow(mNameOfBlurredWindow);
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
