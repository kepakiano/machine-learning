// explosion.hpp
#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP
#include "sprite.hpp"

class CExplosion{
	public:
		void Init	(CSprite *pSpriteExplosion, float fXPos, float fYPos, float speed);
		void Update	();
		void Render	();
    
		bool IsAlive() {
			return m_bIsAlive;
		}
    
		void SetAlive(bool bIsAlive){
			m_bIsAlive = bIsAlive;
		}
		
		SDL_Rect GetRect(){
			return m_Rect;
		}
    
	private:
		CSprite *m_pSpriteExplosion;
		float m_fXPos;
		float m_fYPos;
		float m_fAnimPhase;
		float m_speed;
		bool m_bIsAlive;
		SDL_Rect m_Rect;
};
#endif