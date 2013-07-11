#include <iostream>
#include <thread>

#ifdef _WIN32
#include <vld.h> // Visual Leak Detector
#include <conio.h>
#endif

#ifdef __linux__
#include <curses.h>
#endif

#include <opencv2/highgui/highgui.hpp>

#include "VideoProcessor.h"

using namespace std;
using namespace cv;
using namespace FallDetector;

int main(int argc, char** argv)
{
    try
    {
        VideoProcessor videoProcessor{};
        thread videoProcessing{ &VideoProcessor::Run, &videoProcessor };

        while (true)
        {
            if (_kbhit())
            {
                char key = _getch();

                videoProcessor.SetKey(key);

                if (key == 'q')
                {
                    break;
                }
            }

            cout << videoProcessor.GetHeight() << endl;
        }

        cout << "Waiting video processing thread..." << endl;

        videoProcessing.join();
    }
    catch (runtime_error ex)
    {
        cout << ex.what() << endl;
        return 1;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return 1;
    }

    return 0;
}