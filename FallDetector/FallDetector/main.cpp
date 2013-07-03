#ifdef _WIN32
#include <vld.h> // Visual Leak Detector
#endif

#include <opencv2/core/core.hpp> // Basic OpenCV data structures
#include <opencv2/highgui/highgui.hpp> // OpenCV interface
#include <opencv2/imgproc/imgproc.hpp> // Image processing module

int main()
{
    cv::Mat originalImage, resizedImage, savedImage;
    
    originalImage = cv::imread("lena.png");
    cv::namedWindow("Original Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Original Image", originalImage);

    cv::resize(originalImage, resizedImage, cv::Size(), 0.5, 0.5, cv::INTER_LINEAR);
    cv::namedWindow("Resized Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Resized Image", resizedImage);

    cv::imwrite("saved.png", resizedImage);
    cv::namedWindow("Image Saved", cv::WINDOW_AUTOSIZE);
    savedImage = cv::imread("saved.png");
    cv::imshow("Image Saved", savedImage);

    cv::waitKey(0);

    return 0;
}