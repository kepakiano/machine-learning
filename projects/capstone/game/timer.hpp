#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include <thread>

#include "singleton.hpp"

#define g_pTimer CTimer::Get ()
class CTimer : public TSingleton<CTimer>{
	public:
		CTimer();
	
		void Update();

        auto GetElapsed(){
            return m_fElapsed.count();
        }

        std::chrono::duration<double, std::ratio<1>> GetElapsedSeconds(){
            return m_fElapsed;
        }
		
        auto GetCurTime(){
            return m_fCurTime;
        }

        auto GetStartTime(){
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
#endif
