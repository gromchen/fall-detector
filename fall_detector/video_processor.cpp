#include "video_processor.h"

#include <boost/thread/thread.hpp>

#include "helpers.h"

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
        mMaxAreaOfObject = mFrameHeight*mFrameWidth/1.5;

        mIntervalProcessor.StartTracking();

        while (true)
            processFrame();
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

        mVideoCapture.set(CV_CAP_PROP_FRAME_WIDTH, mFrameWidth);
        mVideoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, mFrameHeight);
        mMaxAreaOfObject = mFrameHeight*mFrameWidth/1.5;

        string name_original_frame = "Original Frame";
        string name_foreground_mask = "Foreground Mask";
        string name_erode_mask = "Erode Mask";
        string name_dilate_mask = "Dilate Mask";

        startWindowThread();

        namedWindow(name_original_frame, CV_WINDOW_AUTOSIZE);
        namedWindow(name_foreground_mask, CV_WINDOW_AUTOSIZE);
        namedWindow(name_dilate_mask, CV_WINDOW_AUTOSIZE);
        namedWindow(name_erode_mask, CV_WINDOW_AUTOSIZE);

        createTrackbar("Erode element size", name_erode_mask, &mErodeElementSize, 50);
        createTrackbar("Erode iterations", name_erode_mask, &mErodeIterations, 50);
        createTrackbar("Dilate element size", name_dilate_mask, &mDilateElementSize, 50);
        createTrackbar("Dilate iterations", name_dilate_mask, &mDilateIterations, 50);

        mIntervalProcessor.StartTracking();

        while (true)
        {
            processFrame();

            if(mObjectFound)
                ellipse(mOriginalFrame, mEllipse, Scalar(0, 255, 0), 2);

            imshow(name_original_frame, mOriginalFrame);
            imshow(name_foreground_mask, mForegroundMask);
            imshow(name_dilate_mask, mDilateMask);
            imshow(name_erode_mask, mErodeMask);

            waitKey(30);
        }
    }
    catch(thread_interrupted&)
    {
        destroyAllWindows();

        if(mVideoCapture.isOpened())
            mVideoCapture.release();
    }
}

void VideoProcessor::SetResolution(int width, int height)
{
    mFrameWidth = width;
    mFrameHeight = height;
}

std::string VideoProcessor::PrintResolution()
{
    return ToString(mFrameWidth) + "x" + ToString(mFrameHeight);
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
    this_thread::interruption_point();

    if (!mVideoCapture.read(mOriginalFrame))
        throw runtime_error("Could not read new frame");

    (*mpBackgroundSubtractor)(mOriginalFrame, mForegroundMask);

    // Removes shadows
    threshold(mForegroundMask, mThresholdMask, 127, 255, THRESH_BINARY);

    Mat erode_element = getStructuringElement(MORPH_ELLIPSE,
                                              Size(mErodeElementSize,
                                                   mErodeElementSize));
    erode(mThresholdMask, mErodeMask, erode_element, Point(-1, -1),
          mErodeIterations);

    Mat dilate_element = getStructuringElement(MORPH_ELLIPSE,
                                               Size(mDilateElementSize,
                                                    mDilateElementSize));
    dilate(mErodeMask, mDilateMask, dilate_element, Point(-1, -1),
           mDilateIterations);

    mDilateMask.copyTo(mContoursMask);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(mContoursMask, contours, hierarchy, CV_RETR_CCOMP,
                 CV_CHAIN_APPROX_SIMPLE);
    drawContours(mOriginalFrame, contours, -1, Scalar(0, 0, 255), 2);

    mObjectFound = false;
    mEllipse = RotatedRect();
    //int x;
    //int y;

    if(hierarchy.size() > 0 && hierarchy.size() < mcMaxNumberOfObjects)
    {
        double reference_area = 0;

        for(int iContour = 0; iContour >= 0; iContour = hierarchy[iContour][0])
        {
            Moments object_moments = moments(Mat(contours[iContour]));
            double area = object_moments.m00;

            if(area > mcMinAreaOfObject && area < mMaxAreaOfObject && area > reference_area
                    && contours[iContour].size() > 5)
            {
                mEllipse = fitEllipse(Mat(contours[iContour]));
                //x = object_moments.m10/area;
                //y = object_moments.m01/area;
                mObjectFound = true;
                reference_area = area;
            }
        }
    }

    mIntervalProcessor.IncludeObject(FrameData(mEllipse, mObjectFound));
}
}
