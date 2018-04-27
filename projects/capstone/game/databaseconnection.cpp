#include "databaseconnection.h"

#include <iostream>

#include <SQLiteCpp/Database.h>

const std::string DatabaseConnection::database_file = "asteroids.db3";

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
    SQLite::Database    db(database_file, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

    // Compile a SQL query, containing one parameter (index 1)
    std::string query_string_environment =  R"(CREATE TABLE )";
    query_string_environment += DatabaseConnection::ENVIRONMENTS + "(";
    query_string_environment += R"(environment_id INTEGER PRIMARY KEY,
                                   reward_space_station_hit REAL NOT NULL,
                                   reward_no_event REAL NOT NULL,
                                   reward_ship_hit REAL NOT NULL,
                                   environment_description TEXT NOT NULL,
                                   UNIQUE(reward_space_station_hit, reward_no_event, reward_ship_hit, environment_description) ON CONFLICT ABORT))";

    std::string query_string_test_cases = "CREATE TABLE ";
    query_string_test_cases += DatabaseConnection::TEST_CASES + "(";
    query_string_test_cases += R"(parameters_id INTEGER PRIMARY KEY,
                                  environment_id_fk INT NOT NULL,
                                  alpha REAL,
                                  gamma REAL,
                                  epsilon_function TEXT,
                                  score_avg REAL,
                                  score_std REAL,
                                  score_min REAL,
                                  time_taken REAL,
                                  FOREIGN KEY(environment_id_fk) REFERENCES ENVIRONMENTS(environment_id),
                                  UNIQUE(environment_id_fk) ON CONFLICT ABORT))";

    std::string query_string_states = "CREATE TABLE ";
    query_string_states += DatabaseConnection::STATES + "(";
    query_string_states += R"(state_id INTEGER PRIMARY KEY,
                              parameters_id_fk INT NOT NULL,
                              hash INT,
                              FOREIGN KEY(parameters_id_fk) REFERENCES PARAMETERS(parameters_id),
                              UNIQUE(parameters_id_fk) ON CONFLICT ABORT))";

    std::string query_string_actions = "CREATE TABLE ";
    query_string_actions += DatabaseConnection::ACTIONS + "(";
    query_string_actions += R"(action_id INTEGER PRIMARY KEY,
                               state_id_fk INT NOT NULL,
                               choice INT,
                               reward REAL,
                               FOREIGN KEY(state_id_fk) REFERENCES STATES(state_id),
                               UNIQUE(state_id_fk) ON CONFLICT ABORT))";

    SQLite::Statement query_environment(db, query_string_environment);
    SQLite::Statement query_test_cases(db, query_string_test_cases);
    SQLite::Statement query_states(db, query_string_states);
    SQLite::Statement query_actions(db, query_string_actions);
    std::cout << query_environment.exec() << std::endl;
    std::cout << query_test_cases.exec() << std::endl;
    std::cout << query_states.exec() << std::endl;
    std::cout << query_actions.exec() << std::endl;

}

void DatabaseConnection::addRowEnvironment(double reward_space_station_hit,
                                           double reward_no_event,
                                           double reward_ship_hit,
                                           const std::string &environment_description)
{

//    SQLite::Database db(database_file, SQLite::OPEN_READWRITE);
//    SQLite::Statement query(db, R"(INSERT INTO ENVIRONMENTS VALUES(NULL, ?, ?, ?, ?))");
//    query.bind(1, reward_space_station_hit);
//    query.bind(2, reward_no_event);
//    query.bind(3, reward_ship_hit);
//    query.bind(4, environment_description);
//    query.exec();
    insert("ENVIRONMENTS", {std::to_string(reward_space_station_hit),
           std::to_string(reward_no_event),
           std::to_string(reward_ship_hit),
           environment_description});
}

int DatabaseConnection::getIdEnvironment(double reward_space_station_hit,
                                         double reward_no_event,
                                         double reward_ship_hit,
                                         const std::string &environment_description)
{
    SQLite::Database db(database_file, SQLite::OPEN_READONLY);
    // Compile a SQL query, containing one parameter (index 1)
       SQLite::Statement   query(db, "SELECT environment_id " + ENVIRONMENTS +" WHERE \
                                        reward_space_station_hit = ? AND \
                                        reward_no_event = ? AND \
                                        reward_ship_hit = ? AND \
                                        environment_description = ?");

       // Bind the integer value 6 to the first parameter of the SQL query
       query.bind(1, reward_space_station_hit);
       query.bind(2, reward_no_event);
       query.bind(3, reward_ship_hit);
       query.bind(4, environment_description);

       query.exec();

       return query.getColumn(0);
}

void DatabaseConnection::addRowTestCases(const size_t environment_id,
                                          const double alpha,
                                          const double gamma,
                                          const std::string &epsilon_function,
                                          const double score_avg,
                                          const double score_std,
                                          const double score_min,
                                          const double time_taken)
{
    insert("PARAMETERS", {
               std::to_string(environment_id),
               std::to_string(alpha),
               std::to_string(gamma),
               epsilon_function,
               std::to_string(score_avg),
               std::to_string(score_std),
               std::to_string(score_min),
               std::to_string(time_taken)
           });
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
