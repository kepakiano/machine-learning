#ifndef SHOT_HPP
#define SHOT_HPP

#include "sprite.hpp"

class CShot{
	public:
		void Init	(CSprite *pSpriteShot, float fXPos, float fYPos);
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
		CSprite *m_pSpriteShot;
		float m_fXPos;
		float m_fYPos;
		bool m_bIsAlive;
		SDL_Rect m_Rect;
};
#endif