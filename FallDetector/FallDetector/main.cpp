#include <ctime>
#include <iostream>

using namespace std;
 
int main()
{
    srand((unsigned int) time(NULL));

    int num = (rand() % 10) + 1;
    
    cout << num << endl;

    return 0;
}