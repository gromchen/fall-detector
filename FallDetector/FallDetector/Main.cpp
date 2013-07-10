#include <iostream>
#include <thread>

#ifdef _WIN32
#include <vld.h> // Visual Leak Detector
#endif

#include "VideoProcessor.h"

using namespace std;
using namespace FallDetector;

int main(int argc, char** argv)
{
    try
    {
        VideoProcessor videoProcessor{};

        thread videoProcessing{ &VideoProcessor::Run, &videoProcessor };

        while (true)
        {
            char key = 0; 
            cin >> key;
            videoProcessor.SetKey(key);

            if (key == 'q')
            {
                break;
            }
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