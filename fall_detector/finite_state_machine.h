#ifndef FINITE_STATE_MACHINE_H
#define FINITE_STATE_MACHINE_H

namespace FallDetector
{
class FiniteStateMachine
{
public:
    FiniteStateMachine();

    void Walk();
    void Fall();
    void Lie();

    bool FallDetected() { return mFallDetected; }

private:
    enum StateType
    {
        WALKING,
        FALLING,
        LYING
    };

    StateType mState;
    unsigned int mFallCount;
    bool mFallDetected;
};
}
#endif // FINITE_STATE_MACHINE_H
