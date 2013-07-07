#include <iostream>

#ifdef _WIN32
#include <vld.h> // Visual Leak Detector
#endif

#include <opencv2/core/core.hpp> // Basic OpenCV data structures
#include <opencv2/highgui/highgui.hpp> // OpenCV interface
#include <opencv2/imgproc/imgproc.hpp> // Image processing module

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    Mat image;
    Mat gray;
    char key = 0;

    VideoCapture capture(0);

    if (!capture.isOpened())
    {
        cout << "Failed to open a video device or video file!" << endl;
        return 1;
    }

    capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

    namedWindow("Camera Video", CV_WINDOW_AUTOSIZE);
    namedWindow("Processed Video", CV_WINDOW_AUTOSIZE);

    while (key != 'q')
    {
        capture >> image;

        cvtColor(image, gray, CV_BGR2GRAY);

        imshow("Camera Video", image);
        imshow("Processed Video", gray);

        key = waitKey(25);
    }

    return 0;
}