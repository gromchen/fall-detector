#ifndef VIDEO_DATA_H
#define VIDEO_DATA_H

#include <boost/date_time.hpp>

namespace FallDetector
{
class IntervalData
{
public:
    IntervalData(boost::posix_time::ptime currentTime, double fps, double standardDeviatioinOfOrientation,
              double standardDeviationOfRatio, int numberOfFoundObjects);

    boost::posix_time::ptime GetCurrentTime() { return mCurrentTime; }
    double GetFps() { return mFps; }
    float GetStandardDeviationOfOrientation() { return mStandardDeviationOfOrientation; }
    float GetStandardDeviationOfRatio() { return mStandardDeviationOfRatio; }
    int GetNumberOfFoundObjects() { return mNumberOfFoundObjects; }

private:
    boost::posix_time::ptime mCurrentTime;
    double mFps;
    float mStandardDeviationOfOrientation;
    float mStandardDeviationOfRatio;
    int mNumberOfFoundObjects;
};
}
#endif // VIDEO_DATA_H
