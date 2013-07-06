#ifdef _WIN32
#include <vld.h> // Visual Leak Detector
#endif

#include "utilities.h"

#include <opencv2/highgui/highgui.hpp> // OpenCV interface
#include <opencv2/imgproc/imgproc.hpp> // Image processing module

using namespace cv;
using namespace fd;

int main(int argc, char** argv)
{
    Mat original_image = imread("lena.png");
    namedWindow("Original Image", WINDOW_AUTOSIZE);
    imshow("Original Image", original_image);

    Mat hidden_image = imread("girlface.png");
    namedWindow("Hidden Image", WINDOW_AUTOSIZE);
    imshow("Hidden Image", hidden_image);

    Mat steged_image;
    steganograph(original_image, hidden_image, steged_image);
    namedWindow("Steganagraphed Image", WINDOW_AUTOSIZE);
    imshow("Steganagraphed Image", steged_image);

    Mat front_image;
    Mat unsteged_image;
    get_original_images(steged_image, front_image, unsteged_image);

    namedWindow("Front Image", WINDOW_AUTOSIZE);
    imshow("Front Image", front_image);

    namedWindow("Unsteganographed Image", WINDOW_AUTOSIZE);
    imshow("Unsteganographed Image", unsteged_image);

    waitKey(0);

    return 0;
}