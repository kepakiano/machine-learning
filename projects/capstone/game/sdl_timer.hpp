#ifndef SDL_TIMER_HPP
#define SDL_TIMER_HPP

#include <chrono>
#include <thread>

#include "timer.hpp"

class SdlTimer : public CTimer {
public:
    SdlTimer()
        : start(std::chrono::high_resolution_clock::now())
        , m_fCurTime(std::chrono::high_resolution_clock::now())
        , m_fLastTime(std::chrono::high_resolution_clock::now())
        , target_fps(60)
        , target_frame_duration(int(1000./60)){

        m_fElapsed = m_fCurTime - m_fLastTime;
    }

    void Update(){
        // vergangene Zeit seit dem letzten Frame
        m_fCurTime = std::chrono::high_resolution_clock::now();
        m_fElapsed = m_fCurTime - m_fLastTime;
        m_fLastTime = m_fCurTime;
    }



    double GetElapsed(){
        return m_fElapsed.count();
    }

    std::chrono::duration<double, std::ratio<1>> GetElapsedSeconds(){
        return m_fElapsed;
    }

    std::chrono::high_resolution_clock::time_point GetCurTime(){
        return m_fCurTime;
    }

    std::chrono::high_resolution_clock::time_point GetStartTime(){
        return start;
    }

    void startFrame(){
        start_of_frame = std::chrono::high_resolution_clock::now();
    }

    void sleepForRestOfFrame(const std::chrono::high_resolution_clock::time_point &start_of_frame){
        auto end_of_frame = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration_of_frame = end_of_frame - start_of_frame;
        std::this_thread::sleep_for(target_frame_duration-duration_of_frame);
    }

private:
    std::chrono::high_resolution_clock::time_point start;
    std::chrono::duration<double, std::ratio<1>> m_fElapsed;
    std::chrono::high_resolution_clock::time_point m_fCurTime;
    std::chrono::high_resolution_clock::time_point m_fLastTime;

    std::chrono::high_resolution_clock::time_point start_of_frame;

    const int target_fps;
    const std::chrono::milliseconds target_frame_duration;

};

#endif // SDL_TIMER_HPP
