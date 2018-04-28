// sprite.cpp
#include "sprite.hpp"
#include "SDL/SDL_image.h"

CSprite::CSprite(){
	m_pScreen = g_pFramework->GetScreen();
} // Konstruktor

CSprite::~CSprite(){
	SDL_FreeSurface(m_pImage);
} // Destruktor

void CSprite::Load(const string sFilename){
	m_pImage = IMG_Load (sFilename.c_str());
  
	if(m_pImage == NULL){
		cout << "Fehler beim Laden von: " << sFilename.c_str() << endl;
		cout << " Fehlermeldung: " << SDL_GetError() << endl;
    
		g_pFramework->Quit();
		exit (1);
	}
  
	m_Rect.x = 0;
	m_Rect.y = 0;
	m_Rect.w = m_pImage->w;
	m_Rect.h = m_pImage->h;
} // Load für nicht animierte Sprite

void CSprite::Load(const string sFilename, int NumFrames, int FrameWidth, int FrameHeight){
	Load(sFilename);
  
	m_NumFrames 	= NumFrames;
	m_FrameWidth 	= FrameWidth;
	m_FrameHeight = FrameHeight;
	m_FrameRect.w = FrameWidth;
	m_FrameRect.h = FrameHeight;
	m_NumFramesX 	= m_pImage->w / m_FrameWidth;
} // Load für animierte Sprites

void CSprite::SetColorKey(int R, int G, int B){
	SDL_SetColorKey(m_pImage, SDL_SRCCOLORKEY, SDL_MapRGB(m_pImage->format, R, G, B));
} // SetColorKey

void CSprite::SetPos(float fXPos, float fYPos){
	m_Rect.x = static_cast<int>(fXPos);
	m_Rect.y = static_cast<int>(fYPos);
} // SetPos

void CSprite::Render(){
	SDL_BlitSurface(m_pImage, NULL, m_pScreen, &m_Rect);
} // Render

void CSprite::Render(float fFrameNumber){
	int Column 	= static_cast<int>(fFrameNumber)%m_NumFramesX;
	int Row	= static_cast<int>(fFrameNumber)/m_NumFramesX;
  
	m_FrameRect.x = Column * m_FrameWidth;
	m_FrameRect.y = Row * m_FrameHeight;
  
	SDL_BlitSurface(m_pImage, &m_FrameRect, m_pScreen, &m_Rect);
} // Render