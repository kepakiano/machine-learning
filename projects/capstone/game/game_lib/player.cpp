// player.cpp
#include "player.hpp"


CPlayer::CPlayer()
  : m_fSpawnSchutz(3.0f)
  , m_fSpawnSchutzTimer(0.0f)
  , m_fShotCooldown(0.5f)
  , m_fShotCooldownTimer(0.5f)
  , m_fSpeed(300.0f)
{
}

void CPlayer::Init(){
  m_Score = 0;

  m_Lebensenergie_Raumstation = 100;
  m_LebensenergieTimer = 0.0f;

  //    m_fSpawnSchutz = 0.0f;
  m_fSpawnSchutzTimer = 0.0f;
} // Init

void CPlayer::SetWerte(int leben, int dmg_raumstation, float regen_raumstation){
  m_Leben = leben;
  m_DmgRaumstation = dmg_raumstation;
  m_RegenRaumstation = regen_raumstation;
} // SetWerte

void CPlayer::Quit(){
} // Quit

void CPlayer::Reset(){
  m_fXPos = 376.0f;
  m_fYPos = 520.0f;

  m_fAnimPhase = 5.0f;
  
  m_Leben--;

  //    m_fSpawnSchutz = 0.0f;
  m_fSpawnSchutzTimer = 0.0f;

  m_fShotCooldownTimer = 0.5f;
} // Reset

void CPlayer::UpdateShots(const double seconds){
  list<CShot>::iterator it = m_ShotList.begin();
  while(it != m_ShotList.end()){
    it->Update(seconds);

    if(it->IsAlive()){
      it++;
    }
    else {
      it = m_ShotList.erase(it);
    }
  }
} // RenderShots

void CPlayer::Update(const double seconds,
                     const ActionChoice &action){
  m_LebensenergieTimer += seconds;
  m_fSpawnSchutzTimer += seconds;
  m_fSpawnSchutz += seconds;
  m_fShotCooldownTimer += seconds;

  if(m_fSpawnSchutzTimer > 0.5f)
    m_fSpawnSchutzTimer = 0.0f;

  ProcessAction(action, seconds);
  CheckPosition();

  if(m_Lebensenergie_Raumstation < 100 && m_LebensenergieTimer > m_RegenRaumstation){
    m_Lebensenergie_Raumstation++;
    m_LebensenergieTimer = 0.0f;
  }
}

void CPlayer::ProcessAction(const ActionChoice &action, const double seconds){
  if(action == MOVE_LEFT){
    m_fXPos -= m_fSpeed * seconds;
    m_fAnimPhase -= 20.0f * seconds;
  }
  else if (action == MOVE_RIGHT){
    m_fXPos += m_fSpeed * seconds;
    m_fAnimPhase -= -20.0f * seconds;
  }
  else {
    if (m_fAnimPhase > 5.0f)
      m_fAnimPhase -= 20.0f * seconds;
    if (m_fAnimPhase < 5.0f)
      m_fAnimPhase += 20.0f * seconds;
  }
  if (action == SHOOT){
    CShot shot(m_fXPos, m_fYPos);
    m_ShotList.push_back(shot);
    m_fShotCooldownTimer = 0.0f;
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
  const int score = (600-Asteroiden_Hoehe)-60;
  m_Score += score;
  return score;
} // BerechnePunkte
