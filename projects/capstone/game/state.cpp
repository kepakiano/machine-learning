#include "state.h"

#include <algorithm>

std::unordered_map<StateHash, StatePtr> State::hashed_states_;

State::State()
{

}

int State::getAsteroidState(std::list<CAsteroid> asteroid_list,
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
    int next_asteroid_position = 0;
    int second_next_asteroid_position = 0;

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
    int state_code = (there_is_an_asteroid_in_front_of_the_ship ? 1 : 0);
    state_code += next_asteroid_position * 10;
    state_code += second_next_asteroid_position * 100;

    return state_code;
}

std::list<ActionChoice> State::validActions(const float player_pos,
                                            const float weapons_array_cooldown)
{


}

StateHash State::hashState(const StatePtr &state)
{

}

void State::safeState(const StatePtr &state, const StateHash hash)
{
    hashed_states_.insert({hash, state});
}


State State::buildState(const std::list<CAsteroid> &asteroid_list,
                        const float player_pos,
                        const std::list<CShot>& shot_list,
                        const int player_lives,
                        const int space_station_health,
                        const float weapons_array_cooldown)
{

}

StatePtr State::getState(StateHash hash)
{
    if(hashed_states_.find(hash) == hashed_states_.end())
        throw "Requested state that is not in state map yet";
    return hashed_states_[hash];
}
