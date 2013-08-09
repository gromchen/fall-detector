#ifndef FALL_DETECTOR_APPLICATION_H
#define FALL_DETECTOR_APPLICATION_H

#include <boost/thread.hpp>

#include "video_processor.h"

namespace FallDetector
{
class Application
{
public:
    Application();
    ~Application();

    void Run();

private:
    void handleResolution();

    VideoProcessor mVideoProcessor;
    boost::thread* mpProcessing;
    bool mIsRunning;
    bool mShowGui;
};
}

#endif // FALL_DETECTOR_APPLICATION_H
