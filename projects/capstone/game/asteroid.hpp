// asteroid.hpp
#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "sprite.hpp"
#include "rectangle.hpp"

class CAsteroid{
	public:
        CAsteroid(float fXPos, float fYPos, float speed);
        void Update	(const double seconds);
    
		bool IsAlive(){
			return m_bIsAlive;
		}
    
		void SetAlive (bool bIsAlive){
			m_bIsAlive = bIsAlive;
		}

        Rectangle getRect() const {return rectangle;}
    
        float GetXPos() const {return m_fXPos;}
        float GetYPos() const {return m_fYPos;}
        float GetAnimPhase() const {return m_fAnimPhase;}
    
    private:
		CSprite *m_pSpriteAsteroid;
        Rectangle rectangle;
		float m_fXPos;
		float m_fYPos;
		float m_fAnimPhase;
        bool m_bIsAlive;
		float m_speed;
};

#endif
    
