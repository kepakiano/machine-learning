// asteroid.cpp

#include "asteroid.hpp"

CAsteroid::CAsteroid(float fXPos, float fYPos, float speed)
    : rectangle(fXPos, fYPos, 64, 64){
	m_fXPos = fXPos;
	m_fYPos = fYPos;
  
	m_fAnimPhase = 0.0f;
  
	m_speed = speed;
  
	m_bIsAlive = true;
} // Init

void CAsteroid::Update(bool pause){
	if(pause == false){
        m_fYPos += m_speed *g_pTimer->GetElapsed();
        rectangle.y = m_fYPos;
	}
	m_fAnimPhase += 10.0f *g_pTimer->GetElapsed();
  
	if(m_fAnimPhase >= 20.0f)
		m_fAnimPhase = m_fAnimPhase - 20.0f;

} // Update
