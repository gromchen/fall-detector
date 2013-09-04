#ifndef FALL_DETECTOR_DATA_COLLECTOR_H
#define FALL_DETECTOR_DATA_COLLECTOR_H

#include <vector>
#include <boost/thread.hpp>

#include "interval_info.h"

namespace FallDetector
{
class DataCollector
{
public:
    DataCollector();
    ~DataCollector();

    void Collect(IntervalInfo intervalInfo);

private:
    void Write(std::string fileName, std::vector<IntervalInfo> intervalInfos);
    void JoinAllWriteThreads();

    std::vector<IntervalInfo> mIntervalInfos;
    std::vector<boost::thread*> mWriteThreads;
    boost::mutex mFileMutex;
};
}

#endif // FALL_DETECTOR_DATA_COLLECTOR_H
