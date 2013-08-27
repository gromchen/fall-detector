#include "finite_state_machine.h"

#include <stdexcept>

using namespace std;

namespace FallDetector
{
FiniteStateMachine::FiniteStateMachine()
{
    mState = WALKING;
    mFallCount = 0;
    mFallDetected = false;
}

void FiniteStateMachine::Walk()
{
    if(mState != WALKING)
        mState = WALKING;
}

void FiniteStateMachine::Fall()
{
    if(mState == LYING)
        throw runtime_error("Fall error");

    if(mState != FALLING)
        mState = FALLING;
}

void FiniteStateMachine::Lie()
{
    if(mState == WALKING)
        throw runtime_error("Lie error");

    if(mState != LYING)
        mState = LYING;

    mFallCount++;

    if(mFallCount >= 5)
        mFallDetected = true;
}
}
