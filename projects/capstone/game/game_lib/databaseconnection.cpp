#include "databaseconnection.h"

#include <iostream>

#include <SQLiteCpp/Database.h>

const std::string DatabaseConnection::database_file = "../ReinforcementLearningTraining/asteroids.db3";

const std::string DatabaseConnection::ENVIRONMENTS = "ENVIRONMENTS";
const std::string DatabaseConnection::TEST_CASES = "TEST_CASES";
const std::string DatabaseConnection::STATES = "STATES";
const std::string DatabaseConnection::ACTIONS = "ACTIONS";

DatabaseConnection::DatabaseConnection()
{
  
}

void DatabaseConnection::createTables()
{
  
  // Open a database file
  SQLite::Database db(database_file, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

  // Compile a SQL query, containing one parameter (index 1)
  std::string query_string_environment =  R"(CREATE TABLE )";
  query_string_environment += DatabaseConnection::ENVIRONMENTS + "(";
  query_string_environment += R"(environment_id INTEGER PRIMARY KEY,
                              reward_space_station_hit_multiplier REAL NOT NULL,
                              reward_no_event REAL NOT NULL,
                              reward_ship_hit REAL NOT NULL,
                              reward_game_over REAL NOT NULL,
                              environment_description TEXT NOT NULL,
                              UNIQUE(reward_space_station_hit_multiplier, reward_no_event, reward_ship_hit, reward_game_over, environment_description) ON CONFLICT IGNORE))";
  
  std::string query_string_test_cases = "CREATE TABLE ";
  query_string_test_cases += DatabaseConnection::TEST_CASES + "(";
  query_string_test_cases += R"(test_cases_id INTEGER PRIMARY KEY,
                             environment_id_fk INT NOT NULL,
                             alpha INT,
                             gamma INT,
                             epsilon_function TEXT,
                             score_avg REAL,
                             score_std REAL,
                             score_min REAL,
                             score_max REAL,
                             time_taken REAL,
                             FOREIGN KEY(environment_id_fk) REFERENCES ENVIRONMENTS(environment_id) ON DELETE CASCADE,
                             UNIQUE(environment_id_fk, alpha, gamma, epsilon_function) ON CONFLICT REPLACE))";
  
  std::string query_string_states = "CREATE TABLE ";
  query_string_states += DatabaseConnection::STATES + "(";
  query_string_states += R"(state_id INTEGER PRIMARY KEY,
                         test_cases_id_fk INT NOT NULL,
                         hash INT,
                         FOREIGN KEY(test_cases_id_fk) REFERENCES TEST_CASES(test_cases_id) ON DELETE CASCADE,
                         UNIQUE(test_cases_id_fk, hash) ON CONFLICT REPLACE))";
  
  std::string query_string_actions = "CREATE TABLE ";
  query_string_actions += DatabaseConnection::ACTIONS + "(";
  query_string_actions += R"(action_id INTEGER PRIMARY KEY,
                          state_id_fk INT NOT NULL,
                          choice INT,
                          reward REAL,
                          FOREIGN KEY(state_id_fk) REFERENCES STATES(state_id) ON DELETE CASCADE,
                          UNIQUE(state_id_fk, choice) ON CONFLICT REPLACE))";

  if(!db.tableExists(ENVIRONMENTS)){
    SQLite::Statement query_environment(db, query_string_environment);
    std::cout << query_environment.exec() << std::endl;
  }
  if(!db.tableExists(TEST_CASES)){
    SQLite::Statement query_test_cases(db, query_string_test_cases);
    std::cout << query_test_cases.exec() << std::endl;
  }
  if(!db.tableExists(STATES)){
    SQLite::Statement query_states(db, query_string_states);
    std::cout << query_states.exec() << std::endl;
  }
  if(!db.tableExists(ACTIONS)){
    SQLite::Statement query_actions(db, query_string_actions);
    std::cout << query_actions.exec() << std::endl;
  }
}

void DatabaseConnection::addRowEnvironment(const double reward_space_station_hit_multiplier,
                                           const double reward_no_event,
                                           const double reward_ship_hit, const double reward_game_over,
                                           const size_t &environment_description)
{
   insert("ENVIRONMENTS", {std::to_string(reward_space_station_hit_multiplier),
                          std::to_string(reward_no_event),
                          std::to_string(reward_ship_hit),
                          std::to_string(reward_game_over),
                          std::to_string(environment_description)});
}

int DatabaseConnection::getIdEnvironment(const double reward_space_station_hit_multiplier,
                                         const double reward_no_event,
                                         const double reward_ship_hit, const double reward_game_over,
                                         const size_t &environment_description)
{
  SQLite::Database db(database_file, SQLite::OPEN_READONLY);
  SQLite::Statement query(db, "SELECT environment_id from " + ENVIRONMENTS +" WHERE \
                                  reward_space_station_hit_multiplier = ? AND \
                                  reward_no_event = ? AND \
                                  reward_ship_hit = ? AND \
                                  reward_game_over = ? AND \
                                  environment_description = ?");
                                  
  query.bind(1, reward_space_station_hit_multiplier);
  query.bind(2, reward_no_event);
  query.bind(3, reward_ship_hit);
  query.bind(4, reward_game_over);
  query.bind(5, std::to_string(environment_description));
  query.executeStep();
  
  return query.getColumn(0);
}

void DatabaseConnection::addRowTestCases(const size_t environment_id,
                                         const double alpha,
                                         const double gamma,
                                         const size_t &epsilon_function,
                                         const double score_avg,
                                         const double score_std,
                                         const double score_min,
                                         const double score_max,
                                         const double time_taken)
{
  insert(TEST_CASES, {
           std::to_string(environment_id),
           std::to_string(int(alpha*100)),
           std::to_string(int(gamma*100)),
           std::to_string(epsilon_function),
           std::to_string(score_avg),
           std::to_string(score_std),
           std::to_string(score_min),
           std::to_string(score_max),
           std::to_string(time_taken)
         });
}

int DatabaseConnection::getIdTestCases(const size_t environment_id,
                                       const double alpha,
                                       const double gamma,
                                       const size_t epsilon_function)
{
  SQLite::Database db(database_file, SQLite::OPEN_READONLY);
  SQLite::Statement query(db, "SELECT test_cases_id from " + TEST_CASES +" WHERE \
                                  environment_id_fk = ? AND \
                                  alpha = ? AND \
                                  gamma = ? AND \
                                  epsilon_function = ?");

  query.bind(1, std::to_string(environment_id));
  query.bind(2, int(alpha*100));
  query.bind(3, int(gamma*100));
  query.bind(4, std::to_string(epsilon_function));
  query.executeStep();

  return query.getColumn(0);
}

int DatabaseConnection::getIdStates(const size_t test_cases_id,
                                    const size_t hash)
{
  SQLite::Database db(database_file, SQLite::OPEN_READONLY);
  SQLite::Statement query_get_states(db, "SELECT * FROM " + STATES + " WHERE test_cases_id_fk = ? AND hash = ?");
  query_get_states.bind(1, std::to_string(test_cases_id));
  query_get_states.bind(2, std::to_string(hash));

  if(query_get_states.executeStep())
    return query_get_states.getColumn(0);
  return -1;
}

void DatabaseConnection::safeStates(const std::unordered_map<StateHash, StatePtr> &states,
                                   const size_t test_cases_id)
{
  SQLite::Database db(database_file, SQLite::OPEN_READWRITE);
  SQLite::Statement query_get_state(db, "SELECT * FROM " + STATES + " WHERE  test_cases_id_fk = ?");
  query_get_state.bind(1, std::to_string(test_cases_id));

  if(query_get_state.executeStep()){
    SQLite::Statement query_delete(db, "DELETE FROM " + STATES + " WHERE  test_cases_id_fk = ?");
    query_delete.bind(1, std::to_string(test_cases_id));
    query_delete.executeStep();
  }

  for(const auto& state_pair : states){
    insert(STATES, {std::to_string(test_cases_id),
                    std::to_string(state_pair.first)});
    auto state_id = getIdStates(test_cases_id, state_pair.second->hash());
    for(const auto& action : state_pair.second->actions()){
      insert(ACTIONS, {std::to_string(state_id),
                       std::to_string(action->choice()),
                       std::to_string(action->reward())});
    }
  }
}

void DatabaseConnection::addScore(const size_t test_cases_id, const double score_min, const double score_max, const double score_avg, const double score_std)
{

  SQLite::Database db(database_file, SQLite::OPEN_READWRITE);
  SQLite::Statement query_get_test_cases(db, "UPDATE " + TEST_CASES + " SET score_avg = ?, score_std = ?, score_min = ?, score_max = ? WHERE test_cases_id = ?");
  query_get_test_cases.bind(1, std::to_string(score_avg));
  query_get_test_cases.bind(2, std::to_string(score_std));
  query_get_test_cases.bind(3, std::to_string(score_min));
  query_get_test_cases.bind(4, std::to_string(score_max));
  query_get_test_cases.bind(5, std::to_string(test_cases_id));
  query_get_test_cases.executeStep();
}

Environment DatabaseConnection::loadStates(std::unordered_map<StateHash, StatePtr> &states, const size_t test_cases_id)
{
  SQLite::Database db(database_file, SQLite::OPEN_READONLY);
  SQLite::Statement query_get_state(db, "SELECT * FROM " + STATES + " WHERE  test_cases_id_fk = ?");
  query_get_state.bind(1, std::to_string(test_cases_id));

//  std::cout << "This should be 0: " << states.size() << " states" << std::endl;
  while(query_get_state.executeStep()){
    int states_id = query_get_state.getColumn(0);
    int hash = query_get_state.getColumn(2);

    SQLite::Statement query_get_actions(db, "SELECT * FROM " + ACTIONS + " WHERE  state_id_fk = ?");
    query_get_actions.bind(1, std::to_string(states_id));

    std::list<ActionPtr> actions;
    while(query_get_actions.executeStep()){
      ActionChoice choice = static_cast<ActionChoice>(query_get_actions.getColumn(2).getInt());
      double reward = query_get_actions.getColumn(3).getDouble();

      actions.push_back(std::make_shared<Action>(choice, reward));
    }

    StatePtr state = std::make_shared<State>(hash, actions);
    states.insert(std::make_pair(hash, state));
  }

//  std::cout << "Loaded " << states.size() << " states" << std::endl;

  SQLite::Statement query_get_test_cases(db, "SELECT * FROM " + TEST_CASES + " WHERE test_cases_id = ?");
  query_get_test_cases.bind(1, std::to_string(test_cases_id));
  query_get_test_cases.executeStep();
  int environment_id_fk = query_get_test_cases.getColumn(1);
  double alpha = query_get_test_cases.getColumn(2);
  double gamma = query_get_test_cases.getColumn(3);
  alpha /= 100.0;
  gamma /= 100.0;

  SQLite::Statement query_get_env(db, "SELECT * FROM " + ENVIRONMENTS + " WHERE environment_id = ?");
  query_get_env.bind(1, std::to_string(environment_id_fk));
  query_get_env.executeStep();
  int environment_id = std::stod(query_get_env.getColumn(5));

  return Environment(environment_id, alpha, gamma);
}

std::vector<int> DatabaseConnection::getAllTestCaseIds()
{

  SQLite::Database db(database_file, SQLite::OPEN_READONLY);
  SQLite::Statement query_get_test_cases(db, "SELECT test_cases_id FROM " + TEST_CASES);

  std::vector<int> test_cases_ids;
  while(query_get_test_cases.executeStep()){
    int test_cases_id = std::stoi(query_get_test_cases.getColumn(0));
    test_cases_ids.push_back(test_cases_id);
  }

  return test_cases_ids;
}

void DatabaseConnection::insert(const std::string &table,
                                const std::vector<std::string>& values)
{
  SQLite::Database db(database_file, SQLite::OPEN_READWRITE);
  std::string query_string = "INSERT INTO " + table + " VALUES(NULL";
  for(size_t i = 0; i < values.size(); ++i)
    query_string += ", ?";
  query_string += ")";
  SQLite::Statement query(db, query_string);
  for(size_t i = 0; i < values.size(); ++i)
    query.bind(i+1, values[i]);
  query.exec();
}
