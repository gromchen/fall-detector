#ifndef DATA_COLLECTOR_H
#define DATA_COLLECTOR_H

#include <vector>
#include <boost/thread.hpp>

#include "interval_data.h"

namespace FallDetector
{
class DataCollector
{
public:
    DataCollector();
    ~DataCollector();

    void CollectData(IntervalData intervalData);

private:
    void writeToFile(std::string fileName, std::vector<IntervalData> data);
    void joinAllWriteThreads();

    std::vector<IntervalData> mData;
    std::vector<boost::thread*> mWriteThreads;
    boost::mutex mFileMutex;
};
}

#endif // DATA_COLLECTOR_H
