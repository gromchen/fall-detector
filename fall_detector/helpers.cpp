#include "helpers.h"

#include <sstream>

using namespace std;

namespace FallDetector
{
string IntToString(int number)
{
    stringstream ss;
    ss << number;
    return ss.str();
}
}
