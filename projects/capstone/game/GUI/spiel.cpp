#include "databaseconnection.h"
#include "framework.hpp"
#include "game.hpp"
#include "states.h"

int main(int argc, char *argv[]){
  const int screen_width = 800;
  const int screen_height = 600;
  
  CGame Game(screen_width, screen_height);
  
  bool reinforcement_learning = false;
  const bool use_sdl_renderer = true;
  const bool bot_is_learning = false;
  double alpha, gamma;
  size_t training_runs = 1;
  size_t test_runs =0;
  double epsilon = 0.0;
  size_t test_cases_id;
  size_t environment_number;
  States states;

  if(argc == 2){
    test_cases_id = std::stoi(argv[1]);
    Environment env = states.loadStates(test_cases_id);
    alpha = env.alpha;
    gamma = env.gamma;
    environment_number = env.environment_id;
    reinforcement_learning = true;
    std::cout << "Loaded environment with: alpha = " << alpha << ", gamma = " << gamma << ", environment_id = " << environment_number << ", test_cases_id = " << test_cases_id << std::endl;
  }
  for(size_t i = 0; i < training_runs+test_runs; ++i){
    if(reinforcement_learning){

      Game.configureReinforcementLearning(bot_is_learning, alpha, gamma, epsilon,
                                          0.0,
                                          0.0, 0.0,
                                          0.0, environment_number,
                                          states);

    }
    std::cout << "epsilon: " << epsilon << " ";
    Game.Init(true);

    Game.Run();
    Game.GameOver();
    Game.Quit();

    if(!reinforcement_learning)
      break;
  }

  states.printStatistics();
  
  g_pFramework->Quit();
  g_pFramework->Del();
  
  return(0);
}
