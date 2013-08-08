#include "video_processor.h"

#include <boost/chrono.hpp>

#include "mog2_public.h"

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

        BackgroundSubtractorMOG2 bg_subtractor;

        //bg_subtractor.SetTau(0.01);

        while (!m_stop)
        {
            high_resolution_clock::time_point timeOfProcessingStart = high_resolution_clock::now();

            Mat frame;

            unique_lock<mutex> lockForVideoCapture(m_mutexForVideoCapture);

            if(mChangeResolution)
            {
                mVideoCapture.set(CV_CAP_PROP_FRAME_WIDTH, mWidth);
                mVideoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, mHeight);
                mChangeResolution = false;
            }

            bool frameIsRead = mVideoCapture.read(frame);

            lockForVideoCapture.unlock();

            if (!frameIsRead)
            {
                throw runtime_error("Could not read new frame");
            }

            cvtColor(frame, _grayFrame, CV_BGR2GRAY);

            //Mat bilateral_filter_output;

            //blur(_grayFrame, bilateral_filter_output, Size(21, 21)); // NOTE: not big change
            //medianBlur(_grayFrame, bilateral_filter_output, 41); // NOTE: not big change
            //GaussianBlur(_grayFrame, bilateral_filter_output, Size(21, 21), 0); // NOTE: not big change
            //Canny(blur, canny, 10, hey);

            Mat foreground_mask;

            bg_subtractor(frame, foreground_mask);
            erode(foreground_mask, foreground_mask, Mat(), Point(-1, -1), 3);
            dilate(foreground_mask, foreground_mask, Mat(), Point(-1, -1), 3);

            vector<vector<Point> > contours;
            //vector<Vec4i> hierarchy;

            //findContours(foreground_mask, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
            findContours(foreground_mask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
            // TODO: CV_RETR_TREE

            drawContours(frame, contours, -1, Scalar(0, 0, 255), 2);

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

//                if(hierarchy[i][3] == -1)
//                {
//                    color = Scalar(255, 0, 0);
//                }
//
//                if(hierarchy[i][2] == -1)
//                {
//                    color = Scalar(0, 0, 255);
//                }

                ellipse(frame, minEllipse[i], color, 2, 8);
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

                imshow(m_nameOfInputWindow, frame);
                //imshow(processed_video, gray);
                //imshow(mNameOfBlurredWindow, bilateral_filter_output);
                //imshow(canny_video, canny);
                imshow(m_nameOfOutputWindow, foreground_mask);

                waitKey(30);
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
