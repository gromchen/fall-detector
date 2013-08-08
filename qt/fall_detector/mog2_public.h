#ifndef FALL_DETECTOR_BACKGROUND_SUBTRACTOR_FD_H
#define FALL_DETECTOR_BACKGROUND_SUBTRACTOR_FD_H

#include <opencv2/opencv.hpp>

namespace FallDetector
{
    class Mog2Public : public cv::BackgroundSubtractorMOG2
    {
        public:
            Mog2Public();

            void SetTau(float tau);
        protected:
        private:
    };
}

#endif // FALL_DETECTOR_BACKGROUND_SUBTRACTOR_FD_H
