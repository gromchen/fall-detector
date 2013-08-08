#include "mog2_public.h"

using namespace cv;

namespace FallDetector
{
    Mog2Public::Mog2Public() :
        BackgroundSubtractorMOG2()
    {
    }

    void Mog2Public::SetTau(float tau)
    {
        fTau = tau;
    }
}
