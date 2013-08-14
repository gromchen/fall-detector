#ifndef INTERVAL_PROCESSOR_H
#define INTERVAL_PROCESSOR_H

#include <boost/chrono.hpp>

#include "data_collector.h"
#include "frame_data.h"

namespace FallDetector
{
class IntervalProcessor
{
public:
    IntervalProcessor();

    void StartTracking();
    void IncludeObject(FallDetector::FrameData frameData);

private:
    FallDetector::DataCollector mDataCollector;
    boost::chrono::high_resolution_clock::time_point mTimeOfPreviousSecond;
    std::vector<FrameData> mFrameDataCollection;
};
}

#endif // INTERVAL_PROCESSOR_H
