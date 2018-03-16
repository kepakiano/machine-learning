#include "framework.hpp"
#include "game.hpp"

int main(int argc, char *argv[]){
    const int screen_width = 800;
    const int screen_height = 600;
  
    CGame Game(screen_width, screen_height);
  
	Game.Init();
  
	Game.Run();
	Game.GameOver();
	Game.Quit();
  
	g_pFramework->Quit();
	g_pFramework->Del();
  
	return(0);
}
