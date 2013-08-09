#include "video_processor.h"

#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>

using namespace std;
using namespace boost;
using namespace boost::chrono;
using namespace cv;

namespace FallDetector
{
VideoProcessor::VideoProcessor()
{
    mResolutionWidth = 320;
    mResolutionHeight = 240;
}

VideoProcessor::~VideoProcessor()
{
}

void VideoProcessor::RunWithoutGui()
{
    try
    {
        if (!mVideoCapture.open(0))
            throw runtime_error("Could not open video device");

        mVideoCapture.set(CV_CAP_PROP_FRAME_WIDTH, mResolutionWidth);
        mVideoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, mResolutionHeight);

        while (true)
        {
            this_thread::interruption_point();
            high_resolution_clock::time_point start_time = high_resolution_clock::now();

            if (!mVideoCapture.read(mOriginalFrame))
                throw runtime_error("Could not read new frame");

            processFrame();
            mFps = 1000.0 / duration_cast<milliseconds>(
                        high_resolution_clock::now() - start_time).count();
        }
    }
    catch(thread_interrupted&)
    {
        if(mVideoCapture.isOpened())
            mVideoCapture.release();
    }
}

void VideoProcessor::RunWithGui()
{
    try
    {
        if (!mVideoCapture.open(0))
            throw runtime_error("Could not open video device");

        mVideoCapture.set(CV_CAP_PROP_FRAME_WIDTH, mResolutionWidth);
        mVideoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, mResolutionHeight);

        string name_original_frame = "Original Frame";
        string name_foreground_mask = "Foreground Mask";
        string name_erode_mask = "Erode Mask";
        string name_dilate_mask = "Dilate Mask";
        string name_contours_mask = "Contours Mask";

        startWindowThread();

        namedWindow(name_original_frame, CV_WINDOW_AUTOSIZE);
        namedWindow(name_foreground_mask, CV_WINDOW_AUTOSIZE);
        namedWindow(name_erode_mask, CV_WINDOW_AUTOSIZE);
        namedWindow(name_dilate_mask, CV_WINDOW_AUTOSIZE);
        namedWindow(name_contours_mask, CV_WINDOW_AUTOSIZE);

        while (true)
        {
            this_thread::interruption_point();
            high_resolution_clock::time_point start_time = high_resolution_clock::now();

            if (!mVideoCapture.read(mOriginalFrame))
                throw runtime_error("Could not read new frame");

            processFrame();

            imshow(name_original_frame, mOriginalFrame);
            imshow(name_foreground_mask, mForegroundMask);
            imshow(name_erode_mask, mErodeMask);
            imshow(name_dilate_mask, mDilateMask);
            imshow(name_contours_mask, mContoursMask);

            waitKey(30);

            mFps = 1000.0 / duration_cast<milliseconds>(
                        high_resolution_clock::now() - start_time).count();
        }
    }
    catch(thread_interrupted&)
    {
        if(mVideoCapture.isOpened())
            mVideoCapture.release();

        destroyAllWindows();
    }
}

void VideoProcessor::SetResolution(int width, int height)
{
    mResolutionWidth = width;
    mResolutionHeight = height;
}

std::string VideoProcessor::PrintResolution()
{
    return to_string(mResolutionWidth) + "x" + to_string(mResolutionHeight);
}

void VideoProcessor::processFrame()
{
    //cvtColor(frame, _grayFrame, CV_BGR2GRAY);

    //Mat bilateral_filter_output;
    //blur(_grayFrame, bilateral_filter_output, Size(21, 21)); // NOTE: not big change
    //medianBlur(_grayFrame, bilateral_filter_output, 41); // NOTE: not big change
    //GaussianBlur(_grayFrame, bilateral_filter_output, Size(21, 21), 0); // NOTE: not big change

    //Canny(blur, canny, 10, hey);

    mBackgroundSubtractor(mOriginalFrame, mForegroundMask);

    erode(mForegroundMask, mErodeMask, Mat());
    dilate(mErodeMask, mDilateMask, Mat());

    //vector<Vec4i> hierarchy;
    //findContours(foreground_mask, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

    mContoursMask = mDilateMask; // TODO: Bad copy
    vector<vector<Point> > contours;
    findContours(mContoursMask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    // TODO: CV_RETR_TREE

    drawContours(mOriginalFrame, contours, -1, Scalar(0, 0, 255), 2);

    vector<RotatedRect> minEllipse(contours.size());

    for(unsigned int i = 0; i < contours.size(); i++)
        if(contours[i].size() > 5)
            minEllipse[i] = fitEllipse(Mat(contours[i]));

    for(unsigned int i = 0; i < contours.size(); i++)
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

        ellipse(mOriginalFrame, minEllipse[i], color, 2, 8);
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
}
}
