#include "video_processor.h"

#include <fstream>
#include <sstream>
#include <cmath>
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
    mFrameWidth = 320;
    mFrameHeight = 240;

    mErodeElementSize = 3;
    mErodeIterations = 2;
    mDilateElementSize = 8;
    mDilateIterations = 2;

    mHistory = 500;
    mThreshold = 16;
    mpBackgroundSubtractor = new BackgroundSubtractorMOG2(mHistory, mThreshold);

    initializeVariables();
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

        initializeVariables();

        while (true)
        {
            processFrame();
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
        createTrackbar("One contours or several", name_original_frame, &mObjectFromOneContour, 1);

        initializeVariables();

        while (true)
        {
            processFrame();

            if(mObjectFound)
                ellipse(mOriginalFrame, mEllipse, Scalar(0, 255, 0), 2);

            imshow(name_original_frame, mOriginalFrame);
            imshow(name_foreground_mask, mForegroundMask);
            imshow(name_dilate_mask, mDilateMask);
            imshow(name_erode_mask, mErodeMask);
            imshow(name_contours_mask, mContoursMask);

            waitKey(30);
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

void VideoProcessor::initializeVariables()
{
    mObjectFound = false;
    mObjectFromOneContour = 0;

    mMaxAreaOfObject = mFrameHeight*mFrameWidth/1.5;

    mHasObjectDataAfterOneSecond = false;
    mStandardDeviationOfOrientation = 0;
    mStandardDeviatioinOfRatio = 0;

    mFps = 0;
    mTimeOfPreviousSecond = high_resolution_clock::now();
    mFrameCount = 0;
}

void VideoProcessor::processFrame()
{
    this_thread::interruption_point();
    mFrameCount++;

    if (!mVideoCapture.read(mOriginalFrame))
        throw runtime_error("Could not read new frame");

    (*mpBackgroundSubtractor)(mOriginalFrame, mForegroundMask);
    threshold(mForegroundMask, mThresholdMask, 127, 255, THRESH_BINARY); // Removes shadows

    Mat erode_element = getStructuringElement(MORPH_ELLIPSE, Size(mErodeElementSize, mErodeElementSize));
    erode(mThresholdMask, mErodeMask, erode_element, Point(-1, -1), mErodeIterations);

    Mat dilate_element = getStructuringElement(MORPH_ELLIPSE, Size(mDilateElementSize, mDilateElementSize));
    dilate(mErodeMask, mDilateMask, dilate_element, Point(-1, -1), mDilateIterations);

    mDilateMask.copyTo(mContoursMask);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(mContoursMask, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    drawContours(mOriginalFrame, contours, -1, Scalar(0, 0, 255), 2);

    mObjectFound = false;
    mEllipse = RotatedRect();
    mStandardDeviationOfOrientation = 0;
    mStandardDeviatioinOfRatio = 0;
    //int x;
    //int y;

    if(hierarchy.size() > 0 && hierarchy.size() < mcMaxNumberOfObjects)
    {
        if(mObjectFromOneContour == 0)
        {
            double reference_area = 0;

            for(int iContour = 0; iContour >= 0; iContour = hierarchy[iContour][0])
            {
                Moments object_moments = moments(Mat(contours[iContour]));
                double area = object_moments.m00;

                if(area > mcMinAreaOfObject
                        && area < mMaxAreaOfObject
                        && area > reference_area
                        && contours[iContour].size() > 5)
                {
                    mEllipse = fitEllipse(Mat(contours[iContour]));
                    //x = object_moments.m10/area;
                    //y = object_moments.m01/area;
                    mObjectFound = true;
                    reference_area = area;
                }
                else
                    mObjectFound = false;
            }
        }
        else
        {
            vector<Point> one_contour;

            for(int iContour = 0; iContour >= 0; iContour = hierarchy[iContour][0])
            {
                Moments object_moments = moments(Mat(contours[iContour]));
                double area = object_moments.m00;

                if(area > mcMinAreaOfObject
                        && area < mMaxAreaOfObject
                        && contours[iContour].size() > 5)
                    for(unsigned int iPoint = 0; iPoint < contours[iContour].size(); iPoint++)
                        one_contour.push_back(contours[iContour][iPoint]);
            }

            if(one_contour.size() > 5)
            {
                mEllipse = fitEllipse(Mat(one_contour));
                mObjectFound = true;
            }
            else
                mObjectFound = false;
        }
    }

    if(mObjectFound)
    {
        mEllipsesAfterOneSecond.push_back(mEllipse);
    }

    high_resolution_clock::time_point current_time = high_resolution_clock::now();
    int number_of_milliseconds = duration_cast<milliseconds>(current_time - mTimeOfPreviousSecond).count();

    if(number_of_milliseconds >= 1000)
    {
        if(mEllipsesAfterOneSecond.size() != 0)
        {
            mHasObjectDataAfterOneSecond = true;

            int sum_of_angles = 0;
            int sum_of_ratios = 0;
            vector<double> ratios;

            for(unsigned int iEllipse = 0; iEllipse < mEllipsesAfterOneSecond.size(); iEllipse++)
            {
                sum_of_angles += mEllipsesAfterOneSecond[iEllipse].angle;
                ratios.push_back(mEllipsesAfterOneSecond[iEllipse].size.height
                                 /mEllipsesAfterOneSecond[iEllipse].size.width);
                sum_of_ratios += ratios[iEllipse];
            }

            int average_of_angles = sum_of_angles/mEllipsesAfterOneSecond.size();
            int average_of_ratios = sum_of_ratios/ratios.size();
            int mean_square_sum_of_angles = 0;
            int mean_square_sum_of_ratios = 0;

            for(unsigned int iEllipse = 0; iEllipse < mEllipsesAfterOneSecond.size(); iEllipse++)
            {
                mean_square_sum_of_angles += pow(mEllipsesAfterOneSecond[iEllipse].angle - average_of_angles, 2);
                mean_square_sum_of_ratios += pow(ratios[iEllipse] - average_of_ratios, 2);
            }

            mStandardDeviationOfOrientation = sqrt(mean_square_sum_of_angles / mEllipsesAfterOneSecond.size());
            mStandardDeviatioinOfRatio = sqrt(mean_square_sum_of_ratios / ratios.size());

            mEllipsesAfterOneSecond.clear();
        }
        else
        {
            mHasObjectDataAfterOneSecond = false;
        }

        // TODO: process sequence of deviations

        mFps = mFrameCount * 1000 / number_of_milliseconds;
        mTimeOfPreviousSecond = current_time;
        mFrameCount = 0;

        collectData();
    }
}

void VideoProcessor::collectData()
{
    if(mVideoDataCollection.size() < 10)
        mVideoDataCollection.push_back(VideoData(boost::posix_time::microsec_clock::local_time(),
                                                 mFps,
                                                 mStandardDeviationOfOrientation,
                                                 mStandardDeviatioinOfRatio,
                                                 mHasObjectDataAfterOneSecond));
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
                 << data[iData].mFps << ",";

        if(mHasObjectDataAfterOneSecond)
        {
            out_file << data[iData].mStandardDeviationOfOrientation << ","
                     << data[iData].mStandardDeviationOfRatio << ",has object" << endl;
        }
        else
        {
            out_file << ",,without object" << endl;
        }
    }

    out_file.close();
    mFileMutex.unlock();
}
}
