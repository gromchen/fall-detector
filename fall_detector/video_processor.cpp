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
    mResolutionWidth = 320;
    mResolutionHeight = 240;

    mErodeElementSize = 3;
    mErodeIterations = 2;
    mDilateElementSize = 8;
    mDilateIterations = 2;

    mHistory = 500;
    mThreshold = 16;
    //mThresholdGen = 0;

    mpBackgroundSubtractor = new BackgroundSubtractorMOG2(mHistory, mThreshold);
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

        mVideoCapture.set(CV_CAP_PROP_FRAME_WIDTH, mResolutionWidth);
        mVideoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, mResolutionHeight);

        string name_original_frame = "Original Frame";
        string name_foreground_mask = "Foreground Mask";
        //string name_morphological_mask = "Morphological Mask";
        string name_erode_mask = "Erode Mask";
        string name_dilate_mask = "Dilate Mask";
        string name_contours_mask = "Contours Mask";

        startWindowThread();

        namedWindow(name_original_frame, CV_WINDOW_AUTOSIZE);
        namedWindow(name_foreground_mask, CV_WINDOW_AUTOSIZE);
        //namedWindow(name_morphological_mask, CV_WINDOW_AUTOSIZE);
        namedWindow(name_dilate_mask, CV_WINDOW_AUTOSIZE);
        namedWindow(name_erode_mask, CV_WINDOW_AUTOSIZE);
        namedWindow(name_contours_mask, CV_WINDOW_AUTOSIZE);

        createTrackbar("Erode element size", name_erode_mask, &mErodeElementSize, 50);
        createTrackbar("Erode iterations", name_erode_mask, &mErodeIterations, 50);
        createTrackbar("Dilate element size", name_dilate_mask, &mDilateElementSize, 50);
        createTrackbar("Dilate iterations", name_dilate_mask, &mDilateIterations, 50);

        //createTrackbar("History", name_original_frame, &mHistory, 3000);
        //createTrackbar("Threshold", name_original_frame, &mThreshold, 50);
        //createTrackbar("Threshold Gen", name_original_frame, &mThresholdGen, 50);

        while (true)
        {
            this_thread::interruption_point();
            high_resolution_clock::time_point start_time = high_resolution_clock::now();

            if (!mVideoCapture.read(mOriginalFrame))
                throw runtime_error("Could not read new frame");

            processFrame();

            imshow(name_original_frame, mOriginalFrame);
            imshow(name_foreground_mask, mForegroundMask);
            //imshow(name_morphological_mask, mMorphologicalMask);
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
    mResolutionWidth = width;
    mResolutionHeight = height;
}

std::string VideoProcessor::PrintResolution()
{
    return IntToString(mResolutionWidth) + "x" + IntToString(mResolutionHeight);
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
    threshold(mForegroundMask, mThresholdMask, 127, 255, THRESH_BINARY);

    Mat erode_element = getStructuringElement(MORPH_ELLIPSE, Size(mErodeElementSize, mErodeElementSize));
    erode(mThresholdMask, mErodeMask, erode_element, Point(-1, -1), mErodeIterations);

    Mat dilate_element = getStructuringElement(MORPH_ELLIPSE, Size(mDilateElementSize, mDilateElementSize));
    dilate(mErodeMask, mDilateMask, dilate_element, Point(-1, -1), mDilateIterations);

    //vector<Vec4i> hierarchy;
    //findContours(foreground_mask, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

    mDilateMask.copyTo(mContoursMask);
    vector<vector<Point> > contours;
    findContours(mContoursMask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    // TODO: CV_RETR_TREE

    drawContours(mOriginalFrame, contours, -1, Scalar(0, 0, 255), 2);
    mEllipses.clear();

    for(unsigned int iContour = 0; iContour < contours.size(); iContour++)
        if(contours[iContour].size() > 5)
            mEllipses.push_back(fitEllipse(Mat(contours[iContour])));

    Scalar color = Scalar(0, 255, 0);

    for(unsigned int iContour = 0; iContour < contours.size(); iContour++)
    {
        /*
        //                if(hierarchy[i][3] == -1)
        //                {
        //                    color = Scalar(255, 0, 0);
        //                }
        //
        //                if(hierarchy[i][2] == -1)
        //                {
        //                    color = Scalar(0, 0, 255);
        //                }
        */
        ellipse(mOriginalFrame, mEllipses[iContour], color, 2, 8);
    }
    /*
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
    */
}

void VideoProcessor::collectData()
{
    if(mVideoDataCollection.size() < 100)
    {
        if(mEllipses.size() != 0)
        {
            VideoData video_data(
                        boost::posix_time::microsec_clock::local_time(),
                        mFps,
                        mEllipses[0].size.height);
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
