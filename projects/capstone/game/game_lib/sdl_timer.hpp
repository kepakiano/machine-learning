#ifndef SDL_TIMER_HPP
#define SDL_TIMER_HPP

#include <chrono>
#include <thread>
#include <iostream>

#include "timer.hpp"

class SdlTimer : public CTimer {
public:
    SdlTimer()
        : start(std::chrono::high_resolution_clock::now())
        , m_fCurTime(std::chrono::high_resolution_clock::now())
        , m_fLastTime(std::chrono::high_resolution_clock::now())
        , target_fps(60)
        , target_frame_duration(int(1000000./60)){

        m_fElapsed = m_fCurTime - m_fLastTime;
    }

    void Update(){
        // vergangene Zeit seit dem letzten Frame
        m_fCurTime = std::chrono::high_resolution_clock::now();
        m_fElapsed = m_fCurTime - m_fLastTime;
        std::cout << m_fElapsed.count() << std::endl;
        m_fLastTime = m_fCurTime;
    }

    virtual double GetElapsed() override{
      return SECONDS_PER_FRAME;
//        return m_fElapsed.count();
    }

    virtual std::chrono::high_resolution_clock::time_point GetCurTime() override{
        return m_fCurTime;
    }

    virtual std::chrono::high_resolution_clock::time_point GetStartTime() override {
        return start;
    }

    virtual void startFrame() override {
        start_of_frame = std::chrono::high_resolution_clock::now();
    }

    virtual void sleepForRestOfFrame(const std::chrono::high_resolution_clock::time_point &start_of_frame) override{
        auto end_of_frame = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::micro> duration_of_frame = end_of_frame - start_of_frame;
        std::this_thread::sleep_for(target_frame_duration-duration_of_frame);
    }

private:
    std::chrono::high_resolution_clock::time_point start;
    std::chrono::duration<double, std::ratio<1>> m_fElapsed;
    std::chrono::high_resolution_clock::time_point m_fCurTime;
    std::chrono::high_resolution_clock::time_point m_fLastTime;

    std::chrono::high_resolution_clock::time_point start_of_frame;

    const int target_fps;
    const std::chrono::microseconds target_frame_duration;

};

#endif // SDL_TIMER_HPP
