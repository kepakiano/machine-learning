#include "framework.hpp"
#include "game.hpp"

int main(int argc, char *argv[]){
    const int screen_width = 800;
    const int screen_height = 600;
    if(g_pFramework->Init(screen_width, screen_height,16,false) == false)
		return 0;
  
    CGame Game(screen_height, screen_width);
  
	Game.Init();
	Game.Menu();
  
	Game.Run();
	Game.GameOver();
	Game.Quit();
  
	g_pFramework->Quit();
	g_pFramework->Del();
  
	return(0);
}
