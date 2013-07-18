#include <thread>

#include "video_processor.h"

using namespace std;
using namespace cv;
using namespace FallDetector;

int main()
{
    try
    {
        VideoProcessor videoProcessor;
		thread processing(&VideoProcessor::Run, &videoProcessor);

		while (true)
        {
			char key;
            
			cin >> key;
			videoProcessor.SetKey(key);

            if (key == 'q')
            {
                break;
            }
        }

		cout << "Waiting video processing thread.." << endl;
		processing.join();
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