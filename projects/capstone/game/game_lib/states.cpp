#include "states.h"


std::unordered_map<StateHash, StatePtr> States::hashed_states_;

std::list<ActionChoice> States::validActions(const float player_pos,
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

size_t States::getAsteroidState(std::list<CAsteroid> asteroid_list,
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
    asteroid_state <<= 2;
    asteroid_state += next_asteroid_position;
    asteroid_state <<= 2;
    asteroid_state += second_next_asteroid_position;

    return asteroid_state;
}


void States::safeState(const StatePtr &state, const StateHash hash)
{
    hashed_states_.insert({hash, state});
}

StatePtr States::buildState(const size_t hash,
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
    std::cout << "New state: " << hash << std::endl;
    return std::make_shared<State>(hash, valid_actions);
}

size_t States::hashState(const std::list<CAsteroid> &asteroid_list,
                         const float player_pos,
                         const std::list<CShot> &shot_list,
                         const int player_lives,
                         const int space_station_health,
                         const float weapons_array_cooldown)
{
    size_t hash = getAsteroidState(asteroid_list, player_pos);
    hash <<= 1;
    hash += (weapons_array_cooldown > 0.5f) ? 1u : 0u;
    return hash;
}

StatePtr States::getState(const std::list<CAsteroid> &asteroid_list,
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
    hashed_states_[hash]->visit();
    return hashed_states_[hash];
}

void States::loadStates()
{

}

void States::saveStates()
{

}

void States::printStatistics()
{
    std::cout << "Total states: " << hashed_states_.size() << std::endl;
    for(const auto& state_pair : hashed_states_)
        std::cout << state_pair.second->hash() << " " << state_pair.second->visited() << std::endl;
}
