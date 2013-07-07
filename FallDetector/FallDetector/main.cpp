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
    namedWindow("Blurred Video", CV_WINDOW_AUTOSIZE);
    namedWindow("Canny Video", CV_WINDOW_AUTOSIZE);

    Mat image;
    Mat gray;
    Mat blur;
    Mat canny;
    char key = 0;

    while (key != 'q')
    {
        capture >> image;
        imshow("Camera Video", image);

        cvtColor(image, gray, CV_BGR2GRAY);
        imshow("Processed Video", gray);

        GaussianBlur(gray, blur, Size(5, 5), 0);
        imshow("Blurred Video", blur);

        Canny(blur, canny, 10, 100);
        imshow("Canny Video", canny);

        key = waitKey(25);
    }

    return 0;
}