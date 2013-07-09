#include <iostream>
#include <chrono>
#include <vector>

#ifdef _WIN32
#include <vld.h> // Visual Leak Detector
#endif

#include <opencv2/core/core.hpp> // Basic OpenCV data structures
#include <opencv2/highgui/highgui.hpp> // OpenCV interface
#include <opencv2/imgproc/imgproc.hpp> // Image processing module
#include <opencv2/video/background_segm.hpp>

using namespace std;
using namespace std::chrono;
using namespace cv;

int hey = 0;

void OnChange(int pos, void* userdata)
{
    hey = pos;
}

int main(int argc, char** argv)
{
    VideoCapture capture(0);

    if (!capture.isOpened())
    {
        cout << "Failed to open a video device!" << endl;
        return 1;
    }

    capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

    int value = 50;

    string camera_video = "Camera Video";
    string processed_video = "Processed Video";
    string blurred_video = "Blurred Video";
    string canny_video = "Canny Video";
    string mog_video = "MOG Video";

    namedWindow(camera_video, CV_WINDOW_AUTOSIZE);
    namedWindow(processed_video, CV_WINDOW_AUTOSIZE);
    namedWindow(blurred_video, CV_WINDOW_AUTOSIZE);
    namedWindow(canny_video, CV_WINDOW_AUTOSIZE);
    namedWindow(mog_video, CV_WINDOW_AUTOSIZE);

    createTrackbar("track1", camera_video, &value, 255, OnChange);

    Mat frame;
    Mat gray;
    //Mat blur;
    //Mat canny;
    Mat foreground;

    Mat thresholded;
    Mat thresholded2;

    char key = 0;
    bool show_ui = false;

    BackgroundSubtractorMOG2 bg_subtractor(1000, 15);

    vector<vector<Point>> contours;

    while (key != 'q')
    {
        auto t0 = high_resolution_clock::now();

        if (key == 's')
        {
            show_ui = !show_ui;
        }

        if (!capture.read(frame))
        {
            break;
        }

        cvtColor(frame, gray, CV_BGR2GRAY);
        //GaussianBlur(gray, blur, Size(5, 5), 0);
        //Canny(blur, canny, 10, hey);

        bg_subtractor(gray, foreground, 0.01);
        //erode(foreground, foreground, Mat());
        //dilate(foreground, foreground, Mat());

        threshold(foreground, thresholded, 70.0f, 255, CV_THRESH_BINARY);
        cv::Mat elementCLOSE(5, 5, CV_8U, cv::Scalar(1));
        cv::morphologyEx(thresholded, thresholded, cv::MORPH_CLOSE, elementCLOSE);

        findContours(thresholded, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
        //drawContours(frame, contours, -1, Scalar(255, 255, 0), 4);

        unsigned int cmin = 50;
        unsigned int cmax = 1000;

        vector<vector<Point>>::iterator itc = contours.begin();

        while (itc != contours.end())
        {
            if (itc->size() > cmin && itc->size() < cmax)
            {
                vector<Point> pts = *itc;
                Mat pointsMatrix = Mat(pts);
                Scalar color(0, 255, 0);

                Rect r0 = boundingRect(pointsMatrix);
                cout << r0.height << " ";
                rectangle(frame, r0, color, 2);

                ++itc;
            }
            else
            {
                ++itc;
            }
        }

        cout << endl;

        if (show_ui)
        {
            imshow(camera_video, frame);
            //imshow(processed_video, gray);
            //imshow(blurred_video, blur);
            //imshow(canny_video, canny);
            imshow(mog_video, foreground);
        }

        key = waitKey(1);

        cout << string(50, '\n')
            << 1000 / duration_cast<milliseconds>(high_resolution_clock::now() - t0).count()
            << " fps" << endl;
    }

    return 0;
}