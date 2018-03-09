#include "framework.hpp"
#include "game.hpp"

int main(int argc, char *argv[]){
	if(g_pFramework->Init(800,600,16,false) == false)
		return 0;
  
	CGame Game;
  
	Game.Init();
	Game.Menu();
  
	Game.Run();
	Game.GameOver();
	Game.Quit();
  
	g_pFramework->Quit();
	g_pFramework->Del();
  
	return(0);
}