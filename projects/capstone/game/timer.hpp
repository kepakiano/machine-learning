#ifndef TIMER_HPP
#define TIMER_HPP

// #include <SDL.h>
#include "SDL/SDL.h"
#include "singleton.hpp"

#define g_pTimer CTimer::Get ()
class CTimer : public TSingleton<CTimer>{
	public:
		CTimer();
	
		void Update();
    
		float GetElapsed(){
			return m_fElapsed;
		}
		
		int GetCurTime(){
			return (int)m_fCurTime;
		}
      
	private:
		float m_fElapsed;
		float m_fCurTime;
		float m_fLastTime;
};
#endif