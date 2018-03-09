// asteroid.cpp

#include "asteroid.hpp"

void CAsteroid::Init(CSprite *pSpriteAsteroid, float fXPos, float fYPos, float speed){
	m_pSpriteAsteroid = pSpriteAsteroid;
	m_fXPos = fXPos;
	m_fYPos = fYPos;
  
	m_fAnimPhase = 0.0f;
  
	m_Rect.x = static_cast<int>(fXPos);
	m_Rect.y = static_cast<int>(fYPos);
	m_Rect.w = pSpriteAsteroid->GetRect().w;
	m_Rect.h = pSpriteAsteroid->GetRect().h;
	m_speed = speed;
  
	m_bIsAlive = true;
} // Init

void CAsteroid::Update(bool pause){
	if(pause == false){
		m_fYPos += m_speed *g_pTimer->GetElapsed();
		m_Rect.y = static_cast<int>(m_fYPos);
	}
	m_fAnimPhase += 10.0f *g_pTimer->GetElapsed();
  
	if(m_fAnimPhase >= 20.0f)
		m_fAnimPhase = m_fAnimPhase - 20.0f;
  
//	if(m_fYPos > 590.0f)
//      m_bIsAlive = false;
} // Update

void CAsteroid::Render(){
	m_pSpriteAsteroid->SetPos(m_fXPos, m_fYPos);
	m_pSpriteAsteroid->Render(m_fAnimPhase);
} // Render