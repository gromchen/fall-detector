#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <sstream>

namespace FallDetector
{
template<typename T>
std::string ToString(T object)
{
    std::stringstream ss;
    ss << object;
    return ss.str();
}

enum HumanStateType
{
    STANDING,
    WALKING,
    FALLING,
    LYING
};
}

#endif // HELPERS_H
