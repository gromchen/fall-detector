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
    bool IsFallDetected() { return mFallDetected; }
    HumanState GetHumanState() { return mHumanState; }
    void Reset();

private:
    HumanState mHumanState;
    unsigned int mLyingCount;
    bool mFallDetected;
};
}
#endif // FALL_DETECTOR_FINITE_STATE_MACHINE_H
