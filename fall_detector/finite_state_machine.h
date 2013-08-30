#ifndef FINITE_STATE_MACHINE_H
#define FINITE_STATE_MACHINE_H

#include "helpers.h"

namespace FallDetector
{
class FiniteStateMachine
{
public:
    FiniteStateMachine();

    void Stand();
    void Walk();
    void Fall();
    void Lie();

    bool FallDetected() { return mFallDetected; }
    HumanStateType GetState() { return mState; }

    void Reset();

private:
    HumanStateType mState;
    unsigned int mFallCount;
    bool mFallDetected;
};
}
#endif // FINITE_STATE_MACHINE_H
