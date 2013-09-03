#include "application.h"

using namespace std;

namespace FallDetector
{
Application::Application()
{
    mpProcessing = NULL;
    mRunning = false;
    mShowGui = false;
}

Application::~Application()
{
    delete mpProcessing;
}

void Application::Run()
{
    bool stop = false;

    while (!stop)
    {
        cout << "Running:\t\t" << mRunning << endl
             << "Camera number:\t\t" << mVideoProcessor.GetCamera() << endl
             << "Camera resolution:\t" << mVideoProcessor.GetResolution() << endl
             << "Show GUI:\t\t" << mShowGui << endl
             << "History:\t\t" << mVideoProcessor.GetHistory() << endl
             << "Threshold:\t\t" << mVideoProcessor.GetThreshold() << endl
             << "===================" << endl
             << "Available commands:" << endl;

        if(mRunning)
        {
            cout << "s - Stop processing" << endl
                 << "q - Quit" << endl
                 << ">>";
            char key;
            cin >> key;

            switch (key)
            {
            case 's':
                mpProcessing->interrupt();
                cout << "Waiting video processing thread.." << endl;
                mpProcessing->join();
                delete mpProcessing;
                mpProcessing = NULL;
                mRunning = false;
                break;
            case 'q':
                mpProcessing->interrupt();
                cout << "Waiting video processing thread.." << endl;
                mpProcessing->join();
                mRunning = false;
                stop = true;
                break;
            default:
                cout << "There is not such command!" << endl;
                break;
            }
        }
        else
        {
            cout << "s - Start processing" << endl
                 << "g - Show/hide GUI" << endl
                 << "r - Change resolution" << endl
                 << "c - Change camera" << endl
                 << "b - Create new backgound subtractor" << endl
                 << "q - Quit" << endl
                 << ">>";

            unsigned int camera_number = 0;
            int history = 0;
            float threshold = 0;
            char key;
            cin >> key;

            switch (key)
            {
            case 's':
                if(mShowGui)
                    mpProcessing = new boost::thread(&VideoProcessor::RunWithGui, &(mVideoProcessor));
                else
                    mpProcessing = new boost::thread(&VideoProcessor::RunWithoutGui, &(mVideoProcessor));

                mRunning = true;
                break;
            case 'g':
                mShowGui = !mShowGui;
                break;
            case 'r':
                handleResolution();
                break;
            case 'c':
                cout << "Input camera number: ";
                cin >> camera_number;
                mVideoProcessor.SetCamera(camera_number);
                break;
            case 'b':
                cout << "Input history: ";
                cin >> history;
                cout << "Input threshold: ";
                cin >> threshold;
                mVideoProcessor.CreateNewBackgroundSubtractor(history, threshold);
                break;
            case 'q':
                stop = true;
                break;
            default:
                cout << "There is not such command!" << endl;
                break;
            }
        }

        cout << endl;
    }
}

void Application::handleResolution()
{
    bool stop = false;

    while (!stop)
    {
        cout << endl << "Available resolutions:" << endl
             << "1 - 320x240" << endl
             << "2 - 640x480" << endl
             << "q - Quit" << endl
             << ">>";

        char key;
        cin >> key;

        switch (key)
        {
        case '1':
            mVideoProcessor.SetResolution(320, 240);
            break;
        case '2':
            mVideoProcessor.SetResolution(1024, 768);
            break;
        case 'q':
            stop = true;
            break;
        default:
            cout << "There is not such resolution!" << endl;
            break;
        }
    }
}
}
