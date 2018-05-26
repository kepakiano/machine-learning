#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

class CTimer {

protected:
  const double SECONDS_PER_FRAME = 0.0166666;
public:
    virtual void Update() = 0;

    virtual double GetElapsed() = 0;
    virtual std::chrono::high_resolution_clock::time_point GetCurTime() = 0;
    virtual std::chrono::high_resolution_clock::time_point GetStartTime() = 0;
    virtual void startFrame() = 0;
    virtual void sleepForRestOfFrame(const std::chrono::high_resolution_clock::time_point &start_of_frame) = 0;
};

#endif
