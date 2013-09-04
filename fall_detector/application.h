#ifndef FALL_DETECTOR_APPLICATION_H
#define FALL_DETECTOR_APPLICATION_H

#include <boost/thread.hpp>

#include "fall_detector_lib.h"

class Application
{
public:
    Application();
    ~Application();

    void Run();

private:
    void handleResolution();

    FallDetector::VideoProcessor mVideoProcessor;
    boost::thread* mpProcessing;
    bool mRunning;
    bool mShowGui;
};

#endif // FALL_DETECTOR_APPLICATION_H
