#pragma once

#include <chrono>

namespace MR {
class Timer {
    public:
        Timer();
        ~Timer();

        void start();
        float getDelta();
    
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_Clock;
};
}
