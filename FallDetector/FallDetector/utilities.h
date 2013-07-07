#ifndef FD_UTILITIES_H
#define FD_UTILITIES_H

#include <opencv2/core/core.hpp> // Basic OpenCV data structures

namespace fd
{
    /// <summary>
    /// Thresholds image
    /// </summary>
    /// <param name="image">Image</param>
    /// <param name="thresh_value">Thresh value</param>
    /// <returns>Nothing</returns>
    void thresholding(cv::Mat &image, uchar thresh_value);

    /// <summary>
    /// Puts image inside another
    /// </summary>
    /// <param name="front_image">Image which must be visible</param>
    /// <param name="hidden_image">Image which will be hidden</param>
    /// <param name="output_image">Resulting image</param>
    /// <returns>Nothing</returns>
    void steganograph(const cv::Mat& front_image, const cv::Mat& hidden_image, cv::Mat& output_image);

    /// <summary>
    /// Gets hidden and original images from steganographed iamge
    /// </summary>
    /// <param name="steged_image">Steganographed image</param>
    /// <param name="front_image">Image which was visible</param>
    /// <param name="hidden_image">Image which was hidden</param>
    /// <returns>Nothing</returns>
    void get_original_images(const cv::Mat& steged_image, cv::Mat& front_image, cv::Mat& hidden_image);
}

#endif // !FD_UTILITIES_H