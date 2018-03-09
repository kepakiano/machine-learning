// shot.cpp
#include "shot.hpp"

void CShot::Init(CSprite *pSpriteShot, float fXPos, float fYPos){
	m_pSpriteShot = pSpriteShot;
	m_fXPos = fXPos;
	m_fYPos = fYPos;
  
	m_Rect.x = static_cast<int>(fXPos);
	m_Rect.y = static_cast<int>(fYPos);
	m_Rect.w = pSpriteShot->GetRect().w;
	m_Rect.h = pSpriteShot->GetRect().h;
  
	m_bIsAlive = true;
}

void CShot::Update(){
	m_fYPos -= 400.0f * g_pTimer->GetElapsed();
  
	m_Rect.y = static_cast<int>(m_fYPos);
  
	if(m_fYPos < -15.0f){
	m_bIsAlive = false;
	}
} // Update

void CShot::Render(){
	if(m_bIsAlive == true){
		m_pSpriteShot->SetPos(m_fXPos, m_fYPos);
		m_pSpriteShot->Render();
	}
} // Render