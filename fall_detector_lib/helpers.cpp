#include "helpers.h"

using namespace std;

namespace FallDetector
{
string HumanStateToString(HumanState humanState)
{
    string state = "";

    switch (humanState)
    {
    case NOT_DETECTED:
        state = "not detected";
        break;
    case STANDING:
        state = "standing";
        break;
    case WALKING:
        state = "walking";
        break;
    case FALLING:
        state = "falling";
        break;
    case LYING:
        state = "lying";
        break;
    case INJURED:
        state = "injured";
        break;
    default:
        state = "[not set]";
        break;
    }

    return state;
}
}
