#include "video_processor.h"

using namespace std;
using namespace cv;
using namespace FallDetector;

int main()
{
    try
    {
        VideoProcessor videoProcessor;
        while (true)
        {
            videoProcessor.Run();

            char key = waitKey(25);

            if (key == 'q')
            {
                break;
            }
        }

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