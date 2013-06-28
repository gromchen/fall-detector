// Visual Leak Detector
#include <vld.h>

// OpenCV
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

int main()
{
    cv::Mat image = cv::imread("img.jpg"); // TODO: cannot find file
    cv::namedWindow("My Image");
    cv::imshow("My Image", image);
    cv::waitKey(5000);

    return 0;
}