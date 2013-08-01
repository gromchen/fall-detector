#ifndef FALL_DETECTOR_APPLICATION_H
#define FALL_DETECTOR_APPLICATION_H


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
        void handleMainCommands();
        void handleResolution();

        VideoProcessor m_videoProcessor;
    };
}

#endif // FALL_DETECTOR_APPLICATION_H
