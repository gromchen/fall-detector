#include <iostream>

#ifdef _WIN32
#include <vld.h> // Visual Leak Detector
#endif

#include <opencv2/core/core.hpp> // Basic OpenCV data structures
#include <opencv2/highgui/highgui.hpp> // OpenCV interface
#include <opencv2/imgproc/imgproc.hpp> // Image processing module

using namespace std;
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
        cout << "Failed to open a video device or video file!" << endl;
        return 1;
    }

    capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

    int value = 50;
    int value2 = 0;

    string camera_video = "Camera Video";
    string processed_video = "Processed Video";
    string blurred_video = "Blurred Video";
    string canny_video = "Canny Video";

    namedWindow(camera_video, CV_WINDOW_AUTOSIZE);
    namedWindow(processed_video, CV_WINDOW_AUTOSIZE);
    namedWindow(blurred_video, CV_WINDOW_AUTOSIZE);
    namedWindow(canny_video, CV_WINDOW_AUTOSIZE);

    createTrackbar("track1", camera_video, &value, 255, OnChange);

    Mat image;
    Mat gray;
    Mat blur;
    Mat canny;

    char key = 0;

    while (key != 'q')
    {
        capture >> image;
        imshow(camera_video, image);

        cvtColor(image, gray, CV_BGR2GRAY);
        imshow(processed_video, gray);

        GaussianBlur(gray, blur, Size(5, 5), 0);
        imshow(blurred_video, blur);

        Canny(blur, canny, 10, hey);
        imshow(canny_video, canny);

        key = waitKey(25);
    }

    return 0;
}