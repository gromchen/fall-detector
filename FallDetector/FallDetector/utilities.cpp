#include "utilities.h"

#include <iostream>

using namespace std;
using namespace cv;

void fd::thresholding(Mat &image, uchar thresh_value)
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

                if (value > thresh_value)
                {
                    image.at<uchar>(j, i) = 255;
                }
            }
            else
            {
                if (image.channels() == 3)
                {
                    // Color image
                    uchar average_value = static_cast<uchar>((
                        image.at<Vec3b>(j, i)[0] +
                        image.at<Vec3b>(j, i)[1] +
                        image.at<Vec3b>(j, i)[2]
                        ) / 3);

                    if (average_value > thresh_value)
                    {
                        image.at<Vec3b>(j, i)[0] = 255;
                        image.at<Vec3b>(j, i)[1] = 255;
                        image.at<Vec3b>(j, i)[2] = 255;
                    }
                }
            }
        }
    }
}

void fd::steganograph(const Mat& front_image, const Mat& hidden_image, Mat& output_image)
{
    if (front_image.type() != hidden_image.type())
    {
        cout << "Types of given images are different" << endl;
        return;
    }

    if (front_image.size() != hidden_image.size())
    {
        cout << "Sizes of given images are different" << endl;
        return;
    }

    output_image.create(front_image.rows, front_image.cols, front_image.type());

    Mat mask(front_image.rows, front_image.cols, front_image.type(), Scalar(0xF0, 0xF0, 0xF0));

    Mat front_image_with_MSB;
    bitwise_and(front_image, mask, front_image_with_MSB);
    
    Mat hidden_image_with_MSB;
    bitwise_and(hidden_image, mask, hidden_image_with_MSB);

    for (size_t j = 0; j < (size_t) front_image.rows; j++)
    {
        for (size_t i = 0; i < (size_t) front_image.rows; i++)
        {
            hidden_image_with_MSB.at<Vec3b>(j, i)[0] = hidden_image_with_MSB.at<Vec3b>(j, i)[0] >> 4;
            hidden_image_with_MSB.at<Vec3b>(j, i)[1] = hidden_image_with_MSB.at<Vec3b>(j, i)[1] >> 4;
            hidden_image_with_MSB.at<Vec3b>(j, i)[2] = hidden_image_with_MSB.at<Vec3b>(j, i)[2] >> 4;
        }
    }

    bitwise_or(front_image_with_MSB, hidden_image_with_MSB, output_image);
}

void fd::get_original_images(const cv::Mat& steged_image, cv::Mat& front_image, cv::Mat& hidden_image)
{
    front_image.create(steged_image.rows, steged_image.cols, steged_image.type());
    hidden_image.create(steged_image.rows, steged_image.cols, steged_image.type());

    Mat temp_front_image;
    Mat temp_hidden_image;

    Mat front_mask(steged_image.rows, steged_image.cols, steged_image.type(), Scalar(0xF0, 0xF0, 0xF0));
    Mat hidden_mask(steged_image.rows, steged_image.cols, steged_image.type(), Scalar(0x0F, 0x0F, 0x0F));

    bitwise_and(steged_image, front_mask, front_image);
    bitwise_and(steged_image, hidden_mask, hidden_image);

    for (size_t j = 0; j < (size_t) steged_image.rows; j++)
    {
        for (size_t i = 0; i < (size_t) steged_image.cols; i++)
        {
            hidden_image.at<Vec3b>(j, i)[0] = hidden_image.at<Vec3b>(j, i)[0] << 4;
            hidden_image.at<Vec3b>(j, i)[1] = hidden_image.at<Vec3b>(j, i)[1] << 4;
            hidden_image.at<Vec3b>(j, i)[2] = hidden_image.at<Vec3b>(j, i)[2] << 4;
        }
    }
}