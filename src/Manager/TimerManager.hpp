#ifndef TIMERMANAGER_HPP
#define TIMERMANAGER_HPP

#include "../Timer/Timer.hpp"

class TimerManager {
public:
    // The measure method accepts any callable (lambda, function pointer, etc.)
    // that performs the operation to be timed.
    // It returns the elapsed time (in milliseconds) as measured by the Timer.
    template<typename Func>
    static int measureTimeMs(Func executable_function) {
        Timer timer;
        timer.start();
        executable_function();
        timer.stop();
        return timer.result();
    }
};

#endif
