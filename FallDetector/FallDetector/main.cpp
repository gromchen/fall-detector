#include <iostream>
#include <chrono>

#ifdef _WIN32
#include <vld.h> // Visual Leak Detector
#endif

#include <opencv2/core/core.hpp> // Basic OpenCV data structures
#include <opencv2/highgui/highgui.hpp> // OpenCV interface
#include <opencv2/imgproc/imgproc.hpp> // Image processing module

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
    bool show_ui = false;

    while (key != 'q')
    {
        auto t0 = high_resolution_clock::now();

        if (key == 'w')
        {
            show_ui = !show_ui;
        }

        capture >> image;
        cvtColor(image, gray, CV_BGR2GRAY);
        GaussianBlur(gray, blur, Size(5, 5), 0);
        Canny(blur, canny, 10, hey);
        
        if (show_ui)
        {
            imshow(camera_video, image);
            imshow(processed_video, gray);
            imshow(blurred_video, blur);
            imshow(canny_video, canny);
        }

        key = waitKey(1);

        system("CLS"); // TODO: remove
        cout << 1000 / duration_cast<milliseconds>(high_resolution_clock::now() - t0).count() << " fps" << endl;
    }

    return 0;
}