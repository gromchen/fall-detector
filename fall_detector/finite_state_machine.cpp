#include "finite_state_machine.h"

#include <stdexcept>
#include <string>

using namespace std;

namespace FallDetector
{
FiniteStateMachine::FiniteStateMachine()
{
    mHumanState = NOT_DETECTED;
    mLyingCount = 0;
}

void FiniteStateMachine::MoveToState(HumanState humanState)
{
    if(humanState == LYING)
    {
        mLyingCount++;

        if(mLyingCount >= 5)
        {
            mHumanState = INJURED;
            return;
        }
    }
    else
    {
        mLyingCount = 0;
    }

    if(mHumanState == humanState)
    {
        return;
    }

    mHumanState = humanState;
}

void FiniteStateMachine::Reset()
{
    mHumanState = NOT_DETECTED;
    mLyingCount = 0;
}
}
