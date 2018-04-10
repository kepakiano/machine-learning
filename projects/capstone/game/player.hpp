// player.hpp
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <list>

#include "actionchoice.h"
#include "asteroid.hpp"
#include "sprite.hpp"
#include "shot.hpp"


class CPlayer{
	public:
		CPlayer	();
		void Init	();
        void Quit	();
        void UpdateShots(bool pause, const double seconds);
        void Update	(const double seconds, const ActionChoice &action);
		void Reset	();
        list<CShot>& GetShotList(){
            return m_ShotList;
		}

        int GetXPosition() const {return m_fXPos;}
        int GetYPosition() const {return m_fYPos;}
        float getAnimPhase() const {return m_fAnimPhase;}
		int GetLeben();
		int GetLebensenergie_Raumstation();
		int GetScore();
		
        void SetWerte(int leben, int dmg_raumstation, float regen_raumstation);
        void Raumstation_Getroffen();
		int BerechnePunkte(int Asteroiden_Hoehe);

        bool shouldBeRendered(){
            return m_fSpawnSchutz > 3.0f || m_fSpawnSchutzTimer > 0.25f;
        }

        float GetSpawnSchutz() const {return m_fSpawnSchutz;}

        virtual ActionChoice getAction(const std::list<CAsteroid> & asteroid_list) = 0;
    protected:
        bool isShootingPossible(){
            return m_fShotCooldownTimer >= m_fShotCooldown;
        }
//        unsigned int m_MaxShots;
        bool m_bShotLock;

        float m_fXPos;
    protected:

        void ProcessAction(const ActionChoice &action, const double seconds);
		void CheckPosition();

		float m_fYPos;
		float m_fAnimPhase;
		float m_LebensenergieTimer;
		
		 // Variablen für den Schwierigkeitsgrad
		int m_DmgRaumstation;
        int m_Score;
		float m_RegenRaumstation;
		
        float m_fSpawnSchutz;
        float m_fSpawnSchutzTimer;

        const float m_fShotCooldown;
        float m_fShotCooldownTimer;

		unsigned int m_Leben;
		unsigned int m_Lebensenergie_Raumstation;

		list<CShot> m_ShotList;
};

#endif
