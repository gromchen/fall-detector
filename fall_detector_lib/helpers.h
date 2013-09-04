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

typedef enum
{
    NOT_DETECTED,
    STANDING,
    WALKING,
    FALLING,
    LYING,
    INJURED
} HumanState;

std::string HumanStateToString(HumanState humanState);
}

#endif // FALL_DETECTOR_HELPERS_H
