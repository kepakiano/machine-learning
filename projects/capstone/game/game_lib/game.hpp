// game.hpp

#ifndef GAME_HPP
#define GAME_HPP

#include "player.hpp"
#include "asteroid.hpp"
#include "explosion.hpp"
#include "renderer.h"
#include "timer.hpp"

class CGame{
	public:
        CGame(const int screen_width, const int screen_height);

        void Init(const bool use_sdl,
                  const bool use_reinforcment_learning,
                  const bool bot_is_learning,
                  const double alpha,
                  const double gamma,
                  const double epsilon);
        void Run();
        void GameOver();
        void Quit();

	private:
		void ProcessEvents	();
		void SpawnAsteroids	();
		void SpawnExplosion	(int x, int y, float speed);
		void SpawnScore		(int punkte, int x, int y);
        void UpdateAsteroids();
        void UpdateExplosions	();
        double CheckCollisions();
		void FramesPerSecond	();
		void toggleFPS		();
        void togglePause	();
		bool NewHighscore	();
        void SetDifficultyLevel(int schwierigkeitsgrad);
		
		SDL_Surface* load_image(string filename);

        Renderer *renderer;
    
		int schwierigkeitsgrad;
		
		// Variablen, die die Schwierigkeit beeinflussen
        int m_DmgRaumstation;
		int m_PlayerLeben;
		float m_RegenRaumstation;
		float m_SpawnTime;
		float m_SpeedAsteroid;

		CPlayer *m_pPlayer;
        CTimer *m_pTimer;

		float m_fAsteroidTimer;
		float m_fExplosionTimer;

		float m_fStartRunTime;
		
		// Alles wichtige für den Framerate-Zähler
		int fps;
        int frames;
		
		list<CAsteroid> m_AsteroidList;
        list<CExplosion> m_ExplosionList;

		// läuft das Spiel?
		bool m_bGameRun;
		
        bool m_bPause;

		int m_AlterHighscore;
		bool m_bNewHighscore;

        const int screen_width;
        const int screen_height;
		
        const int num_distinct_spawnpoints;

        double rewardNoEvent() {return 1.0;}
        double rewardShipHit() {return -100.0;}


};
#endif