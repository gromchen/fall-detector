#ifndef VIDEO_DATA_H
#define VIDEO_DATA_H

#include <boost/date_time.hpp>
#include <boost/optional/optional.hpp>

#include "helpers.h"
#include "parameters.h"

namespace FallDetector
{
class IntervalData
{
public:
    IntervalData(boost::posix_time::ptime currentTime, double fps, Parameters features, bool fallDetected,
                 HumanStateType state, std::vector<boost::optional<double> > angles);

    boost::posix_time::ptime GetCurrentTime() { return mCurrentTime; }
    double GetFps() { return mFps; }
    Parameters GetFeatures() { return mFeatures; }
    int GetNumberOfFoundObjects() { return mFeatures.GetNumberOfSummands(); }
    bool FallDetected() { return mFallDetected; }
    HumanStateType GetState() { return mState; }
    std::vector<boost::optional<double> > GetAngles() { return mAngles; }

private:
    boost::posix_time::ptime mCurrentTime;
    double mFps;
    Parameters mFeatures;
    bool mFallDetected;
    HumanStateType mState;
    std::vector<boost::optional<double> > mAngles;
};
}
#endif // VIDEO_DATA_H
