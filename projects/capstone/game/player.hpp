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
        void UpdateShots(bool pause, const double seconds);
        void Update	(const double seconds, const Action &action);
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
		
		void SetWerte(int leben, int max_schuesse, int dmg_raumstation, float regen_raumstation);
        void Raumstation_Getroffen();
		int BerechnePunkte(int Asteroiden_Hoehe);

        bool shouldBeRendered(){
            return m_fSpawnSchutz > 3.0f || m_fSpawnSchutzTimer > 0.25f;
        }

        float GetSpawnSchutz() const {return m_fSpawnSchutz;}

        virtual Action getAction() = 0;
    protected:
        bool isShootingPossible(){
            return m_ShotList.size() < m_MaxShots;
        }
        unsigned int m_MaxShots;
        bool m_bShotLock;
		
    private:

        void ProcessAction(const Action &action, const double seconds);
		void CheckPosition();

		float m_fXPos;
		float m_fYPos;
		float m_fAnimPhase;
		float m_LebensenergieTimer;
		
		 // Variablen für den Schwierigkeitsgrad
		int m_DmgRaumstation;
        int m_Score;
		float m_RegenRaumstation;
		
        float m_fSpawnSchutz;
        float m_fSpawnSchutzTimer;

		unsigned int m_Leben;
		unsigned int m_Lebensenergie_Raumstation;

		list<CShot> m_ShotList;
};

#endif
