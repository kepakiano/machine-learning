#include <numeric>
#include <iostream>
#include <omp.h>

#include <boost/program_options.hpp>
#include <iomanip>
#include "databaseconnection.h"
#include "framework.hpp"
#include "game.hpp"
#include "states.h"

namespace
{
const size_t ERROR_IN_COMMAND_LINE = 1;
const size_t SUCCESS = 0;
const size_t ERROR_UNHANDLED_EXCEPTION = 2;
const size_t HELP_CALLED = 3;

} // namespace

int processCommandLineOptions(int argc,
                              char *argv[],
                              double &reward_space_station_hit,
                              double &reward_no_event,
                              double &reward_ship_hit,
                              double &reward_game_over,
                              size_t &environment_number,
                              double &alpha,
                              double &gamma,
                              size_t &epsilon_function,
                              size_t &training_runs)
{
  try
  {
    /** Define and parse the program options
         */
    namespace po = boost::program_options;
    po::options_description desc("Options");
    desc.add_options()
        ("help", "Print help messages")
        ("training_runs,r"           ,po::value<size_t>(&training_runs),            "number of training runs")
        ("reward-space-station-hit,s",po::value<double>(&reward_space_station_hit), "")
        ("reward-no-event,n",         po::value<double>(&reward_no_event),          "Resampling threshold")
        ("reward-ship-hit,h",         po::value<double>(&reward_ship_hit),          "Resampling threshold")
        ("reward_game_over,o",        po::value<double>(&reward_game_over),          "Resampling threshold")
        ("environment-number,e",      po::value<size_t>(&environment_number),       "Number of interspersed particles per gesture per resample step")
        ("alpha,a",                   po::value<double>(&alpha),                    "Resampling threshold")
        ("gamma,g",                   po::value<double>(&gamma),                    "Resampling threshold")
        ("epsilon_function_number,p", po::value<size_t>(&epsilon_function),         "Number of particles");

    po::variables_map vm;
    try
    {
      po::store(po::parse_command_line(argc, argv, desc),
                vm); // can throw


      /** --help option
           */
      if ( vm.count("help")  )
      {
        std::cout << "Basic Command Line Parameter App" << std::endl
                  << desc << std::endl;
        return HELP_CALLED;
      }
      for(const auto& parameter : {"reward-space-station-hit",
          "reward-no-event",
          "reward-ship-hit",
          "environment-number",
          "alpha",
          "gamma",
          "epsilon_function_number"}){
        if(!vm.count(parameter)){
          std::cerr << "ERROR: Please specify " << parameter << "." << std::endl << std::endl;
          return ERROR_IN_COMMAND_LINE;
        }
      }
      po::notify(vm); // throws on error, so do after help in case
      // there are any problems
    }

    catch(po::error& e)
    {
      std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
      std::cerr << desc << std::endl;
      return ERROR_IN_COMMAND_LINE;
    }

  }
  catch(std::exception& e)
  {
    std::cerr << "Unhandled Exception reached the top of main: "
              << e.what() << ", application will now exit" << std::endl;
    return ERROR_UNHANDLED_EXCEPTION;

  }

  return SUCCESS;
}


double epsilon1(size_t i, size_t n){
  const double epsilon_at_95_percent_of_runs = 0.000001;
  double percent95 = n * 0.95;
  const double x = std::pow(epsilon_at_95_percent_of_runs, 1.0/percent95);
  
  return std::pow(x, i);
}

double epsilon2(size_t i, size_t total_training_runs){
  return 1.0 - double(i)/total_training_runs;
}


double calculateEpsilon(size_t epsilon_function_number, double last_epsilon,
                        size_t current_training_run, size_t total_training_runs){
  if(epsilon_function_number == 1)
    return epsilon1(current_training_run, total_training_runs);
  if(epsilon_function_number == 2)
    return epsilon2(current_training_run, total_training_runs);
  else
    throw false;
}

int main(int argc, char** argv){
  double reward_space_station_hit_multiplier;
  double reward_no_event;
  double reward_ship_hit;
  double reward_game_over;
  size_t environment_number;
  double alpha;
  double gamma;
  size_t epsilon_function;
  size_t training_runs = 10000;

  if(processCommandLineOptions(argc, argv, reward_space_station_hit_multiplier,
                               reward_no_event, reward_ship_hit, reward_game_over,
                               environment_number, alpha, gamma,
                               epsilon_function, training_runs) != SUCCESS){
    return EXIT_FAILURE;
  }


  omp_set_num_threads(4);

#pragma omp parallel for
  for(environment_number = 1; environment_number < 5; ++environment_number){
    States states;

#pragma omp critical
    DatabaseConnection::createTables();
    CGame Game(800, 600);

    double epsilon = 1.0;

    bool bot_is_learning = true;

#pragma omp critical
    DatabaseConnection::addRowEnvironment(reward_space_station_hit_multiplier,
                                          reward_no_event, reward_ship_hit,
                                          reward_game_over, environment_number);

    int environment_id;
#pragma omp critical
    environment_id = DatabaseConnection::getIdEnvironment(reward_space_station_hit_multiplier,
                                                                    reward_no_event, reward_ship_hit, reward_game_over,
                                                                    environment_number);
    std::cout << std::fixed;
    std::vector<int> scores;
    for(size_t i = 0; i < training_runs; ++i){
      Game.configureReinforcementLearning(bot_is_learning, alpha, gamma, epsilon,
                                          reward_space_station_hit_multiplier,
                                          reward_no_event, reward_ship_hit,
                                          reward_game_over, environment_number,
                                          states);
      Game.Init(false);

      Game.Run();
      std::cout << i << " epsilon: " << std::setprecision(4) << epsilon <<  " ";
      Game.GameOver();
      Game.Quit();

      epsilon = calculateEpsilon(epsilon_function, epsilon, i, training_runs);
    }

#pragma omp critical
    DatabaseConnection::addRowTestCases(environment_id, alpha, gamma, epsilon_function, 0.0, 0.0, 0.0, 0.0, 0.0);

    int test_cases_id;
#pragma omp critical
    test_cases_id = DatabaseConnection::getIdTestCases(environment_id, alpha, gamma, epsilon_function);


    //  auto saved_state = States::hashedStates();
    //  Environment env = States::loadStates(test_cases_id);
    //  alpha = env.alpha;
    //  gamma = env.gamma;
    //  environment_number = env.environment_id;
    // Make sure the (de)serialization doesn't break anything
    //  for(const auto& p : States::hashedStates()){
    //    const auto& deserialized_state = p.second;
    //    const auto& original_state = saved_state.at(p.first);
    //    if(!(*deserialized_state == *original_state))
    //      throw std::string("Original and DB state are different");
    //  }


#pragma omp critical
    states.saveStates(test_cases_id);
  }
  return EXIT_SUCCESS;
}
