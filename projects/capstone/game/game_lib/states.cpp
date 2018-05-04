#include "states.h"

#include "databaseconnection.h"

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

size_t States::hashState1(const std::list<CAsteroid> &asteroid_list,
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

size_t States::hashState2(const std::list<CAsteroid> &asteroid_list,
                         const float player_pos,
                         const std::list<CShot> &shot_list,
                         const int player_lives,
                         const int space_station_health,
                         const float weapons_array_cooldown)
{
    size_t hash = getAsteroidState(asteroid_list, player_pos);
//    hash <<= 1;
//    hash += (weapons_array_cooldown > 0.5f) ? 1u : 0u;
    hash <<= 2;
    hash += space_station_health <= 0 ? 0 : space_station_health / 34 + 1;
    return hash;
}

size_t States::hashState3(const std::list<CAsteroid> &asteroid_list,
                         const float player_pos,
                         const std::list<CShot> &shot_list,
                         const int player_lives,
                         const int space_station_health,
                         const float weapons_array_cooldown)
{
    size_t hash = hashState2(asteroid_list, player_pos,
                             shot_list, player_lives, space_station_health,
                             weapons_array_cooldown);
    hash <<= 1;
    hash += player_lives > 1;
    return hash;
}


size_t States::hashState4(const std::list<CAsteroid> &asteroid_list,
                         const float player_pos,
                         const std::list<CShot> &shot_list,
                         const int player_lives,
                         const int space_station_health,
                         const float weapons_array_cooldown)
{
    size_t hash = hashState3(asteroid_list, player_pos,
                             shot_list, player_lives, space_station_health,
                             weapons_array_cooldown);

    std::vector<int> asteroid_per_spawnpoint(8, 0);
    for(const auto& asteroid : asteroid_list){
      asteroid_per_spawnpoint[(int(asteroid.GetXPos())-68)/100]++;
    }

    std::vector<int> shots_per_spawnpoint(8, 0);
    for(const auto& shot : shot_list){
      for(size_t spawn_point_x = 68; spawn_point_x < 800; spawn_point_x += 100 ){
        if(shot.GetXPos() < spawn_point_x + 64 &&
           shot.GetXPos() + 64 > spawn_point_x){
          shots_per_spawnpoint[(spawn_point_x-68)/100]++;
        }
      }
    }

    int num_asteroids_getting_hit = 0;
    for(size_t i = 0; i < asteroid_per_spawnpoint.size(); ++i){
      num_asteroids_getting_hit += std::min(asteroid_per_spawnpoint[i],
                                            shots_per_spawnpoint[i]);
    }

    hash <<= 4;
    hash += std::min(16, num_asteroids_getting_hit);


    return hash;
}

size_t States::hashState(const std::list<CAsteroid> &asteroid_list,
                         const float player_pos,
                         const std::list<CShot> &shot_list,
                         const int player_lives,
                         const int space_station_health,
                         const float weapons_array_cooldown,
                         const size_t environment_number)
{
  if(environment_number == 1)
    return hashState1(asteroid_list, player_pos, shot_list, player_lives, space_station_health, weapons_array_cooldown);
  if(environment_number == 2)
    return hashState2(asteroid_list, player_pos, shot_list, player_lives, space_station_health, weapons_array_cooldown);
  if(environment_number == 3)
    return hashState3(asteroid_list, player_pos, shot_list, player_lives, space_station_health, weapons_array_cooldown);
  if(environment_number == 4)
    return hashState4(asteroid_list, player_pos, shot_list, player_lives, space_station_health, weapons_array_cooldown);
  throw false;
}

StatePtr States::getState(const std::list<CAsteroid> &asteroid_list,
                         const float player_pos,
                         const std::list<CShot>& shot_list,
                         const int player_lives,
                         const int space_station_health,
                         const float weapons_array_cooldown,
                         const size_t environment_number)
{
    const size_t hash = hashState(asteroid_list,
                                player_pos,
                                shot_list,
                                player_lives,
                                space_station_health,
                                weapons_array_cooldown,
                                environment_number);

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

void States::saveStates(const int test_cases_id)
{
  DatabaseConnection::safeStates(hashed_states_, test_cases_id);
}

void States::printStatistics()
{
    std::cout << "Total states: " << hashed_states_.size() << std::endl;
    for(const auto& state_pair : hashed_states_)
        std::cout << state_pair.second->hash() << " " << state_pair.second->visited() << std::endl;
}
