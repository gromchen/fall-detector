#ifndef FALL_DETECTOR_INTERVAL_PROCESSOR_H
#define FALL_DETECTOR_INTERVAL_PROCESSOR_H

#include <boost/chrono.hpp>

#include "data_collector.h"
#include "frame_info.h"
#include "finite_state_machine.h"

namespace FallDetector
{
class IntervalProcessor
{
public:
    IntervalProcessor();

    void StartTracking();
    void Include(FrameInfo frameInfo);
    void Reset();

    HumanState GetHumanState() { return mFiniteStateMachine.GetHumanState(); }

private:    
    DataCollector mDataCollector;
    boost::chrono::high_resolution_clock::time_point mTimeOfPreviousSecond;
    std::vector<FrameInfo> mFrameInfos;
    FiniteStateMachine mFiniteStateMachine;
};
}

#endif // FALL_DETECTOR_INTERVAL_PROCESSOR_H
