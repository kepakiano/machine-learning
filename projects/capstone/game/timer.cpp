// timer.cpp
#include "timer.hpp"
CTimer::CTimer()
    : start(std::chrono::high_resolution_clock::now())
    , m_fCurTime(std::chrono::high_resolution_clock::now())
    , m_fLastTime(std::chrono::high_resolution_clock::now()){

    m_fElapsed = m_fCurTime - m_fLastTime;
}

void CTimer::Update(){
  // vergangene Zeit seit dem letzten Frame
    m_fCurTime = std::chrono::high_resolution_clock::now();
    m_fElapsed = m_fCurTime - m_fLastTime;
	m_fLastTime = m_fCurTime;
}
