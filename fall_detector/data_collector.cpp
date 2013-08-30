#include "data_collector.h"

#include <fstream>
#include <sstream>

#include "helpers.h"

using namespace std;
using namespace boost;

namespace FallDetector
{
DataCollector::DataCollector()
{
}

DataCollector::~DataCollector()
{
    joinAllWriteThreads();
}

void DataCollector::CollectData(IntervalData intervalData)
{
    mData.push_back(intervalData);

    if(mData.size() >= 10)
    {
        if(mWriteThreads.size() >= 100)
            joinAllWriteThreads();

        string date = ToString(posix_time::second_clock::local_time().date());
        mWriteThreads.push_back(new thread(&DataCollector::writeToFile, this, "data (" + date + ").csv", mData));
        mData.clear();
    }
}

void DataCollector::writeToFile(string fileName, vector<IntervalData> data)
{
    ofstream file_stream;
    mFileMutex.lock();
    file_stream.open(fileName.c_str(), ios_base::app);
    unsigned int data_size = data.size();

    for(unsigned int iData = 0; iData < data_size; iData++)
    {
        file_stream << data[iData].GetCurrentTime().time_of_day() << ","
                    << data[iData].GetFps() << ",";

        if(data[iData].GetNumberOfFoundObjects() > 0)
        {
            Features features = data[iData].GetFeatures();
            file_stream << features.GetCoefficientOfMotion().GetStandardDeviation() << ","
                        << features.GetOrientation().GetStandardDeviation() << ","
                        << features.GetRatio().GetStandardDeviation() << ","
                        << features.GetPositionX().GetStandardDeviation() << ","
                        << features.GetPositionY().GetStandardDeviation() << ","
                        << features.GetAxisA().GetStandardDeviation() << ","
                        << features.GetAxisB().GetStandardDeviation() << ","
                        << data[iData].GetNumberOfFoundObjects() << ",";

            string state = "";

            if(data[iData].FallDetected())
            {
                state = "Fall detected";
            }
            else
            {
                switch (data[iData].GetState())
                {
                case STANDING:
                    state = "Standing";
                    break;
                case WALKING:
                    state = "Walking";
                    break;
                case FALLING:
                    state = "Falling";
                    break;
                case LYING:
                    state = "Lying";
                    break;
                default:
                    break;
                }
            }

            file_stream << state << endl;
        }
        else
            file_stream << "," << "," << "," << "," << "," << "," << ","
                        << data[iData].GetNumberOfFoundObjects() << "," << endl;
    }

    file_stream.close();
    mFileMutex.unlock();
}

void DataCollector::joinAllWriteThreads()
{
    unsigned int thread_size = mWriteThreads.size();

    for(unsigned int iThread = 0; iThread < thread_size; iThread++)
    {
        mWriteThreads[iThread]->join();
        delete mWriteThreads[iThread];
    }

    mWriteThreads.clear();
}
}

