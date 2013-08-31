#include "finite_state_machine.h"

#include <stdexcept>

using namespace std;

namespace FallDetector
{
FiniteStateMachine::FiniteStateMachine()
{
    mHumanState = STANDING;
    mLyingCount = 0;
    mFallDetected = false;
}

void FiniteStateMachine::MoveToState(HumanState humanState)
{
    if(humanState == LYING)
    {
        mLyingCount++;

        if(mLyingCount >= 5)
        {
            mFallDetected = true;
        }
    }

    if(mHumanState == humanState)
    {
        return;
    }

    mHumanState = humanState;
}

void FiniteStateMachine::Reset()
{
    mHumanState = STANDING;
    mLyingCount = 0;
    mFallDetected = false;
}
}
