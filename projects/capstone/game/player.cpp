// player.cpp
#include "player.hpp"


CPlayer::CPlayer(){
	m_pSpritePlayer = NULL;
	m_pSpriteShot = NULL;
}

void CPlayer::Init(){
	m_pSpritePlayer = new CSprite;
  
    m_pSpritePlayer->Load("../assets/Player.png",11,64,64);
	m_pSpritePlayer->SetColorKey(255,0,255);
  
	m_pSpriteShot = new CSprite;
  
    m_pSpriteShot->Load("../assets/Laser.png",0,64,64);
	m_pSpriteShot->SetColorKey(255,0,255);
	m_CurShots = 0;
	m_Score = 0;

	m_Lebensenergie_Raumstation = 100;
	m_LebensenergieTimer = 0.0f;
} // Init

void CPlayer::SetWerte(int leben, int max_schuesse, int dmg_raumstation, float regen_raumstation){
	m_Leben = leben;
	m_MaxShots = max_schuesse;
	m_DmgRaumstation = dmg_raumstation;
	m_RegenRaumstation = regen_raumstation;
} // SetWerte

void CPlayer::Quit(){
	if(m_pSpritePlayer != NULL){
		delete(m_pSpritePlayer);
		m_pSpritePlayer = NULL;
	}
	if(m_pSpriteShot != NULL){
		delete(m_pSpriteShot);
		m_pSpriteShot = NULL;
	}
} // Quit

void CPlayer::Reset(){
	m_fXPos = 376.0f;
	m_fYPos = 520.0f;
	
	m_fAnimPhase = 5.0f;
  
	m_Leben--;
  
	m_bShotLock = false;
} // Reset

void CPlayer::Render(bool pause){
	m_pSpritePlayer->SetPos(m_fXPos, m_fYPos);
	m_pSpritePlayer->Render(m_fAnimPhase);
} // Render

void CPlayer::RenderShots(bool pause){
	list<CShot>::iterator it = m_ShotList.begin();
	m_CurShots = 0;
	while(it != m_ShotList.end()){
			
		if(pause == false)
			it->Update();
    		
		if(it->IsAlive()){
			it->Render();
			it++;
			m_CurShots++;
		}
		else {	
			it = m_ShotList.erase(it);
		}
	}
} // RenderShots

void CPlayer::Update(){
	m_LebensenergieTimer += g_pTimer->GetElapsed();
    Action action = getAction();
    ProcessAction(action);
	CheckPosition();
	
	if(m_Lebensenergie_Raumstation < 100 && m_LebensenergieTimer > m_RegenRaumstation){
		m_Lebensenergie_Raumstation++;
		m_LebensenergieTimer = 0.0f;
	}
	/*
	if(m_Lebensenergie_Raumstation > 100)
		m_Lebensenergie_Raumstation = 100;
	*/
} // Update

void CPlayer::ProcessAction(const Action &action){
    if(action == MOVE_LEFT){
		m_fXPos -= 300.0f *g_pTimer->GetElapsed();
		m_fAnimPhase -= 20.0f *g_pTimer->GetElapsed();
	}
    else if (action == MOVE_RIGHT){
		m_fXPos += 300.0f * g_pTimer->GetElapsed();
		m_fAnimPhase -= -20.0f * g_pTimer -> GetElapsed();
	}
	else {
		if (m_fAnimPhase > 5.0f)
			m_fAnimPhase -= 20.0f * g_pTimer->GetElapsed();
		if (m_fAnimPhase < 5.0f)
			m_fAnimPhase += 20.0f * g_pTimer->GetElapsed();
	}
    if (action == SHOOT){
		CShot Shot;
    
		Shot.Init(m_pSpriteShot, m_fXPos, m_fYPos);
    
		m_ShotList.push_back(Shot);
    
        m_bShotLock = true;
    
		m_CurShots++;
	}
} // ProcessShooting

void CPlayer::CheckPosition(){
	if(m_fXPos < 0.0f)
		m_fXPos = 0.0f;
	else if(m_fXPos > 752.0f)
		m_fXPos = 752.0f;
  
	if(m_fAnimPhase < 0.0f)
		m_fAnimPhase = 0.0f;
	else if(m_fAnimPhase > 10.0f)
		m_fAnimPhase = 10.0f;
} // CheckPostion

void CPlayer::DecreaseShots(){
	m_CurShots--;
} // DecreaseShots

int CPlayer::GetXPosition(){
	return m_fXPos;
} // GetXPosition

int CPlayer::GetLeben(){
	return m_Leben;
} // GetLeben

void CPlayer::Raumstation_Getroffen(){
	m_Lebensenergie_Raumstation -= m_DmgRaumstation;
} // RaumstationGetroffen

int CPlayer::GetLebensenergie_Raumstation(){
	return m_Lebensenergie_Raumstation;
} // GetLebensenergie_Raumstation

int CPlayer::GetScore(){
	return m_Score;
} // GetScore

int CPlayer::BerechnePunkte(int Asteroiden_Hoehe){
// 	m_Score += 60 +static_cast<int>(Asteroiden_Hoehe / 2);
	m_Score += 60 + Asteroiden_Hoehe / 2;
	return 60 +Asteroiden_Hoehe / 2;
} // BerechnePunkte
