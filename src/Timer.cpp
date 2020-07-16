#include "Mantaray/Core/Timer.hpp"

using namespace MR;

Timer::Timer() {

}

Timer::~Timer() {

}

void Timer::start() {
    m_Clock = std::chrono::high_resolution_clock::now();
}

float Timer::getDelta() {
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> delta = now - m_Clock;
    m_Clock = now;
    return delta.count();
}

float Timer::getElapsedTime() {
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> delta = now - m_Clock;
    return delta.count();
}
