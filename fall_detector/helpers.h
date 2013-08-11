#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <boost/date_time.hpp>

namespace FallDetector
{
struct VideoData
{
public:
    boost::posix_time::ptime mCurrentTime;
    double mFps;
    float mEllipseHeight;
};

std::string IntToString(int number);
}

#endif // HELPERS_H
