#include "background_subtractor_fd.h"

using namespace cv;

namespace FallDetector
{
    BackgroundSubtractorFD::BackgroundSubtractorFD(int hey, float hey2) :
        BackgroundSubtractorMOG2()
    {
        nmixtures = hey;
        fTau = hey2;
    }
}
