#ifndef FALL_DETECTOR_HELPERS_H
#define FALL_DETECTOR_HELPERS_H

#include <string>
#include <sstream>

namespace FallDetector
{
template<typename T>
std::string ToString(T object)
{
    std::stringstream string_stream;
    string_stream << object;
    return string_stream.str();
}

enum HumanState
{
    STANDING,
    WALKING,
    FALLING,
    LYING
};
}

#endif // FALL_DETECTOR_HELPERS_H
