//  explosion.cpp
#include "explosion.hpp"

void CExplosion::Init(CSprite *pSpriteExplosion, float fXPos, float fYPos, float speed){
	m_pSpriteExplosion = pSpriteExplosion;
	m_fXPos = fXPos;
	m_fYPos = fYPos;
  
	m_Rect.x = static_cast<int>(fXPos);
	m_Rect.y = static_cast<int>(fYPos);
	m_Rect.w = pSpriteExplosion->GetRect().w;
	m_Rect.h = pSpriteExplosion->GetRect().h;
	m_speed = speed;
	m_fAnimPhase = 0.0f;
  
	m_bIsAlive = true;
}

void CExplosion::Update(){
	m_fYPos += m_speed * g_pTimer->GetElapsed();
  
	m_Rect.y = static_cast<int>(m_fYPos);
	m_fAnimPhase += 60.0f *g_pTimer->GetElapsed();
  
	if(m_fYPos < -15.0f || m_fAnimPhase > 25.0f){
 		m_bIsAlive = false;
	}
  
} // Update

void CExplosion::Render(){
	m_pSpriteExplosion->SetPos(m_fXPos, m_fYPos);
	m_pSpriteExplosion->Render(m_fAnimPhase);

} // Render