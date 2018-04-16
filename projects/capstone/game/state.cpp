#include "state.h"

#include <algorithm>

std::unordered_map<StateHash, StatePtr> State::hashed_states_;

State::State(const size_t hash,
             const std::list<ActionPtr>& valid_actions)
    : hash_(hash)
    , actions_(valid_actions)
{
    for(const auto& action : valid_actions)
        actions_map_.insert({action->choice(), action});
}

size_t State::getAsteroidState(std::list<CAsteroid> asteroid_list,
                            const float player_position)
{

    asteroid_list.sort(
              [](const CAsteroid &a1, const CAsteroid &a2
              ){
        return a1.GetYPos() > a2.GetYPos();
    });

    bool there_is_an_asteroid_in_front_of_the_ship = false;

    // 1 = asteroid is left
    // 2 = asteroid is right
    // 0 = asteroid does not exist
    size_t next_asteroid_position = 0;
    size_t second_next_asteroid_position = 0;

    for(const CAsteroid &asteroid : asteroid_list){

        Rectangle RectAsteroid = asteroid.getRect();

        bool asteroid_is_in_front_of_the_ship = RectAsteroid.x + 64 > player_position && RectAsteroid.x < player_position + 64;
        there_is_an_asteroid_in_front_of_the_ship |= asteroid_is_in_front_of_the_ship;
        if(!asteroid_is_in_front_of_the_ship){
            if(next_asteroid_position == 0){
                if(RectAsteroid.x < player_position)
                    next_asteroid_position = 1;
                else
                    next_asteroid_position = 2;
                continue;
            }
            if(next_asteroid_position != 0 && second_next_asteroid_position == 0){
                if(RectAsteroid.x < player_position)
                    second_next_asteroid_position = 1;
                else
                    second_next_asteroid_position = 2;
                continue;
            }
        }
    }

    // We use five bit to encode the asteroid state
    size_t asteroid_state = 0;
    if(there_is_an_asteroid_in_front_of_the_ship)
        asteroid_state += 1u;
    asteroid_state <<= 1;
    asteroid_state += next_asteroid_position;
    asteroid_state <<= 2;
    asteroid_state += second_next_asteroid_position;
    asteroid_state <<= 2;

    return asteroid_state;
}

std::list<ActionChoice> State::validActions(const float player_pos,
                                            const float weapons_array_cooldown)
{
    std::list<ActionChoice> valid_actions = {PASS};
    if(player_pos > 0.0)
        valid_actions.push_back(MOVE_RIGHT);
    if(player_pos < 752.0)
        valid_actions.push_back(MOVE_LEFT);
    if(weapons_array_cooldown > 0.5)
        valid_actions.push_back(SHOOT);

    return valid_actions;
}


void State::safeState(const StatePtr &state, const StateHash hash)
{
    hashed_states_.insert({hash, state});
}

StatePtr State::buildState(const size_t hash,
                           const std::list<CAsteroid> &asteroid_list,
                           const float player_pos,
                           const std::list<CShot> &shot_list,
                           const int player_lives,
                           const int space_station_health,
                           const float weapons_array_cooldown)
{
    const auto valid_action_choices = validActions(player_pos, weapons_array_cooldown);
    std::list<ActionPtr> valid_actions;
    for(const auto& action_choice : valid_action_choices)
        valid_actions.push_back(std::make_shared<Action>(action_choice));
    return std::make_shared<State>(hash, valid_actions);
}

size_t State::hashState(const std::list<CAsteroid> &asteroid_list, const float player_pos, const std::list<CShot> &shot_list, const int player_lives, const int space_station_health, const float weapons_array_cooldown)
{
    return getAsteroidState(asteroid_list, player_pos);
}

StatePtr State::getState(const std::list<CAsteroid> &asteroid_list,
                         const float player_pos,
                         const std::list<CShot>& shot_list,
                         const int player_lives,
                         const int space_station_health,
                         const float weapons_array_cooldown)
{
    const size_t hash = hashState(asteroid_list,
                                player_pos,
                                shot_list,
                                player_lives,
                                space_station_health,
                                weapons_array_cooldown);

    if(hashed_states_.find(hash) == hashed_states_.end()){
        hashed_states_[hash] = buildState(hash,
                                          asteroid_list,
                                          player_pos,
                                          shot_list,
                                          player_lives,
                                          space_station_health,
                                          weapons_array_cooldown);
    }
    return hashed_states_[hash];
}

void State::loadStates()
{

}

void State::saveStates()
{

}
