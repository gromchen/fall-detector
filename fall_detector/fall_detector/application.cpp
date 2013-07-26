#include "application.h"

#include <thread>

using namespace std;

namespace FallDetector
{
    Application::Application()
    {
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        thread processing(&VideoProcessor::Run, &m_videoProcessor);

        handleMainCommands();
        cout << "Waiting video processing thread.." << endl;
        processing.join();
    }

    void Application::handleMainCommands()
    {
        bool stop = false;

        while (!stop)
        {
            cout << "Available commands" << endl
                << "i - Show/hide GUI" << endl
                << "r - Change resolution" << endl
                << "q - Quit" << endl
                << ">>";

            char key;

            cin >> key;
            
            switch (key)
            {
            case 'q':
                m_videoProcessor.Stop();
                stop = true;
                break;
            case 'i':
                m_videoProcessor.ShowHideGui();
                break;
            case 'r':
                handleResolution();
                break;
            default:
                cout << "There is not such command!" << endl;
                break;
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
                m_videoProcessor.SetResolution(320, 240);
                break;
            case '2':
                m_videoProcessor.SetResolution(640, 480);
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