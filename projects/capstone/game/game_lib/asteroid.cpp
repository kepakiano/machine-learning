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

void CAsteroid::Update(const double seconds){
    m_fYPos += m_speed * seconds;
    rectangle.y = m_fYPos;

    m_fAnimPhase += 10.0f * seconds;
  
	if(m_fAnimPhase >= 20.0f)
		m_fAnimPhase = m_fAnimPhase - 20.0f;

} // Update
