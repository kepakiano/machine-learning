#include <numeric>
#include <iostream>

#include <boost/program_options.hpp>

#include "databaseconnection.h"
#include "framework.hpp"
#include "game.hpp"
#include "states.h"


int main(int argc, char** argv){

  double reward_space_station_hit_multiplier;
  double reward_no_event;
  double reward_ship_hit;
  double reward_game_over;
  size_t environment_number;
  double alpha;
  double gamma;

  DatabaseConnection::createTables();
  CGame Game(800, 600);

  const size_t test_runs = 100;
  double epsilon = 1.0;

  bool bot_is_learning = false;
  States states;

  std::vector<int> scores;
  int test_cases_id;
  if(argc == 2){
    test_cases_id = std::stoi(argv[1]);
    Environment env = states.loadStates(test_cases_id);
    alpha = env.alpha;
    gamma = env.gamma;
    environment_number = env.environment_id;
    std::cout << "Loaded environment with: alpha = " << alpha << ", gamma = " << gamma << ", environment_id = " << environment_number << std::endl;
  }

  bot_is_learning = false;
  epsilon = 0.0;
  for(size_t i = 0; i < test_runs; ++i){
    Game.configureReinforcementLearning(bot_is_learning, alpha, gamma, epsilon,
                                        reward_space_station_hit_multiplier,
                                        reward_no_event, reward_ship_hit,
                                        reward_game_over, environment_number,states);
    Game.Init(false);

    Game.Run();
    std::cout << i << " epsilon: " << epsilon <<  " ";
    Game.GameOver();

    scores.push_back(Game.getScore());
    Game.Quit();

  }


  double score_min = *std::min_element(scores.begin(), scores.end());
  double score_max = *std::max_element(scores.begin(), scores.end());
  double sum = std::accumulate(scores.begin(), scores.end(), 0.0);
  double mean = sum / scores.size();

  double score_avg = mean;

  std::vector<double> diff(scores.size());
  std::transform(scores.begin(), scores.end(), diff.begin(), [mean](double x) { return x - mean; });

  double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
  double stdev = std::sqrt(sq_sum / scores.size());
  double score_std = stdev;

  DatabaseConnection::addScore(test_cases_id, score_min, score_max, score_avg, score_std);
  std::cout << score_avg << " " << score_min << " " << score_max << std::endl;

  return EXIT_SUCCESS;
}
