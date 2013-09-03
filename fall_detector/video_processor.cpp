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
    mCameraNumber = 0;
    mFrameWidth = 320;
    mFrameHeight = 240;

    mErodeElementSize = 3;
    mErodeIterations = 2;
    mDilateElementSize = 8;
    mDilateIterations = 2;

    mHistory = 3000; // TODO: maybe even more
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
        {
            throw runtime_error("Could not open video device");
        }

        mVideoCapture.set(CV_CAP_PROP_FRAME_WIDTH, mFrameWidth);
        mVideoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, mFrameHeight);
        mMaxAreaOfObject = mFrameHeight*mFrameWidth/1.5;
        mMhiMask = Mat::zeros(mFrameHeight, mFrameWidth, CV_32FC1);

        mIntervalProcessor.StartTracking();

        while (true)
        {
            ProcessFrame();
        }
    }
    catch(thread_interrupted&)
    {
        if(mVideoCapture.isOpened())
        {
            mVideoCapture.release();
        }

        mIntervalProcessor.Reset();
    }
}

void VideoProcessor::RunWithGui()
{
    try
    {
        if (!mVideoCapture.open(mCameraNumber))
        {
            throw runtime_error("Could not open video device");
        }

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
            ProcessFrame();

            if(mObjectFound)
            {
                ellipse(mOriginalFrame, mRotatedRectangle, Scalar(0, 255, 0), 2);
            }

            Scalar colour = Scalar(255, 0, 0);

            if(mIntervalProcessor.GetHumanState() == INJURED)
            {
                colour = Scalar(0, 0, 255);
            }

            putText(mOriginalFrame, "Human is " + HumanStateToString(mIntervalProcessor.GetHumanState()),
                    Point(0, 20), 1, 1, colour, 2);

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
        {
            mVideoCapture.release();
        }

        mIntervalProcessor.Reset();
    }
}

void VideoProcessor::SetResolution(int width, int height)
{
    mFrameWidth = width;
    mFrameHeight = height;
}

std::string VideoProcessor::GetResolution()
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

void VideoProcessor::ProcessFrame()
{
    this_thread::interruption_point();

    if (!mVideoCapture.read(mOriginalFrame))
    {
        throw runtime_error("Could not read new frame");
    }

    (*mpBackgroundSubtractor)(mOriginalFrame, mForegroundMask);

    // Removes shadows
    threshold(mForegroundMask, mThresholdMask, 127, 255, THRESH_BINARY);

    // Morphological transformation
    Mat erode_element = getStructuringElement(MORPH_ELLIPSE, Size(mErodeElementSize, mErodeElementSize));
    erode(mThresholdMask, mErodeMask, erode_element, Point(-1, -1), mErodeIterations);

    dilate(mErodeMask, mErodeMask, erode_element, Point(-1, -1), mErodeIterations);

    Mat dilate_element = getStructuringElement(MORPH_ELLIPSE, Size(mDilateElementSize, mDilateElementSize));
    dilate(mErodeMask, mDilateMask, dilate_element, Point(-1, -1), mDilateIterations);

    erode(mDilateMask, mDilateMask, dilate_element, Point(-1, -1), mDilateIterations);

    // Contours
    mDilateMask.copyTo(mContoursMask);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(mContoursMask, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    drawContours(mOriginalFrame, contours, -1, Scalar(0, 0, 255), 2);

    // Object detection
    mObjectFound = false;
    mRotatedRectangle = RotatedRect();

    if(hierarchy.size() > 0 && hierarchy.size() < mcMaxNumberOfObjects)
    {
        double reference_area = 0;

        for(int i_contour = 0; i_contour >= 0; i_contour = hierarchy[i_contour][0])
        {
            Moments object_moments = moments(Mat(contours[i_contour]));
            double area = object_moments.m00;

            if(area > mcMinAreaOfObject && area < mMaxAreaOfObject && area > reference_area
               && contours[i_contour].size() > 5)
            {
                mRotatedRectangle = fitEllipse(Mat(contours[i_contour]));
                mObjectFound = true;
                reference_area = area;
            }
        }
    }

    double coefficient_of_motion = 0;

    if(mObjectFound)
    {
        coefficient_of_motion = CalculateCoefficientOfMotion(mDilateMask, mMhiMask);
    }

    if(mObjectFound)
    {
        mIntervalProcessor.Include(FrameInfo(coefficient_of_motion, optional<RotatedRect>(mRotatedRectangle)));
    }
    else
    {
        mIntervalProcessor.Include(FrameInfo(coefficient_of_motion, optional<RotatedRect>()));
    }
}

double VideoProcessor::CalculateCoefficientOfMotion(Mat &silhouette, Mat &mhiMask)
{
    updateMotionHistory(silhouette, mhiMask, (double)clock()/CLOCKS_PER_SEC, 1);

    double number_of_blob_pixels = 0;
    double number_of_history_pixels = 0;

    for(int i_row = 0; i_row < mhiMask.rows; i_row++)
    {
        unsigned char* foreground_data = silhouette.ptr(i_row);
        unsigned char* mhi_data = mhiMask.ptr(i_row);

        for(int i_column = 0; i_column < mhiMask.cols; i_column++)
        {
            if(mhi_data[i_column] > 0)
            {
                number_of_history_pixels += 1;
            }

            if(foreground_data[i_column] > 0)
            {
                number_of_blob_pixels += 1;
            }
        }
    }

    double c_motion = 0;

    if(number_of_history_pixels > number_of_blob_pixels && number_of_blob_pixels > 0)
    {
        c_motion = (number_of_history_pixels - number_of_blob_pixels) / number_of_blob_pixels;
    }

    //    cout << number_of_history_pixels << " "
    //         << number_of_blob_pixels << " "
    //         << c_motion << endl;

    return c_motion;
}
}
