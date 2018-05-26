#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <string>
#include <vector>

#include "environment.h"
#include "states.h"

class DatabaseConnection
{
public:
    DatabaseConnection();

    static void createTables();
    static void addRowEnvironment(const double reward_space_station_hit_multiplier,
                                  const double reward_no_event,
                                  const double reward_ship_hit,
                                  const double reward_game_over,
                                  const size_t &environment_description);

    static int getIdEnvironment(const double reward_space_station_hit_multiplier,
                                const double reward_no_event,
                                const double reward_ship_hit,
                                const double reward_game_over,
                                const size_t &environment_description);
    
    static void addRowTestCases(const size_t environment_id,
                                const double alpha,
                                const double gamma,
                                const size_t &epsilon_function,
                                const double score_avg,
                                const double score_std,
                                const double score_min,
                                const double score_max,
                                const double time_taken);

    static int getIdTestCases(const size_t environment_id,
                              const double alpha,
                              const double gamma,
                              const size_t epsilon_function);

    static int getIdStates(const size_t test_cases_id, const size_t hash);

    static void safeStates(const std::unordered_map<StateHash, StatePtr>& states,
                          const size_t test_cases_id);

    static void addScore(const size_t test_cases_id,
                         const double score_min,
                         const double score_max,
                         const double score_avg, const double score_std);

    static Environment loadStates(std::unordered_map<StateHash, StatePtr>& states,
                          const size_t test_cases_id);

    static std::vector<int> getAllTestCaseIds();
private:
    static const std::string database_file;

    static const std::string ENVIRONMENTS;
    static const std::string TEST_CASES;
    static const std::string STATES;
    static const std::string ACTIONS;
    
    static void insert(const std::string& table,
                       const std::vector<std::string>& values);
};

#endif // DATABASECONNECTION_H
