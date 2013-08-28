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
    mCameraNumber = 1;
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
        if (!mVideoCapture.open(mCameraNumber))
            throw runtime_error("Could not open video device");

        mVideoCapture.set(CV_CAP_PROP_FRAME_WIDTH, mFrameWidth);
        mVideoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, mFrameHeight);
        mMaxAreaOfObject = mFrameHeight*mFrameWidth/1.5;
        mMhiMask = Mat::zeros(mFrameHeight, mFrameWidth, CV_32FC1);

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
        if (!mVideoCapture.open(mCameraNumber))
            throw runtime_error("Could not open video device");

        mVideoCapture.set(CV_CAP_PROP_FRAME_WIDTH, mFrameWidth);
        mVideoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, mFrameHeight);
        mMaxAreaOfObject = mFrameHeight*mFrameWidth/1.5;
        mMhiMask = Mat::zeros(mFrameHeight, mFrameWidth, CV_32FC1);

        string name_original_frame = "Original Frame";
        string name_foreground_mask = "Foreground Mask";
        string name_erode_mask = "Erode Mask";
        string name_dilate_mask = "Dilate Mask";

        string name_mhi = "MHI";

        startWindowThread();

        namedWindow(name_original_frame, CV_WINDOW_AUTOSIZE);
        namedWindow(name_foreground_mask, CV_WINDOW_AUTOSIZE);
        namedWindow(name_dilate_mask, CV_WINDOW_AUTOSIZE);
        namedWindow(name_erode_mask, CV_WINDOW_AUTOSIZE);

        namedWindow(name_mhi, CV_WINDOW_AUTOSIZE);

        createTrackbar("Erode element size", name_erode_mask,
                       &mErodeElementSize, 50);
        createTrackbar("Erode iterations", name_erode_mask,
                       &mErodeIterations, 50);
        createTrackbar("Dilate element size", name_dilate_mask,
                       &mDilateElementSize, 50);
        createTrackbar("Dilate iterations", name_dilate_mask,
                       &mDilateIterations, 50);

        mIntervalProcessor.StartTracking();

        while (true)
        {
            processFrame();

            if(mObjectFound)
                ellipse(mOriginalFrame, mEllipse, Scalar(0, 255, 0), 2);

//            if(mIntervalProcessor.FallDetected())
//                putText(mOriginalFrame, "Fall detected", Point(0, 0), 1, 1,
//                        Scalar(0, 255, 0), 2);
//            else
//                putText(mOriginalFrame, "Stable condition", Point(0, 0), 1, 1,
//                        Scalar(255, 0, 0), 2);

            imshow(name_original_frame, mOriginalFrame);
            imshow(name_foreground_mask, mForegroundMask);
            imshow(name_dilate_mask, mDilateMask);
            imshow(name_erode_mask, mErodeMask);

            imshow(name_mhi, mMhiMask);

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

void VideoProcessor::CreateNewBackgroundSubtractor(int history,
                                                   float threashold)
{
    mHistory = history;
    mThreshold = threashold;
    delete mpBackgroundSubtractor;
    mpBackgroundSubtractor = new BackgroundSubtractorMOG2(mHistory, mThreshold);
}

void VideoProcessor::processFrame()
{
    this_thread::interruption_point();

    if (!mVideoCapture.read(mOriginalFrame))
        throw runtime_error("Could not read new frame");

    (*mpBackgroundSubtractor)(mOriginalFrame, mForegroundMask);

    // Removes shadows
    threshold(mForegroundMask, mThresholdMask, 127, 255, THRESH_BINARY);

    // Morphological transformation
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

    // Contours
    mDilateMask.copyTo(mContoursMask);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(mContoursMask, contours, hierarchy, CV_RETR_CCOMP,
                 CV_CHAIN_APPROX_SIMPLE);
    drawContours(mOriginalFrame, contours, -1, Scalar(0, 0, 255), 2);

    // Object detection
    mObjectFound = false;
    mEllipse = RotatedRect();
    int x = 0;
    int y = 0;

    if(hierarchy.size() > 0 && hierarchy.size() < mcMaxNumberOfObjects)
    {
        double reference_area = 0;

        for(int iContour = 0; iContour >= 0; iContour = hierarchy[iContour][0])
        {
            Moments object_moments = moments(Mat(contours[iContour]));
            double area = object_moments.m00;

            if(area > mcMinAreaOfObject && area < mMaxAreaOfObject
                    && area > reference_area && contours[iContour].size() > 5)
            {
                mEllipse = fitEllipse(Mat(contours[iContour]));
                x = object_moments.m10/area;
                y = object_moments.m01/area;
                mObjectFound = true;
                reference_area = area;
            }
        }
    }

    double c_motion = 0;

//    if(mObjectFound)
//        c_motion = calculateCoefficientOfMotion(mDilateMask, mMhiMask);

    mIntervalProcessor.IncludeObject(FrameData(c_motion, mEllipse,
                                               mObjectFound));
}

double VideoProcessor::calculateCoefficientOfMotion(Mat &silhouette,
                                                    Mat &mhiMask)
{
    updateMotionHistory(silhouette, mhiMask,
                        (double)clock()/CLOCKS_PER_SEC, 1);

    unsigned int number_of_rows = mhiMask.rows;
    unsigned int number_of_columns = mhiMask.cols;

    double number_of_blob_pixels = 0;
    double number_of_history_pixels = 0;

    for(unsigned int iRow = 0; iRow < number_of_rows; iRow++)
    {
        unsigned char* foreground_data = silhouette.ptr(iRow);
        unsigned char* mhi_data = mhiMask.ptr(iRow);

        for(unsigned int iColumn = 0; iColumn < number_of_columns; iColumn++)
        {
            if(mhi_data[iColumn] > 0)
                number_of_history_pixels += 1;

            if(foreground_data[iColumn] > 0)
                number_of_blob_pixels += 1;
        }
    }

    double c_motion = 0;

    if(number_of_history_pixels > number_of_blob_pixels
            && number_of_blob_pixels > 0)
    {
        c_motion = (number_of_history_pixels - number_of_blob_pixels)
                / number_of_blob_pixels;
    }

    cout << number_of_history_pixels << " "
         << number_of_blob_pixels << " "
         << c_motion << endl;

    return c_motion;
}
}
