#ifndef FALL_DETECTOR_FINITE_STATE_MACHINE_H
#define FALL_DETECTOR_FINITE_STATE_MACHINE_H

#include "helpers.h"

namespace FallDetector
{
class FiniteStateMachine
{
public:
    FiniteStateMachine();

    void MoveToState(HumanState humanState);
    HumanState GetCurrentHumanState() { return mHumanState; }
    void Reset();

private:
    HumanState mHumanState;
    unsigned int mLyingCount;
};
}
#endif // FALL_DETECTOR_FINITE_STATE_MACHINE_H
