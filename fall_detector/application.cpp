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
        cout << "Running:\t\t" << mIsRunning << endl
             << "Camera resolution:\t" << mVideoProcessor.PrintResolution() << endl
             << "Show GUI:\t\t" << mShowGui << endl
             << "History:\t\t" << mVideoProcessor.GetHistory() << endl
             << "Threshold:\t\t" << mVideoProcessor.GetThreshold() << endl
             << "===================" << endl
             << "Available commands:" << endl;

        if(mIsRunning)
        {
            cout << "c - Stop processing" << endl
                 << "q - Quit" << endl
                 << ">>";
            char key;
            cin >> key;

            switch (key)
            {
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
            cout << "s - Start processing" << endl
                 << "i - Show/hide GUI" << endl
                 << "r - Change resolution" << endl
                 << "h - Create new backgound subtractor" << endl
                 << "q - Quit" << endl
                 << ">>";

            int history = 0;
            float threshold = 0;
            char key;
            cin >> key;

            switch (key)
            {
            case 's':
                if(mShowGui)
                    mpProcessing = new thread(&VideoProcessor::RunWithGui, &(mVideoProcessor));
                else
                    mpProcessing = new thread(&VideoProcessor::RunWithoutGui, &(mVideoProcessor));

                mIsRunning = true;
                break;
            case 'i':
                mShowGui = !mShowGui;
                break;
            case 'r':
                handleResolution();
                break;
            case 'h':
                cout << "Input history: ";
                cin >> history;
                cout << "Input threshold";
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
