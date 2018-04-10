// game.cpp
#include "game.hpp"
#include "SDL/SDL_ttf.h"
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include <cmath>
#include <thread>

#include "gameevents.hpp"
#include "humanplayer.h"
#include "randomplayer.h"
#include "reinforcementlearningplayer.h"
#include "sdlrenderer.h"
#include "sdl_timer.hpp"
#include "fake_timer.hpp"
#include "fake_renderer.hpp"
#include "utilities.hpp"

using namespace std;

CGame::CGame(const int screen_width, const int screen_height)
    : screen_width(screen_width), screen_height(screen_height), num_distinct_spawnpoints(8){
    m_pPlayer = NULL;
    renderer = new SdlRenderer(screen_width, screen_height);
    m_pTimer = new SdlTimer();

//    renderer = new FakeRenderer();
//    m_pTimer = new FakeTimer();
}

void CGame::Init(){
    renderer->Init();
	
    m_pPlayer = new ReinforcementLearningPlayer(false);
	m_pPlayer->Init();
	m_pPlayer->Reset();

    m_fAsteroidTimer = 0.0f;
	
	fps = 0;
	frames = 0;
	schwierigkeitsgrad = 0;

	m_bGameRun = true;
	
    m_bPause = false;

    SetDifficultyLevel(renderer->GetDifficultyLevel());

    m_pPlayer->SetWerte(m_PlayerLeben, m_DmgRaumstation, m_RegenRaumstation);
} // Init

void CGame::Quit(){
	if(m_pPlayer != NULL){
		m_pPlayer->Quit();
		delete(m_pPlayer);
		m_pPlayer = NULL;
	}
  
    renderer->Quit();
} // Quit

void CGame::Run(){
	while(m_bGameRun == true && m_pPlayer->GetLeben() > 0 && m_pPlayer->GetLebensenergie_Raumstation() > 0){
        m_pTimer->Update();
        auto start_of_frame = std::chrono::high_resolution_clock::now();

        GameEvent game_event = renderer->ProcessEvents();

        if(game_event == END_GAME)
            m_bGameRun = false;
        if(game_event == TOGGLE_PAUSE)
            togglePause();

  		FramesPerSecond();		
		
        UpdateExplosions();
        m_pPlayer->UpdateShots(m_bPause, m_pTimer->GetElapsed());
        UpdateAsteroids(m_bPause);

		if(m_bPause == false){
            ActionChoice action = m_pPlayer->getAction(m_AsteroidList);
            m_pPlayer->Update(m_pTimer->GetElapsed(), action);
 			SpawnAsteroids();
            CheckCollisions();
        }

        renderer->RenderFrame(m_AsteroidList,
                              m_ExplosionList,
                              m_pPlayer,
                              m_bPause,
                              m_pTimer->GetElapsed());

        m_pTimer->sleepForRestOfFrame(start_of_frame);
    }
}

void CGame::GameOver()
{
    //Wenn der Spieler verloren hat, also nicht per Escape beendet wurde
    if(m_pPlayer->GetLeben() == 0 || m_pPlayer->GetLebensenergie_Raumstation() < 1){
        renderer->GameOver(NewHighscore(), m_pPlayer->GetLeben(), m_pPlayer->GetScore(), m_AlterHighscore);
    }

} // Run

void CGame::SpawnAsteroids(){
    m_fAsteroidTimer += m_pTimer->GetElapsed();
  
    if(m_fAsteroidTimer >= m_SpawnTime){

        const int asteroid_width = 64;
        const int space_between_spawnpoints = screen_width / num_distinct_spawnpoints;
        const int leftmost_position = space_between_spawnpoints - asteroid_width / 2;
        // The width of one asteroid is 64 pixels. We want their mids to spawn at x-position 50, 100... etc
        int XPos = leftmost_position + getRandomNumberBetween(0, num_distinct_spawnpoints) * space_between_spawnpoints;
        CAsteroid asteroid(static_cast<float>(XPos), -60.0f, m_SpeedAsteroid);
    
        m_AsteroidList.push_back(asteroid);
		m_fAsteroidTimer = 0.0f;
  }	
} // SpawnAsteroids

void CGame::CheckCollisions(){
    list<CShot> &ShotList = m_pPlayer->GetShotList();
	list<CAsteroid>::iterator ItAsteroid = m_AsteroidList.begin();
	list<CShot>::iterator ItShot;
  
	while(ItAsteroid != m_AsteroidList.end()){
        Rectangle RectAsteroid = ItAsteroid->getRect();

        // Does an asteroid hit the ship?
        if(m_pPlayer->GetSpawnSchutz() > 3.0f && RectAsteroid.y > 466 &&
            RectAsteroid.x + 64 - 10 > m_pPlayer->GetXPosition() &&
            RectAsteroid.x < (m_pPlayer->GetXPosition() + 54))
		{
			ItAsteroid->SetAlive(false);
			
            renderer->handleGameEvent(UPDATE_LIVES_TEXT);
			SpawnExplosion(RectAsteroid.x, RectAsteroid.y, m_SpeedAsteroid/2.0f);
			SpawnExplosion(m_pPlayer->GetXPosition(), 530.0, m_SpeedAsteroid/2.0f);
            m_pPlayer->Reset();
		}
        for(ItShot = ShotList.begin(); ItShot != ShotList.end(); ++ItShot){
            Rectangle RectShot = ItShot->GetRect();
			
			// Überschneiden sich die Rechtecke eines Asteroiden und eines Schusses?
			if	(RectShot.y < RectAsteroid.y + RectAsteroid.h &&
				RectShot.y + RectShot.h > RectAsteroid.y &&
				RectShot.x < RectAsteroid.x + 64 &&
				RectShot.x + RectShot.w > RectAsteroid.x){
				
				ItAsteroid->SetAlive(false);
                ItShot->SetAlive(false);
				m_punkte = m_pPlayer->BerechnePunkte(RectAsteroid.y);
                renderer->handleGameEvent(UPDATE_SCORE_TEXT);
				SpawnExplosion(RectAsteroid.x, RectAsteroid.y, m_SpeedAsteroid/2.0f);
                renderer->SpawnScore(m_punkte, RectAsteroid.x, RectAsteroid.y);
			}
		}

		if(RectAsteroid.y > 590){
			ItAsteroid->SetAlive(false);
			m_pPlayer->Raumstation_Getroffen();
            renderer->handleGameEvent(UPDATE_ENERGY_TEXT);
		}
		if(ItAsteroid->IsAlive())
			ItAsteroid++;
		else {
			ItAsteroid = m_AsteroidList.erase(ItAsteroid); 
		}
	}
} // CheckCollisions

void CGame::UpdateAsteroids(bool pause){
    for(CAsteroid &asteroid : m_AsteroidList)
        asteroid.Update(pause, m_pTimer->GetElapsed());
}

void CGame::UpdateExplosions(){
	list<CExplosion>::iterator It;
	It = m_ExplosionList.begin();
	while(It != m_ExplosionList.end()){
		if(It->IsAlive()){
			if(m_bPause == false)
                It->Update(m_pTimer->GetElapsed());
			It++;
		}
		else
			It = m_ExplosionList.erase(It);
	}
} // RenderExplosion



void CGame::FramesPerSecond(){
	frames++;
    fps = (frames*1000000000.f) / (m_pTimer->GetCurTime() - m_pTimer->GetStartTime()).count();

    renderer->setFramesPerSecond(fps);
} // FramesPerSecond

void CGame::togglePause(){
	m_bPause = m_bPause ? false : true;
} // toggleFPS

void CGame::SetDifficultyLevel(int schwierigkeitsgrad){
    this->schwierigkeitsgrad = schwierigkeitsgrad;
	if(schwierigkeitsgrad == 1){
		m_SpeedAsteroid = 150.0f;
        m_DmgRaumstation = 12;
		m_PlayerLeben = 7;
		m_RegenRaumstation = 0.2f;
		m_SpawnTime = 1.0f;
	}
	else if (schwierigkeitsgrad == 2){
		m_SpeedAsteroid = 175.0f;
        m_DmgRaumstation = 16;
		m_PlayerLeben = 6;
		m_RegenRaumstation = 0.35f;
		m_SpawnTime = 0.75f;
	}
	else{
		m_SpeedAsteroid = 200.0f;
        m_DmgRaumstation = 20;
		m_PlayerLeben = 5;
		m_RegenRaumstation = 0.5f;
		m_SpawnTime = 0.5f;
		
	}
} // SetSchwierigkeitsgrad

void CGame::SpawnExplosion(int x, int y, float speed){
	CExplosion Explosion;
    Explosion.Init(x, y, speed);
	m_ExplosionList.push_back(Explosion);
	m_fExplosionTimer = 0.0f;
}

bool CGame::NewHighscore(){
	ifstream Alt("Highscore.txt", ios::in);
	
	string AlterScoreLeicht = "nnnnnnnnnnnn";
	string AlterScoreMittel = "nnnnnnnnnnnn";
	string AlterScoreSchwer = "nnnnnnnnnnnn";
	
	string *AlteScores[3] = {&AlterScoreLeicht, &AlterScoreMittel, &AlterScoreSchwer};
	
	
	Alt >> AlterScoreLeicht >> AlterScoreMittel >> AlterScoreSchwer;
	Alt.close();
	
	m_AlterHighscore = 0;
	int stelle = 1;
	int str_position = schwierigkeitsgrad-1;
	
	for(int i = AlteScores[str_position]->length()-1; i >= 0; i--){
		if(AlteScores[str_position]->at(i) != 'n' && AlteScores[str_position]->at(i) != '\0'){
			m_AlterHighscore += ((int)((AlteScores[str_position]->at(i))-'0')) * stelle;
			stelle *= 10;
		}
	}
	
	if(m_pPlayer->GetScore() > m_AlterHighscore){
		
		ofstream Neu("Highscore.txt", ios::out);
		
		ostringstream temp;
		temp << m_pPlayer->GetScore();
		string hinne_jetz;
		hinne_jetz = temp.str();
		
		if(schwierigkeitsgrad == 1)
			Neu << hinne_jetz << '\n' << AlterScoreMittel << '\n' << AlterScoreSchwer;
		else if(schwierigkeitsgrad == 2)
			Neu << AlterScoreLeicht << '\n' << hinne_jetz << '\n' << AlterScoreSchwer;
		else
			Neu << AlterScoreLeicht << '\n' << AlterScoreMittel << '\n' << hinne_jetz ;
		
		Neu.close();
		
		return true;
	}
	else
		return false;
}

