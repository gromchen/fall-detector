#ifdef _WIN32
#include <vld.h> // Visual Leak Detector
#endif

#include "utilities.h"

#include <opencv2/highgui/highgui.hpp> // OpenCV interface
#include <opencv2/imgproc/imgproc.hpp> // Image processing module

using namespace cv;

int main(int argc, char** argv)
{
    Mat image, gray, edge, cedge;
    int edgeThresh = 10;

    image = imread("lena.png");
    gray.create(image.rows, image.cols, CV_8UC1);
    cvtColor(image, gray, CV_BGR2GRAY);

    cedge.create(gray.size(), gray.type());

    Canny(gray, edge, edgeThresh, edgeThresh * 3, 3);
    cedge = Scalar::all(0);

    image.copyTo(cedge, edge);

    namedWindow("Output Image", CV_WINDOW_AUTOSIZE);
    imshow("Output Image", cedge);

    waitKey(0);

    return 0;
}