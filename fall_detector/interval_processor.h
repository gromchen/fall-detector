#ifndef INTERVAL_PROCESSOR_H
#define INTERVAL_PROCESSOR_H

#include <boost/chrono.hpp>

#include "data_collector.h"
#include "frame_data.h"
//#include "finite_state_machine.h"

namespace FallDetector
{
class IntervalProcessor
{
public:
    IntervalProcessor();

    void StartTracking();
    void IncludeObject(FallDetector::FrameData frameData);
    //bool FallDetected() { return mFiniteStateMachine.FallDetected(); }

private:
    FallDetector::DataCollector mDataCollector;
    boost::chrono::high_resolution_clock::time_point mTimeOfPreviousSecond;
    std::vector<FrameData> mFrameDataCollection;
    //FiniteStateMachine mFiniteStateMachine;
};
}

#endif // INTERVAL_PROCESSOR_H
