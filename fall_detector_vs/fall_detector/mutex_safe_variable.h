#ifndef FALL_DETECTOR_MUTEX_SAFE_VARIABLE_H
#define FALL_DETECTOR_MUTEX_SAFE_VARIABLE_H

#include <mutex>

namespace FallDetector
{
    template<typename T>
    class MutexSafeVariable
    {
    public:
        MutexSafeVariable(T variable);
        ~MutexSafeVariable();

        T& rGetVariable();
        void SetVariable(T);

    private:
        T mVariable;
        std::mutex mMutex; 

    };
}

#endif // !FALL_DETECTOR_MUTEX_SAFE_VARIABLE_H
