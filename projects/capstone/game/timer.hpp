#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

#include "SDL/SDL.h"
#include "singleton.hpp"

#define g_pTimer CTimer::Get ()
class CTimer : public TSingleton<CTimer>{
	public:
		CTimer();
	
		void Update();
    
        auto GetElapsed(){
            return m_fElapsed.count();
		}
		
        auto GetCurTime(){
            return m_fCurTime;
        }

        auto GetStartTime(){
            return start;
        }
      
	private:
        std::chrono::high_resolution_clock::time_point start;
        std::chrono::duration<double, std::ratio<1>> m_fElapsed;
        std::chrono::high_resolution_clock::time_point m_fCurTime;
        std::chrono::high_resolution_clock::time_point m_fLastTime;
};
#endif
