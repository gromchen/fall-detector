#include <iostream>

#include "video_processor.h"

using namespace std;
using namespace cv;
using namespace FallDetector;

int main()
{
    try
    {
        VideoProcessor videoProcessor;
        videoProcessor.Run();

        //videoProcessor.SetKey(key);

        //cout << videoProcessor.GetHeight() << endl;
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