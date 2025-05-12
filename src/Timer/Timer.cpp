#include "Timer.hpp"

Timer::Timer() {
    running = false;
}

void Timer::reset() {
    running = false;
}

int Timer::start() {
    running = true;
    startTime = std::chrono::high_resolution_clock::now();
    return 0;
}

int Timer::stop() {
    if (running) {
        endTime = std::chrono::high_resolution_clock::now();
        running = false;
    }
    return 0;
}

int Timer::result() {
    std::chrono::milliseconds milliseconds;
    if (running) {
        auto now = std::chrono::high_resolution_clock::now();
        milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
    } else {
        milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    }
    return static_cast<int>(milliseconds.count());
}