#include "databaseconnection.h"
#include "framework.hpp"
#include "game.hpp"
#include "states.h"

int main(int argc, char *argv[]){

//    DatabaseConnection::createTables();
//    DatabaseConnection::addRowEnvironment(2.0, 3.0, 3.0, 1);
//    DatabaseConnection::addRowEnvironment(1.0, 3.0, 3.0, 1);
//    std::cout << DatabaseConnection::getIdEnvironment(1.0, 3.0, 3.0, 1) << std::endl;
//    std::cout << DatabaseConnection::getIdEnvironment(1.0, 3.0, 3.0, 1) << std::endl;
//    std::cout << DatabaseConnection::getIdEnvironment(1.0, 3.0, 3.0, 1) << std::endl;

//    return EXIT_SUCCESS;

    const int screen_width = 800;
    const int screen_height = 600;
  
    CGame Game(screen_width, screen_height);
  
    bool reinforcement_learning = false;
    bool use_sdl_renderer = true;
    bool bot_is_learning = true;
    double alpha, gamma;
    size_t training_runs = 1;
    size_t test_runs =0;
    double epsilon = 1.0;
    if(argc == 4){
        training_runs = std::stoi(argv[1]);
        alpha = std::stod(argv[2]);
        gamma = std::stod(argv[3]);
        reinforcement_learning = true;
        use_sdl_renderer = false;
        training_runs = 100;
        test_runs = 3;
    }
    for(size_t i = 0; i < training_runs+test_runs; ++i){
        if(i == training_runs && reinforcement_learning){
            bot_is_learning = false;
            use_sdl_renderer = true;
        }
        std::cout << "epsilon: " << epsilon << " ";
        Game.Init(use_sdl_renderer/*, reinforcement_learning, bot_is_learning, alpha, gamma, epsilon*/);
        epsilon *= 0.999;

        Game.Run();
        Game.GameOver();
        Game.Quit();

        if(!reinforcement_learning)
            break;
    }

    States::printStatistics();
  
  g_pFramework->Quit();
  g_pFramework->Del();
  
  return(0);
}
