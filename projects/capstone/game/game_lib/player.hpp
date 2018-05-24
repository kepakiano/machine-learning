// player.hpp
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <list>

#include "actionchoice.h"
#include "asteroid.hpp"
#include "explosion.hpp"
#include "sprite.hpp"
#include "shot.hpp"

#include "state.h"

class CPlayer{
public:
  CPlayer	();
  virtual ~CPlayer() = default;
  void Init	();
  void Quit	();
  void UpdateShots(const double seconds);
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

  virtual void setCurrentAction(ActionPtr){}

  bool shouldBeRendered(){
    return m_fSpawnSchutz > 3.0f || m_fSpawnSchutzTimer > 0.25f;
  }

  float GetSpawnSchutz() const {return m_fSpawnSchutz;}

  double getDamageToSpaceStation() const {return m_DmgRaumstation;}

  virtual void computeState(const std::list<CAsteroid> & asteroid_list,
                            const std::list<CExplosion> &explosion_list) = 0;
  virtual void learn(const double reward, const StatePtr &new_state) = 0;
  virtual StatePtr getCurrentState() const = 0;
  virtual ActionChoice chooseAction() const = 0;

  Rectangle getRect() const {
    return Rectangle(m_fXPos, m_fYPos, 64, 64);
  }

  float getWeaponArrayCooldown() const{
    return m_fShotCooldownTimer;
  }

  float speed() const {return m_fSpeed;}
protected:
  bool isShootingPossible() const{
    return m_fShotCooldownTimer >= m_fShotCooldown;
  }

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

  const float m_fSpeed;

  list<CShot> m_ShotList;
};

#endif
