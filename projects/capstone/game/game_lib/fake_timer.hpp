#ifndef FAKE_TIMER_HPP
#define FAKE_TIMER_HPP
#include "timer.hpp"

class FakeTimer : public CTimer {
public:
    FakeTimer()
        : start(std::chrono::high_resolution_clock::now())
    {}

    virtual void Update() override {}

    // Always return 1/60. seconds, which will result in a fake framerate of 60FPS
    virtual double GetElapsed() override {return SECONDS_PER_FRAME;}

    virtual std::chrono::high_resolution_clock::time_point GetCurTime() override {
        return std::chrono::high_resolution_clock::now();
    }
    virtual std::chrono::high_resolution_clock::time_point GetStartTime() override {
        return start;
    }
    virtual void startFrame() override{}
    virtual void sleepForRestOfFrame(const std::chrono::high_resolution_clock::time_point &start_of_frame) override {}

private:

    std::chrono::high_resolution_clock::time_point start;
};

#endif // FAKE_TIMER_HPP
