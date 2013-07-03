#ifdef _WIN32
#include <vld.h> // Visual Leak Detector
#endif

#include <opencv2/core/core.hpp> // Basic OpenCV data structures
#include <opencv2/highgui/highgui.hpp> // OpenCV interface
#include <opencv2/imgproc/imgproc.hpp> // Image processing module

/// <summary>
/// Thresholds image
/// </summary>
/// <param name="image">Image</param>
/// <param name="threshValue">Thresh value</param>
/// <returns>Nothing</returns>
void thresholding(cv::Mat &image, uchar threshValue)
{
    if (image.rows == -1 && image.cols == -1)
    {
        return;
    }

    for (size_t j = 0; j < (size_t) image.rows; j++)
    {
        for (size_t i = 0; i < (size_t) image.cols; i++)
        {
            if (image.channels() == 1)
            {
                // Grayscale image
                uchar value = image.at<uchar>(j, i);

                if (value > threshValue)
                {
                    image.at<uchar>(j, i) = 255;
                }
            }
            else
            {
                if (image.channels() == 3)
                {
                    // Color image
                    uchar averageValue = static_cast<uchar>((
                        image.at<cv::Vec3b>(j, i)[0] +
                        image.at<cv::Vec3b>(j, i)[1] +
                        image.at<cv::Vec3b>(j, i)[2]
                        ) / 3);

                    if (averageValue > threshValue)
                    {
                        image.at<cv::Vec3b>(j, i)[0] = 255;
                        image.at<cv::Vec3b>(j, i)[1] = 255;
                        image.at<cv::Vec3b>(j, i)[2] = 255;
                    }
                }
            }
        }
    }
}

int main()
{
    cv::Mat image;

    image = cv::imread("lena.png");
    cv::namedWindow("Original Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Original Image", image);

    thresholding(image, 150);
    cv::namedWindow("Threshold Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Threshold Image", image);

    cv::waitKey(0);

    return 0;
}