#include "video_processor.h"

#include <fstream>
#include <sstream>
#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>

using namespace std;
using namespace boost;
using namespace boost::chrono;
using namespace cv;

namespace FallDetector
{
VideoProcessor::VideoProcessor()
//: mBackgroundSubtractor(16, 16, true)
{
    mFrameWidth = 320;
    mFrameHeight = 240;

    mErodeElementSize = 3;
    mErodeIterations = 2;
    mDilateElementSize = 8;
    mDilateIterations = 2;

    mHistory = 500;
    mThreshold = 16;
    mpBackgroundSubtractor = new BackgroundSubtractorMOG2(mHistory, mThreshold);

    mHasEllipse = false;

    mMaxAreaOfObject = mFrameHeight*mFrameWidth/1.5;
}

VideoProcessor::~VideoProcessor()
{
    delete mpBackgroundSubtractor;
}

void VideoProcessor::RunWithoutGui()
{
    try
    {
        if (!mVideoCapture.open(0))
            throw runtime_error("Could not open video device");

        mVideoCapture.set(CV_CAP_PROP_FRAME_WIDTH, mFrameWidth);
        mVideoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, mFrameHeight);

        while (true)
        {
            this_thread::interruption_point();
            high_resolution_clock::time_point start_time = high_resolution_clock::now();

            if (!mVideoCapture.read(mOriginalFrame))
                throw runtime_error("Could not read new frame");

            processFrame();
            mFps = 1000.0 / duration_cast<milliseconds>(
                        high_resolution_clock::now() - start_time).count();
            collectData();
        }
    }
    catch(thread_interrupted&)
    {
        clearStop();
    }
}

void VideoProcessor::RunWithGui()
{
    try
    {
        if (!mVideoCapture.open(0))
            throw runtime_error("Could not open video device");

        mVideoCapture.set(CV_CAP_PROP_FRAME_WIDTH, mFrameWidth);
        mVideoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, mFrameHeight);

        string name_original_frame = "Original Frame";
        string name_foreground_mask = "Foreground Mask";
        string name_erode_mask = "Erode Mask";
        string name_dilate_mask = "Dilate Mask";
        string name_contours_mask = "Contours Mask";

        startWindowThread();

        namedWindow(name_original_frame, CV_WINDOW_AUTOSIZE);
        namedWindow(name_foreground_mask, CV_WINDOW_AUTOSIZE);
        namedWindow(name_dilate_mask, CV_WINDOW_AUTOSIZE);
        namedWindow(name_erode_mask, CV_WINDOW_AUTOSIZE);
        namedWindow(name_contours_mask, CV_WINDOW_AUTOSIZE);

        createTrackbar("Erode element size", name_erode_mask, &mErodeElementSize, 50);
        createTrackbar("Erode iterations", name_erode_mask, &mErodeIterations, 50);
        createTrackbar("Dilate element size", name_dilate_mask, &mDilateElementSize, 50);
        createTrackbar("Dilate iterations", name_dilate_mask, &mDilateIterations, 50);

        while (true)
        {
            this_thread::interruption_point();
            high_resolution_clock::time_point start_time = high_resolution_clock::now();

            if (!mVideoCapture.read(mOriginalFrame))
                throw runtime_error("Could not read new frame");

            processFrame();
            ellipse(mOriginalFrame, mEllipse, Scalar(0, 255, 0), 2);

            imshow(name_original_frame, mOriginalFrame);
            imshow(name_foreground_mask, mForegroundMask);
            imshow(name_dilate_mask, mDilateMask);
            imshow(name_erode_mask, mErodeMask);
            imshow(name_contours_mask, mContoursMask);

            waitKey(30);

            mFps = 1000.0 / duration_cast<milliseconds>(
                        high_resolution_clock::now() - start_time).count();
            collectData();
        }
    }
    catch(thread_interrupted&)
    {
        destroyAllWindows();
        clearStop();
    }
}

void VideoProcessor::SetResolution(int width, int height)
{
    mFrameWidth = width;
    mFrameHeight = height;
}

std::string VideoProcessor::PrintResolution()
{
    return IntToString(mFrameWidth) + "x" + IntToString(mFrameHeight);
}

void VideoProcessor::CreateNewBackgroundSubtractor(int history, float threashold)
{
    mHistory = history;
    mThreshold = threashold;
    delete mpBackgroundSubtractor;
    mpBackgroundSubtractor = new BackgroundSubtractorMOG2(mHistory, mThreshold);
}

int VideoProcessor::GetHistory()
{
    return mHistory;
}

float VideoProcessor::GetThreshold()
{
    return mThreshold;
}

void VideoProcessor::processFrame()
{
    /*
    //cvtColor(frame, _grayFrame, CV_BGR2GRAY);

    //Mat bilateral_filter_output;
    //blur(_grayFrame, bilateral_filter_output, Size(21, 21)); // NOTE: not big change
    //medianBlur(_grayFrame, bilateral_filter_output, 41); // NOTE: not big change
    //GaussianBlur(_grayFrame, bilateral_filter_output, Size(21, 21), 0); // NOTE: not big change

    //Canny(blur, canny, 10, hey);
    */
    (*mpBackgroundSubtractor)(mOriginalFrame, mForegroundMask);
    threshold(mForegroundMask, mThresholdMask, 127, 255, THRESH_BINARY); // Remove shadows

    Mat erode_element = getStructuringElement(MORPH_ELLIPSE, Size(mErodeElementSize, mErodeElementSize));
    erode(mThresholdMask, mErodeMask, erode_element, Point(-1, -1), mErodeIterations);

    Mat dilate_element = getStructuringElement(MORPH_ELLIPSE, Size(mDilateElementSize, mDilateElementSize));
    dilate(mErodeMask, mDilateMask, dilate_element, Point(-1, -1), mDilateIterations);


    //findContours(foreground_mask, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

    mDilateMask.copyTo(mContoursMask);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(mContoursMask, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    drawContours(mOriginalFrame, contours, -1, Scalar(0, 0, 255), 2);

    // TODO: only one ellipse
    vector<Point> one_contour;

    for(unsigned int iContour = 0; iContour < contours.size(); iContour++)
    {
        for(unsigned int iPoint = 0; iPoint < contours[iContour].size(); iPoint++)
        {
            one_contour.push_back(contours[iContour][iPoint]);
        }
    }

    if(one_contour.size() > 5)
    {
        mEllipse = fitEllipse(Mat(one_contour));
        mHasEllipse = true;
    }
    else
        mHasEllipse = false;

    double reference_area = 0;
    bool mObjectFound = false;
    int x;
    int y;
    RotatedRect ell;

    if(hierarchy.size() > 0)
    {
        int number_of_objects = hierarchy.size();

        if(number_of_objects < mcMaxNumberOfObjects)
        {
            for(int iContour = 0; iContour >= 0; iContour = hierarchy[iContour][0])
            {
                Moments object_moments = moments(Mat(contours[iContour]));
                double area = object_moments.m00;

                if(area > mcMinAreaOfObject && area < mMaxAreaOfObject && area > reference_area)
                {
                    if(contours[iContour].size() > 5)
                    {
                        // TODO: collect only large contours
                        ell = fitEllipse(Mat(contours[iContour]));
                        //x = object_moments.m10/area;
                        //y = object_moments.m01/area;
                        mObjectFound = true;
                    }
                }
                else
                {
                    mObjectFound = false;
                }
            }

            if(mObjectFound == true)
            {
                ellipse(mOriginalFrame, ell, Scalar(255, 0, 0), 4);
                //circle(mOriginalFrame, Point(x, y), 20, Scalar(255, 0, 0),2);
            }
        }
    }
}

void VideoProcessor::collectData()
{
    if(mVideoDataCollection.size() < 100)
    {
        if(mHasEllipse) // TODO: register even is there is not ellipse
        {
            VideoData video_data(
                        boost::posix_time::microsec_clock::local_time(),
                        mFps,
                        mEllipse.size.height);
            mVideoDataCollection.push_back(video_data);
        }
    }
    else
        writeToFile();
}

void VideoProcessor::writeToFile()
{
    stringstream ss;
    ss << boost::posix_time::second_clock::local_time().date();
    mWriteProcesses.push_back(new thread(&VideoProcessor::log, this,
                                         "data(" + ss.str() + ").csv", mVideoDataCollection));
    mVideoDataCollection.clear();
}

void VideoProcessor::clearStop()
{
    if(mVideoCapture.isOpened())
        mVideoCapture.release();

    if(mVideoDataCollection.size() != 0)
        writeToFile();

    for(unsigned int iProcesses = 0; iProcesses < mWriteProcesses.size(); iProcesses++)
    {
        mWriteProcesses[iProcesses]->join();
        delete mWriteProcesses[iProcesses];
    }

    mWriteProcesses.clear();
}

void VideoProcessor::log(string fileName, vector<VideoData> data)
{
    ofstream out_file;
    mFileMutex.lock();
    out_file.open(fileName.c_str(), ios_base::app);

    for(unsigned int iData = 0; iData < data.size(); iData++)
    {
        out_file << data[iData].mCurrentTime.time_of_day() << ","
                 << data[iData].mFps << ","
                 << data[iData].mEllipseHeight << endl;
    }

    out_file.close();
    mFileMutex.unlock();
}
}
