#include "databaseconnection.h"
#include "framework.hpp"
#include "game.hpp"
#include "states.h"

int main(int argc, char *argv[]){
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
    size_t test_cases_id;
    size_t environment_number;
    if(argc == 4){
        training_runs = std::stoi(argv[1]);
        alpha = std::stod(argv[2]);
        gamma = std::stod(argv[3]);
        reinforcement_learning = true;
        use_sdl_renderer = false;
        training_runs = 100;
        test_runs = 3;
    }
    if(argc == 2){
      test_cases_id = std::stoi(argv[1]);
      Environment env = States::loadStates(test_cases_id);
      alpha = env.alpha;
      gamma = env.gamma;
      environment_number = env.environment_id;
      reinforcement_learning = true;
      bot_is_learning = false;
      std::cout << "Loaded environment with: alpha = " << alpha << ", gamma = " << gamma << ", environment_id = " << environment_number << std::endl;
    }
    for(size_t i = 0; i < training_runs+test_runs; ++i){
        if(i == training_runs && reinforcement_learning){
            bot_is_learning = false;
            use_sdl_renderer = true;
        }
        if(reinforcement_learning){

          Game.configureReinforcementLearning(bot_is_learning, alpha, gamma, epsilon,
                                              0.0,
                                              0.0, 0.0,
                                              0.0, environment_number);

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
