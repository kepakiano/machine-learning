// player.hpp
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <list>
#include "sprite.hpp"
#include "shot.hpp"

class CPlayer{
	public:
		CPlayer	();
		void Init	();
		void Quit	();
		void Render	(bool pause);
		void RenderShots(bool pause);
		void Update	();
		void Reset	();
		list<CShot> *GetShotList(){
			return &m_ShotList;
		}
	
		int GetXPosition();
		int GetLeben();
		int GetLebensenergie_Raumstation();
		int GetScore();
		
		void SetWerte(int leben, int max_schuesse, int dmg_raumstation, float regen_raumstation);
		void Raumstation_Getroffen();
		void DecreaseShots();
		int BerechnePunkte(int Asteroiden_Hoehe);
		
	private:
		void ProcessMoving();
		void ProcessShooting();
		void CheckPosition();
    
		CSprite *m_pSpritePlayer;
		CSprite *m_pSpriteShot;
		float m_fXPos;
		float m_fYPos;
		float m_fAnimPhase;
		float m_LebensenergieTimer;
		
		 // Variablen für den Schwierigkeitsgrad
		int m_DmgRaumstation;
		int m_Score;
		unsigned int m_MaxShots;
		float m_RegenRaumstation;
		
		
		unsigned int m_CurShots;
		unsigned int m_Leben;
		unsigned int m_Lebensenergie_Raumstation;
		
		bool m_bShotLock;
		list<CShot> m_ShotList;
};

#endif