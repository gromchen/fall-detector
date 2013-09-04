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
    JoinAllWriteThreads();
}

void DataCollector::Collect(IntervalInfo intervalInfo)
{
    mIntervalInfos.push_back(intervalInfo);

    if(mIntervalInfos.size() >= 10)
    {
        if(mWriteThreads.size() >= 100)
        {
            JoinAllWriteThreads();
        }

        string date_string = ToString(posix_time::second_clock::local_time().date());
        mWriteThreads.push_back(new thread(&DataCollector::Write,
                                           this,
                                           "data (" + date_string + ").csv",
                                           mIntervalInfos));
        mIntervalInfos.clear();
    }
}

void DataCollector::Write(string fileName, std::vector<IntervalInfo> intervalInfos)
{
    ofstream file_stream_for_parameters;
    ofstream file_stream_for_angles;

    mFileMutex.lock();

    file_stream_for_parameters.open(fileName.c_str(), ios_base::app);
    file_stream_for_angles.open(("frames_" + fileName).c_str(), ios_base::app);

    for(unsigned int i_interval_info = 0; i_interval_info < intervalInfos.size(); i_interval_info++)
    {
        IntervalInfo interval_info = intervalInfos[i_interval_info];

        file_stream_for_parameters << interval_info.GetCurrentTime().time_of_day() << ","
                                   << interval_info.GetFps() << ",";

        if(interval_info.GetNumberOfFoundObjects() > 0)
        {
            FeatureCollection feature_collection = interval_info.GetFeatureCollection();
            file_stream_for_parameters << feature_collection.GetCoefficientOfMotion().GetStandardDeviation() << ","
                                       << feature_collection.GetOrientation().GetStandardDeviation() << ","
                                       << feature_collection.GetRatio().GetStandardDeviation() << ","
                                       << feature_collection.GetPositionX().GetStandardDeviation() << ","
                                       << feature_collection.GetPositionY().GetStandardDeviation() << ","
                                       << feature_collection.GetAxisA().GetStandardDeviation() << ","
                                       << feature_collection.GetAxisB().GetStandardDeviation() << ","
                                       << interval_info.GetNumberOfFoundObjects() << ","
                                       << HumanStateToString(interval_info.GetHumanState()) << endl;
        }
        else
        {
            file_stream_for_parameters << "," << "," << "," << "," << "," << "," << ","
                                       << interval_info.GetNumberOfFoundObjects() << ","
                                       << HumanStateToString(interval_info.GetHumanState()) << endl;
        }

        for(unsigned int i_angle = 0; i_angle < interval_info.GetAngles().size(); i_angle++)
        {
            if(interval_info.GetAngles()[i_angle])
            {
                file_stream_for_angles << interval_info.GetAngles()[i_angle].get() << endl;
            }
            else
            {
                file_stream_for_angles << endl;
            }
        }
    }

    file_stream_for_parameters.close();
    file_stream_for_angles.close();
    mFileMutex.unlock();
}

void DataCollector::JoinAllWriteThreads()
{
    for(unsigned int i_thread = 0; i_thread < mWriteThreads.size(); i_thread++)
    {
        mWriteThreads[i_thread]->join();
        delete mWriteThreads[i_thread];
    }

    mWriteThreads.clear();
}
}

