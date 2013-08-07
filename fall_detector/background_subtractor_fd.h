#ifndef FALL_DETECTOR_BACKGROUND_SUBTRACTOR_FD_H
#define FALL_DETECTOR_BACKGROUND_SUBTRACTOR_FD_H

#include <opencv2/opencv.hpp>

namespace FallDetector
{
    class BackgroundSubtractorFD : public cv::BackgroundSubtractorMOG2
    {
        public:
            BackgroundSubtractorFD(int hey, float hey2);
        protected:
        private:
    };
}

#endif // FALL_DETECTOR_BACKGROUND_SUBTRACTOR_FD_H
