// game.hpp

#ifndef GAME_HPP
#define GAME_HPP

#include "player.hpp"
#include "asteroid.hpp"
#include "explosion.hpp"

class CGame{
	public:
        CGame(const int screen_width, const int screen_height);
    
		void Menu();
        void Init();
		void Run();
		void GameOver();
		void Quit();

    
	private:
		void ProcessEvents	();
		void SpawnAsteroids	();
		void SpawnExplosion	(int x, int y, float speed);
		void SpawnScore		(int punkte, int x, int y);
        void RenderAsteroids(bool pause);
		void RenderSpieler	();
		void RenderExplosions	();
		void RenderNewHighscore	();
		void CheckCollisions	();
		void UpdateTexts	();
		void RenderTexts	();
		void FramesPerSecond	();
		void toggleFPS		();
		void togglePause	();
		void RenderPause	();
		bool NewHighscore	();
		void SetSchwierigkeitsgrad(int schwierigkeitsgrad);
		
		SDL_Surface* load_image(string filename);
    
		int schwierigkeitsgrad;
		
		// Variablen, die die Schwierigkeit beeinflussen
		int m_DmgRaumstation;
		int m_PlayerMaxSchuesse;
		int m_PlayerLeben;
		float m_RegenRaumstation;
		float m_SpawnTime;
		float m_SpeedAsteroid;
		
		// Zu ladende Bilder
		CPlayer *m_pPlayer;
		CSprite *m_pSpriteBackground;
		CSprite *m_pSpriteAsteroid;
		CSprite *m_pSpriteExplosion;
		
		float m_fAsteroidTimer;
		float m_fExplosionTimer;
		float m_fSpawnSchutz;
		float m_fSpawnSchutzTimer;
		float m_fStartRunTime;
		
		// Alles wichtige für den Framerate-Zähler
		int fps;
		int frames;
		bool showFPS;
		CText *m_pFramesPerSecond;
		
		
		list<CAsteroid> m_AsteroidList;
		list<CExplosion> m_ExplosionList;
		list<CText> m_TextList;
		
		// Texte für das Start-Menü
		CText *m_pWaehle;
		CText *m_pBruchpilot;
		CText *m_pDurchschnittstyp;
		CText *m_pEliteFlieger;
		
		// Feste Texte während des Spiels
		CText *m_pLebenText;
		CText *m_pScore_Text;
		CText *m_pLebensenergieText;
		
		
		// Dynamische Texte (Zahlen) während des Spiels
		CText *m_pScore;
		CText *m_pLebensenergie;
		CText *m_pLeben;
		
		// Müssen diese Texte upgedatet werden?
		bool m_UpdateScore;
		bool m_UpdateLeben;
		bool m_UpdateLebensenergie;
		
		// Texte am Spielende
		CText *m_pGameOver;
		CText *m_pHighscoreText;
		CText *m_pHighscore;
		CText *m_pNewHighscoreText1;
		CText *m_pNewHighscoreText2;
		CText *m_pNewHighscore;
		CText *m_pOldHighscoreText;
		CText *m_pOldHighscore;
		
		// läuft das Spiel?
		bool m_bGameRun;
		
		bool m_bPause;
		float m_fPauseTimer;
		CText *m_pPauseText;
		
		int m_punkte;
		int m_AlterHighscore;
		bool m_bNewHighscore;

        const int screen_width;
        const int screen_height;
		
        const int num_distinct_spawnpoints;

		CText *m_pVersion;
};
#endif
