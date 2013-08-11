#include "application.h"

using namespace std;
using namespace boost;

namespace FallDetector
{
Application::Application()
{
    mpProcessing = NULL;
    mIsRunning = false;
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
        cout << "Running: " << mIsRunning << "; "
             << "Camera resolution: " << mVideoProcessor.PrintResolution() << "; "
             << "Show GUI: " << mShowGui << "; "
             << "History: " << mVideoProcessor.GetHistory() << endl
             << "Available commands:" << endl
             << "s - Start processing" << endl
             << "c - Stop processing" << endl
             << "i - Show/hide GUI" << endl
             << "r - Change resolution" << endl
             << "q - Quit" << endl
             << ">>";
        char key;
        cin >> key;

        if(mIsRunning)
        {
            switch (key)
            {
            case 's':
            case 'i':
            case 'r':
            case 'h':
                cout << "Processing is running, try to stop first" << endl;
                break;
            case 'c':
                mpProcessing->interrupt();
                cout << "Waiting video processing thread.." << endl;
                mpProcessing->join();
                delete mpProcessing;
                mpProcessing = NULL;
                mIsRunning = false;
                break;
            case 'q':
                mpProcessing->interrupt();
                cout << "Waiting video processing thread.." << endl;
                mpProcessing->join();
                mIsRunning = false;
                stop = true;
                break;
            default:
                cout << "There is not such command!" << endl;
                break;
            }
        }
        else
        {
            int history = 0;

            switch (key)
            {
            case 's':
                if(mShowGui)
                    mpProcessing = new thread(&VideoProcessor::RunWithGui, &(mVideoProcessor));
                else
                    mpProcessing = new thread(&VideoProcessor::RunWithoutGui, &(mVideoProcessor));

                mIsRunning = true;
                break;
            case 'c':
                cout << "Processing is not started" << endl;
                break;
            case 'i':
                mShowGui = true;
                break;
            case 'r':
                handleResolution();
                break;
            case 'h':
                cin >> history;
                mVideoProcessor.CreateNewBackgroundSubtractor(history);
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
            mVideoProcessor.SetResolution(640, 480);
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
