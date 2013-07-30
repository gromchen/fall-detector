#ifndef FALL_DETECTOR_APPLICATION_H
#define FALL_DETECTOR_APPLICATION_H

#include "video_processor.h"

namespace FallDetector
{
    /// <summary>
    /// Main application
    /// </summary>
    class Application
    {
    public:
        /// <summary>
        /// Default constructor
        /// </summary>
        Application();

        /// <summary>
        /// Destructor
        /// </summary>
        ~Application();

        /// <summary>
        /// Runs application
        /// </summary>
        void Run();

    private:
        /// <summary>
        /// Hendles command of main menu
        /// </summary>
        void handleMainCommands();

        /// <summary>
        /// Handles resolution commands
        /// </summary>
        void handleResolution();

        /// <summary>
        /// Video processor
        /// </summary>
        VideoProcessor m_videoProcessor;
    };
}

#endif // !FALL_DETECTOR_APPLICATION_H
