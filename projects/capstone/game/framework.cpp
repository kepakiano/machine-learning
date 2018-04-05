// framework.cpp
#include "framework.hpp"

bool CFramework::Init(int ScreenWidth, int ScreenHeight, int ColorDepth, bool bFullscreen){
	if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1){
		cout << "SDL konnte nicht initialisiert werden!" << endl;
		cout << "Fehlermeldung: " << SDL_GetError() << endl;
		Quit();
        return false;
	}
	
	if (bFullscreen == true)
		m_pScreen = SDL_SetVideoMode(ScreenWidth, ScreenHeight, ColorDepth, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
	else
		m_pScreen = SDL_SetVideoMode(ScreenWidth, ScreenHeight, ColorDepth, SDL_HWSURFACE | SDL_DOUBLEBUF);
  
	if(m_pScreen == NULL){
		cout << "Videomodus konnte nicht gesetzt werden!" << endl;
		cout << "Fehlermeldung: " << SDL_GetError() << endl;
		Quit();
        return false;
	}
  
	m_pKeystate = SDL_GetKeyState(NULL);
  
    return true;
} // Init

void CFramework::Quit(){
	SDL_Quit();
} // Quit

void CFramework::Update(){
	SDL_PumpEvents();
} // Update

bool CFramework::KeyDown(int Key_ID){
	return (m_pKeystate[Key_ID] ? true : false);
} // KeyDown

void CFramework::Clear(){
	SDL_FillRect(m_pScreen, NULL, SDL_MapRGB(m_pScreen->format, 0,0,0));
} // Clear

void CFramework::Flip(){
	SDL_Flip(m_pScreen);
} // Flip
