#ifndef SHOT_HPP
#define SHOT_HPP

#include "rectangle.hpp"
#include "sprite.hpp"

class CShot{
public:
  CShot(float fXPos, float fYPos);
  void Update	(const double seconds);

  bool IsAlive() const {
    return m_bIsAlive;
  }

  void SetAlive(bool bIsAlive){
    m_bIsAlive = bIsAlive;
  }

  float GetXPos() const {return m_fXPos;}
  float GetYPos() const {return m_fYPos;}

  int getLifetime() const {return lifetime;}

  Rectangle GetRect() const {
    return m_Rect;
  }

private:
  float m_fXPos;
  float m_fYPos;
  bool m_bIsAlive;
  Rectangle m_Rect;

  int lifetime;
};
#endif
