#ifndef TEXTE_HPP
#define TEXTE_HPP

#include "SDL/SDL_ttf.h"
#include <string>

using namespace std;

class CText{
	public:
		CText(int x, int y, int size);
		CText(string str, int x, int y, int size);
		~CText();
		void Update(int wert);
		void Update(float speed);
		void Render();
		
		int GetTransparency(){
			return transparency;
		}
		
	private:
 		string m_wert;
		string m_komplett;
		string text;
		SDL_Surface *fertig;
		TTF_Font *font;
		SDL_Rect offset;
		SDL_Surface *m_pScreen;
		float TextTimer;
		SDL_Color TextFarbe;
		int transparency;
};
#endif
