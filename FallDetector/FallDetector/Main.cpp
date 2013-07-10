#include <iostream>

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

        videoProcessor.Run();
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