// asteroid.hpp
#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "sprite.hpp"

class CAsteroid{
	public:
		void Init	(CSprite *pSpriteAsteroid, float fXPos, float fYPos, float speed);
		void Update	(bool pause);
		void Render	();
    
		bool IsAlive(){
			return m_bIsAlive;
		}
    
		void SetAlive (bool bIsAlive){
			m_bIsAlive = bIsAlive;
		}
    
		SDL_Rect GetRect(){
			return m_Rect;
		}
    
	private:
		CSprite *m_pSpriteAsteroid;
		float m_fXPos;
		float m_fYPos;
		float m_fAnimPhase;
		bool m_bIsAlive;
		SDL_Rect m_Rect;
		float m_speed;
};

#endif
    