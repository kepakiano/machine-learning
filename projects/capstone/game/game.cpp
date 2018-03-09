// game.cpp
#include "game.hpp"
#include "SDL/SDL_ttf.h"
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include <cmath>

using namespace std;

CGame::CGame(){
	m_pPlayer = NULL;
	m_pSpriteBackground = NULL;
	m_pSpriteAsteroid = NULL;
}

void CGame::Menu(){
	
	SDL_Event Event;
	
	m_pSpriteBackground->Render();
	m_pWaehle->Render();
	m_pBruchpilot->Render();
	m_pDurchschnittstyp->Render();
	m_pEliteFlieger->Render();
	m_pVersion->Render();
	g_pFramework->Flip();
	
	while(schwierigkeitsgrad == 0){
		if(SDL_PollEvent(&Event)){
			if(SDL_KEYDOWN) {
				switch(Event.key.keysym.sym){
					case (SDLK_1): {
						schwierigkeitsgrad = 1;
						SetSchwierigkeitsgrad(1);
					} break;
					case (SDLK_2): {
						schwierigkeitsgrad = 2;
						SetSchwierigkeitsgrad(2);
					} break;
					case (SDLK_3): {
						schwierigkeitsgrad = 3;
						SetSchwierigkeitsgrad(3);
					} break;
				}
			}
		}
	}
	
	m_pPlayer->SetWerte(m_PlayerLeben, m_PlayerMaxSchuesse, m_DmgRaumstation, m_RegenRaumstation);
  
}
	
void CGame::Init(){
	TTF_Init();
	
	m_pWaehle = new CText("Wie gut fliegst du?", 210, 260, 15);
	m_pBruchpilot = new CText("1. Bruchpilot", 210, 290, 15);
	m_pDurchschnittstyp = new CText("2. Durchschnittstyp", 210, 320, 15);
	m_pEliteFlieger = new CText("3. Elite-Flieger", 210, 350, 15);
	m_pPlayer = new CPlayer;
	m_pPlayer->Init();
	m_pPlayer->Reset();

	m_pSpriteBackground = new CSprite;
	m_pSpriteBackground->Load("Background.png");
	
	m_pSpriteExplosion = new CSprite;
	m_pSpriteExplosion->Load("explosion3.png", 25, 64, 64);
 	m_pSpriteExplosion->SetColorKey(0,0,0);
	
	m_pSpriteAsteroid = new CSprite;
	m_pSpriteAsteroid->Load("Asteroid.png", 20, 64, 64);
	m_pSpriteAsteroid->SetColorKey(255,0,255);
  
	m_fAsteroidTimer = 0.0f;
	m_fSpawnSchutz = 3.0f;
	m_fSpawnSchutzTimer = 0.0f;
	
	fps = 0;
	frames = 0;
	schwierigkeitsgrad = 0;
  
	m_pLebensenergie = new CText(200, 15, 15);
	m_pLebensenergieText = new CText("Raumstation: ", 10, 15, 15);
  
	m_pLeben = new CText(120, 35, 15);
	m_pLebenText = new CText("Leben: ", 10, 35, 15);

	m_pScore_Text = new CText("Score:", 600, 15, 15);
	m_pScore = new CText(720, 15, 15);
  
	m_pHighscoreText = new CText("Erreichte Punktzahl: ", 210, 320, 15);
	m_pHighscore = new CText(560, 320, 15);
	
	m_pFramesPerSecond = new CText(10, 580, 12);
	showFPS = false;
  
	m_UpdateScore = true;
	m_UpdateLeben = true;
	m_UpdateLebensenergie = true;

	m_bGameRun = true;
	
	m_bPause = false;
	m_fPauseTimer = 0.0f;
	m_pPauseText = new CText("PAUSE",230, 260, 72);
	
	m_pVersion = new CText("0.14",10,580, 12);
} // Init

void CGame::Quit(){
	if(m_pPlayer != NULL){
		m_pPlayer->Quit();
		delete(m_pPlayer);
		m_pPlayer = NULL;
	}
  
	if(m_pSpriteBackground != NULL){
		delete(m_pSpriteBackground);
		m_pSpriteBackground = NULL;
	}
	if(m_pSpriteAsteroid != NULL){
		delete(m_pSpriteAsteroid);
		m_pSpriteAsteroid = NULL;
	}
	if(m_pSpriteExplosion != NULL){
		delete(m_pSpriteExplosion);
		m_pSpriteExplosion = NULL;
	}
} // Quit

void CGame::Run(){
	m_fStartRunTime = g_pTimer->GetCurTime();
	while(m_bGameRun == true && m_pPlayer->GetLeben() > 0 && m_pPlayer->GetLebensenergie_Raumstation() > 0){
		ProcessEvents();
  		FramesPerSecond();
		
		g_pFramework->Update();
		
		g_pFramework->Clear();
		
    		m_pSpriteBackground->Render();
		
 		RenderExplosions();
		
 		RenderSpieler();
 		RenderAsteroids(m_bPause);
		
		if(showFPS)
			m_pFramesPerSecond->Render();
		if(m_bPause == false){
 			m_pPlayer->Update();
		
 			SpawnAsteroids();
		
    			CheckCollisions();

  			UpdateTexts();
 			RenderTexts();
		}
		else
			RenderPause();
		
		g_pFramework->Flip();
      }
} // Run

void CGame::GameOver(){
	//Wenn der Spieler verloren hat, also nicht per Escape beendet wurde
	if(m_pPlayer->GetLeben() == 0 || m_pPlayer->GetLebensenergie_Raumstation() < 1){	
	float drei_sekunden = 0.0f;
	m_bNewHighscore = NewHighscore();
		while(drei_sekunden < 3.0f){
			g_pFramework->Clear();
			m_pSpriteBackground->Render();
			if(m_pPlayer->GetLeben() == 0)
				m_pGameOver = new CText("Du hast all deine Leben verloren!", 210,300, 15);
			else
				m_pGameOver = new CText("Die Raumstation wurde vernichtet!", 210,300, 15);
				
			if(m_bNewHighscore){
				m_pNewHighscoreText1 = new CText("Du hast einen neuen Highscore erreicht!", 210, 350, 15);
				m_pOldHighscoreText = new CText("Alter Highscore: ", 210, 380, 15);
				m_pOldHighscore = new CText(440, 380, 15);
				m_pNewHighscoreText2 = new CText("Neuer Highscore: ", 210, 410, 15);
				m_pNewHighscore = new CText(440, 410, 15);
				RenderNewHighscore();
			}
			m_pGameOver->Render();
			m_pHighscoreText->Render();
			m_pHighscore->Update(m_pPlayer->GetScore());
			m_pHighscore->Render();
			g_pFramework->Flip();
			drei_sekunden += g_pTimer->GetElapsed();
		}
	}
}

void CGame::ProcessEvents(){
	SDL_Event Event;
	if(SDL_PollEvent(&Event)){
		switch(Event.type){
			case (SDL_QUIT): {
				m_bGameRun = false;
			} break;
			case(SDL_KEYDOWN): {
				switch(Event.key.keysym.sym){
					case (SDLK_ESCAPE): {
						m_bGameRun = false;
					} break;
					case (SDLK_f):{
						toggleFPS();
					} break;
					case (SDLK_p):{
						togglePause();
					} break;
				}
			} break;
		}
	}
}

void CGame::SpawnAsteroids(){
	m_fAsteroidTimer += g_pTimer->GetElapsed();
  
	if(m_fAsteroidTimer >= m_SpawnTime){
		CAsteroid Asteroid;
    
		int XPos = ((SDL_GetTicks()+rand())*(m_pPlayer->GetScore()+1))%736;
		Asteroid.Init(m_pSpriteAsteroid, static_cast<float>(XPos), -60.0f, m_SpeedAsteroid);
    
		m_AsteroidList.push_back(Asteroid);
		m_fAsteroidTimer = 0.0f;
  }	
} // SpawnAsteroids

void CGame::CheckCollisions(){
	list<CShot> *ShotList = m_pPlayer->GetShotList();
	list<CAsteroid>::iterator ItAsteroid = m_AsteroidList.begin();
	list<CShot>::iterator ItShot;
  
	SDL_Rect RectAsteroid;
	SDL_Rect RectShot;
  
	while(ItAsteroid != m_AsteroidList.end()){
		RectAsteroid = ItAsteroid->GetRect();
//      		cout << "Beginn while-Schleife" << endl;

		// Trifft ein Asteroid das Schiff?
		if(m_fSpawnSchutz > 3.0f && RectAsteroid.y > 466 &&
			RectAsteroid.x + 64 - 10 > m_pPlayer->GetXPosition() &&
			RectAsteroid.x < (m_pPlayer->GetXPosition() + 54))
		{
			ItAsteroid->SetAlive(false);
			
 			m_UpdateLeben = true;
			SpawnExplosion(RectAsteroid.x, RectAsteroid.y, m_SpeedAsteroid/2.0f);
			SpawnExplosion(m_pPlayer->GetXPosition(), 530.0, m_SpeedAsteroid/2.0f);
			m_pPlayer->Reset();
			m_fSpawnSchutz = 0.0f;
			m_fSpawnSchutzTimer = 0.0f;
		}
		for(ItShot = ShotList->begin(); ItShot != ShotList->end(); ++ItShot){
//       		cout << "Beginn For-Schleife" << endl;
			RectShot = ItShot->GetRect();
			
			// Überschneiden sich die Rechtecke eines Asteroiden und eines Schusses?
			if	(RectShot.y < RectAsteroid.y + RectAsteroid.h &&
				RectShot.y + RectShot.h > RectAsteroid.y &&
				RectShot.x < RectAsteroid.x + 64 &&
				RectShot.x + RectShot.w > RectAsteroid.x){
				
				ItAsteroid->SetAlive(false);
				ItShot->SetAlive(false);
				m_pPlayer->DecreaseShots();	
				m_punkte = m_pPlayer->BerechnePunkte(RectAsteroid.y);
 				m_UpdateScore = true;
				SpawnExplosion(RectAsteroid.x, RectAsteroid.y, m_SpeedAsteroid/2.0f);
				SpawnScore(m_punkte, RectAsteroid.x, RectAsteroid.y);
			}
		}

		if(RectAsteroid.y > 590){
			ItAsteroid->SetAlive(false);
			m_pPlayer->Raumstation_Getroffen();
 			m_UpdateLebensenergie = true;
		}
		if(ItAsteroid->IsAlive())
			ItAsteroid++;
		else {
			ItAsteroid = m_AsteroidList.erase(ItAsteroid); 
		}
	}
} // CheckCollisions

void CGame::RenderAsteroids(bool pause){
	list<CAsteroid>::iterator It;
	for(It = m_AsteroidList.begin(); It != m_AsteroidList.end(); ++It){
		It->Render();
		It->Update(pause);
	}
} // RenderAsteroids

void CGame::RenderExplosions(){
	list<CExplosion>::iterator It;
	It = m_ExplosionList.begin();
	while(It != m_ExplosionList.end()){
		if(It->IsAlive()){
			if(m_bPause == false)
				It->Update();
			
			It->Render();
			It++;
		}
		else
			It = m_ExplosionList.erase(It);
	}
} // RenderExplosion

void CGame::RenderTexts(){
	// Alle (ggf geupdaten) Texte rendern
	m_pLebenText->Render();
	m_pLebensenergieText->Render();
	m_pScore_Text->Render();
	m_pLebensenergie->Render();
	m_pLeben->Render();
	m_pScore->Render();
} // RenderTexts

void CGame::FramesPerSecond(){
	frames++;
	fps = (frames*1000) / (SDL_GetTicks() - m_fStartRunTime);
	
	m_pFramesPerSecond->Update(fps);
//   	cout << "Frames: " << frames << endl;
// 	cout << "SDL GetTicks: " << SDL_GetTicks() << endl;
} // FramesPerSecond

void CGame::toggleFPS(){
// 	showFPS = showFPS ? false : true;
	showFPS = !showFPS;
} // toggleFPS

void CGame::togglePause(){
	m_bPause = m_bPause ? false : true;
} // toggleFPS


void CGame::UpdateTexts(){
	// Lebensenergie der Raumstation updaten?
  	if(m_UpdateLebensenergie == true)
		m_pLebensenergie->Update(m_pPlayer->GetLebensenergie_Raumstation());
	
	// Leben updaten?
   	if(m_UpdateLeben == true){
		m_pLeben->Update(m_pPlayer->GetLeben());
		m_UpdateLeben = false;
	}
	
	// Score updaten?
   	if(m_UpdateScore == true){
		m_pScore->Update(m_pPlayer->GetScore());
		m_UpdateScore = false;
	}
	
	// Wenn die Raumstation noch nicht komplett wieder repariert ist, soll sie im nächsten Frame upgedatet werden
 	if(m_pPlayer->GetLebensenergie_Raumstation() == 100)
		m_UpdateLebensenergie = false;	
	
	// Updaten der aufsteigend transparenter werdenden Score-Texte
	list<CText>::iterator ItText = m_TextList.begin();
	while(ItText != m_TextList.end()){
		// Ist der Text kurz vom verschwinden? Dann löschen.
		if(ItText->GetTransparency() < 20.0f)
			ItText = m_TextList.erase(ItText);
		else {
			ItText->Update(3.0f);
			ItText->Render();
			ItText++;
		}
	}
	
} // UpdateTexts

void CGame::SetSchwierigkeitsgrad(int schwierigkeitsgrad){
	if(schwierigkeitsgrad == 1){
		m_SpeedAsteroid = 150.0f;
		m_DmgRaumstation = 12;
		m_PlayerMaxSchuesse = 5;
		m_PlayerLeben = 7;
		m_RegenRaumstation = 0.2f;
		m_SpawnTime = 1.0f;
	}
	else if (schwierigkeitsgrad == 2){
		m_SpeedAsteroid = 175.0f;
		m_DmgRaumstation = 16;
		m_PlayerMaxSchuesse = 4;
		m_PlayerLeben = 6;
		m_RegenRaumstation = 0.35f;
		m_SpawnTime = 0.75f;
		
	}
	else{
		m_SpeedAsteroid = 200.0f;
		m_DmgRaumstation = 20;
		m_PlayerMaxSchuesse = 3;
		m_PlayerLeben = 5;
		m_RegenRaumstation = 0.5f;
		m_SpawnTime = 0.5f;
		
	}
} // SetSchwierigkeitsgrad

void CGame::SpawnExplosion(int x, int y, float speed){
	CExplosion Explosion;
	Explosion.Init(m_pSpriteExplosion, x, y, speed);
	m_ExplosionList.push_back(Explosion);
	m_fExplosionTimer = 0.0f;
}

void CGame::RenderPause(){
	m_fPauseTimer += g_pTimer->GetElapsed();
	// Wenn das Spiel pausiert ist, wird der Schriftzug PAUSE jeweils für 0,4 Sekunden gezeigt
	if(m_fPauseTimer > 0.4f){
		m_pPauseText->Render();
		if(m_fPauseTimer > 0.8f)
			m_fPauseTimer = 0.0f;
	}
}

void CGame::RenderSpieler(){
	if(m_bPause == false){
		m_fSpawnSchutzTimer += g_pTimer->GetElapsed();
		m_fSpawnSchutz += g_pTimer->GetElapsed();
	}
	
	// Wenn der Spieler vor weniger als 3 Sekunden getroffen wurde...
	if(m_fSpawnSchutz < 3.0f){
		//... wird er nur für jeweils 0,25 Sekunden gezeigt - blinkt
		if(m_fSpawnSchutzTimer > 0.25f){
			m_pPlayer->Render(m_bPause);
			if(m_fSpawnSchutzTimer > 0.5f)
				m_fSpawnSchutzTimer = 0.0f;
		}
	}
	else {
		m_pPlayer->Render(m_bPause);

	}
	m_pPlayer->RenderShots(m_bPause);
}



void CGame::SpawnScore(int punkte, int x, int y){
	// Casten der Punkte zu einem char*
	ostringstream temp;
	temp << punkte;
	string punkte_string;
	punkte_string = temp.str();
	char* punkte_char = const_cast<char*>(punkte_string.c_str());
	
	// Erzeugen eines neuen aufsteigenden Texts
	CText *PunkteText = new CText(punkte_char, x, y, 14);
	m_TextList.push_back(*PunkteText);
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

void CGame::RenderNewHighscore(){
	m_pNewHighscoreText1->Render();
	m_pNewHighscoreText2->Render();
	m_pOldHighscoreText->Render();
	
	m_pNewHighscore->Update(m_pPlayer->GetScore());
	m_pNewHighscore->Render();
	
	m_pOldHighscore->Update(m_AlterHighscore);
	m_pOldHighscore->Render();
}