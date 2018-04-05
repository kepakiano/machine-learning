// shot.cpp
#include "shot.hpp"

CShot::CShot(float fXPos, float fYPos)
    : m_Rect(fXPos, fYPos, 64, 64){
	m_fXPos = fXPos;
	m_fYPos = fYPos;

	m_bIsAlive = true;
}

void CShot::Update(const double seconds){
    m_fYPos -= 400.0f * seconds;
  
	m_Rect.y = static_cast<int>(m_fYPos);
  
	if(m_fYPos < -15.0f){
        m_bIsAlive = false;
	}
} // Update

