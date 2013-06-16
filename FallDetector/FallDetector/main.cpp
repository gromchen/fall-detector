#include <ctime>
#include <iostream>

#include "Fruit.h"

using namespace std;
 
int main()
{
    Fruit orange(2);

    cout << orange.GetQuantity() << endl;

    orange.Buy(4);

    cout << orange.GetQuantity() << endl;

    orange.Sell(5);

    cout << orange.GetQuantity() << endl;

    return 0;
}