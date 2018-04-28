#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <string>
#include <vector>

class DatabaseConnection
{
public:
    DatabaseConnection();

    static void createTables();
    static void addRowEnvironment(double reward_space_station_hit,
                                  double reward_no_event,
                                  double reward_ship_hit,
                                  const std::string& environment_description);

    static int getIdEnvironment(double reward_space_station_hit,
                                double reward_no_event,
                                double reward_ship_hit,
                                const std::string& environment_description);
    
    static void addRowTestCases(const size_t environment_id,
                                 const double alpha,
                                 const double gamma,
                                 const std::string& epsilon_function,
                                 const double score_avg,
                                 const double score_std,
                                 const double score_min, const double time_taken);

    static int getIdTestCases(const size_t environment_id,
                              const double alpha,
                              const double gamma,
                              const std::string& epsilon_function,
                              const double score_avg,
                              const double score_std,
                              const double score_min,
                              const double time_taken);
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
