#include "application.h"

using namespace std;

int main()
{
    try
    {
        Application application;
        application.Run();
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return 1;
    }

    return 0;
}
