#ifndef STATES_H
#define STATES_H

#include "environment.h"
#include "explosion.hpp"
#include "state.h"

class States
{
public:
    static StatePtr getState(const std::list<CAsteroid> &asteroid_list,
                            const float player_pos,
                            const std::list<CShot>& shot_list,
                             const std::list<CExplosion> &explosion_list,
                            const int player_lives,
                            const int space_station_health,
                            const float weapons_array_cooldown, const size_t environment_number);

    static Environment loadStates(const int test_cases_id);
    static void saveStates(const int test_cases_id);

    static void printStatistics();


    static std::unordered_map<StateHash, StatePtr> hashedStates() {return hashed_states_;}
    static void removeAsteroidsAndShotsThatWillGetHit(std::list<CAsteroid>& asteroid_list, std::list<CShot>& shot_list);
    static void removeUnreachableAsteroids(std::list<CAsteroid>& asteroid_list, const double player_pos);
private:
    static std::unordered_map<StateHash, StatePtr> hashed_states_;

    States() = delete;

    static std::list<ActionChoice> validActions(const float player_pos,
                                                const float weapons_array_cooldown);
    static StateHash hashState(const StatePtr &state);
    static void safeState(const StatePtr& state, const StateHash hash);

    static StatePtr buildState(const std::list<CAsteroid> &asteroid_list,
                               const float player_pos,
                               const std::list<CShot>& shot_list,
                               const std::list<CExplosion> &explosion_list,
                               const int player_lives,
                               const int space_station_health,
                               const float weapons_array_cooldown);
    static size_t hashState(const StatePtr &state, const size_t environment_number);

    static size_t hashState1(const StatePtr &state);
    static size_t hashState2(const StatePtr &state);
    static size_t hashState3(const StatePtr &state);
    static size_t hashState5(const StatePtr &state);

    static void statePossibleActions(const StatePtr& state, StateHash &hash);
    static void stateAsteroids(const StatePtr& state, StateHash &hash);
    static void stateExplosions(const StatePtr& state, StateHash &hash);
    static void stateNextAsteroidThatDoesNotGetHit(const StatePtr& state, StateHash &hash);
    static void stateAsteroidsGoingToHitTheShip(const StatePtr& state, StateHash &hash);
    static void stateNextAsteroidThatDoesNotGetHitAndIsReachable(const StatePtr& state, StateHash &hash);
public:
    static size_t hashState4(const StatePtr &state);
    static size_t   getAsteroidState(std::list<CAsteroid> asteroid_list,
                         const float player_position, const std::list<CExplosion> &explosion_list);
};

#endif // STATES_H
