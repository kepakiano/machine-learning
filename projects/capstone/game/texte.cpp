// texte.cpp
#include "texte.hpp"
#include <cstring>
#include "framework.hpp"
#include <sstream>

CText::~CText(){
	SDL_FreeSurface(fertig);
}
CText::CText(int x, int y, int size){
	m_pScreen = g_pFramework->GetScreen();
	offset.x = x;
	offset.y = y;
    font = TTF_OpenFont("../assets/ethnocent.ttf", size);
	TextTimer = 0.0f;
}


CText::CText(string str, int x, int y, int size){
	transparency = 255;
	m_pScreen = g_pFramework->GetScreen();
	offset.x = x;
	offset.y = y;
	text = str;
    font = TTF_OpenFont("../assets/ethnocent.ttf", size);
	TextTimer = 0.0f;
	SDL_Color TextFarbe = {40,240,40};
	fertig = TTF_RenderText_Solid(font, const_cast<char*>(str.c_str()), TextFarbe);
}

void CText::Update(int wert){
		ostringstream temp;
		temp << wert;
		m_komplett = temp.str();
		char* hinne_jetz = const_cast<char*>(m_komplett.c_str());
  		SDL_Color TextFarbe = {40,240,40};
		fertig = TTF_RenderText_Solid(font, hinne_jetz, TextFarbe);
}

void CText::Update(float speed){
	SDL_Color TextFarbe = {40,240,40};
	offset.y -= speed * g_pTimer->GetElapsed();
	transparency -= 300.0f * g_pTimer->GetElapsed();
 	SDL_SetAlpha(fertig, SDL_SRCALPHA, transparency);
}

void CText::Render(){
	SDL_BlitSurface(fertig, NULL, m_pScreen, &offset);
}
