// player.hpp
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <list>
#include "sprite.hpp"
#include "shot.hpp"

enum Action{
    MOVE_RIGHT,
    MOVE_LEFT,
    SHOOT,
    PASS
};

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

    protected:
        virtual Action getAction() = 0;
        unsigned int m_MaxShots;
        bool m_bShotLock;
        unsigned int m_CurShots;
		
    private:

        void ProcessAction(const Action &action);
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
		float m_RegenRaumstation;
		

		unsigned int m_Leben;
		unsigned int m_Lebensenergie_Raumstation;

		list<CShot> m_ShotList;
};

#endif
