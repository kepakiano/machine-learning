#ifndef STATES_H
#define STATES_H

#include "state.h"

class States
{
public:
    static StatePtr getState(const std::list<CAsteroid> &asteroid_list,
                            const float player_pos,
                            const std::list<CShot>& shot_list,
                            const int player_lives,
                            const int space_station_health,
                            const float weapons_array_cooldown);

    static void loadStates();
    static void saveStates(const int test_cases_id);

    static void printStatistics();
private:
    static std::unordered_map<StateHash, StatePtr> hashed_states_;

    States() = delete;

    static std::list<ActionChoice> validActions(const float player_pos,
                                                const float weapons_array_cooldown);
    static StateHash hashState(const std::list<CAsteroid> &asteroid_list, const float player_pos);
    static void safeState(const StatePtr& state, const StateHash hash);

    static StatePtr buildState(const size_t hash,
                               const std::list<CAsteroid> &asteroid_list,
                               const float player_pos,
                               const std::list<CShot>& shot_list,
                               const int player_lives,
                               const int space_station_health,
                               const float weapons_array_cooldown);
    static size_t hashState( const std::list<CAsteroid> &asteroid_list,
                             const float player_pos,
                             const std::list<CShot>& shot_list,
                             const int player_lives,
                             const int space_station_health,
                             const float weapons_array_cooldown);
    static size_t getAsteroidState(std::list<CAsteroid> asteroid_list,
                         const float player_position);
};

#endif // STATES_H
