#include "states.h"
#include <algorithm>
#include <cassert>
#include "databaseconnection.h"

std::unordered_map<StateHash, StatePtr> States::hashed_states_;

/**
 * 1. Add information where the next unshot asteroid _that can be reached_ is
 * 2. Make the agent aware that there might be an asteroid behind the next unshot asteroid
 * 3. Add player's lives to state
 *
 */


std::list<ActionChoice> States::validActions(const float player_pos,
                                             const float weapons_array_cooldown)
{
  std::list<ActionChoice> valid_actions = {};
  if(player_pos > 0.0)
    valid_actions.push_back(MOVE_LEFT);
  if(player_pos < 752.0)
    valid_actions.push_back(MOVE_RIGHT);
  if(weapons_array_cooldown > 0.5)
    valid_actions.push_back(SHOOT);

  return valid_actions;
}

void States::statePossibleActions(const StatePtr& state, StateHash &hash){
  auto valid_actions = validActions(state->player_pos(), state->weapons_array_cooldown());
  hash <<= 1;
  if(std::find(valid_actions.begin(), valid_actions.end(), MOVE_RIGHT) != valid_actions.end()){
    hash += 1;
  }
  hash <<= 1;
  if(std::find(valid_actions.begin(), valid_actions.end(), MOVE_LEFT) != valid_actions.end()){
    hash += 1;
  }
  hash <<= 1;
  if(std::find(valid_actions.begin(), valid_actions.end(), SHOOT) != valid_actions.end()){
    hash += 1;
  }
}

void States::stateAsteroidsGoingToHitTheShip(const StatePtr& state, StateHash &hash){
  hash <<= 1;

  const float overlap = 3.0f;
  for(const auto& asteroid : state->asteroid_list()){
    if(asteroid.getRect().y > 460 &&
       asteroid.getRect().x + asteroid.getRect().w + overlap > state->player_pos() &&
       asteroid.getRect().x + asteroid.getRect().w < state->player_pos()){
      hash += 1;
      break;
    }
  }

  hash <<= 1;
  for(const auto& asteroid : state->asteroid_list()){
    if(asteroid.getRect().y > 460 &&
       asteroid.getRect().x - overlap < state->player_pos() + 64 &&
       asteroid.getRect().x > state->player_pos() + 64){
      hash += 1;
      break;
    }
  }

  hash <<= 1;
  for(const auto& asteroid : state->asteroid_list()){
    if(asteroid.getRect().y > 460 &&
       asteroid.getRect().overlapsX(Rectangle(state->player_pos(), 0, 64, 64))){
      hash += 1;
      break;
    }
  }

}

void States::stateAsteroids(const StatePtr& state, StateHash &hash){
  auto asteroid_list = state->asteroid_list();
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
    Rectangle RectPlayer(state->player_pos(), 0, 64, 64);

    bool asteroid_is_in_front_of_the_ship = RectAsteroid.overlapsX(RectPlayer, 1.0);
    there_is_an_asteroid_in_front_of_the_ship |= asteroid_is_in_front_of_the_ship;
    if(!asteroid_is_in_front_of_the_ship){
      if(next_asteroid_position == 0){
        if(RectAsteroid.x < state->player_pos())
          next_asteroid_position = 1;
        else
          next_asteroid_position = 2;
        continue;
      }
      if(next_asteroid_position != 0 && second_next_asteroid_position == 0){
        if(RectAsteroid.x < state->player_pos())
          second_next_asteroid_position = 1;
        else
          second_next_asteroid_position = 2;
        continue;
      }
    }
  }

  // We use five bit to encode the asteroid state
  hash <<= 1;
  if(there_is_an_asteroid_in_front_of_the_ship)
    hash += 1u;
  hash <<= 2;
  hash += next_asteroid_position;
  hash <<= 2;
  hash += second_next_asteroid_position;
}

void States::stateExplosions(const StatePtr& state, StateHash &hash){
  hash <<= 2;
  hash += std::min(4, int(state->explosion_list().size()));
}

void States::stateNextAsteroidThatDoesNotGetHit(const StatePtr& state, StateHash &hash)
{
  auto asteroid_list = state->asteroid_list();
  hash <<= 2;

  asteroid_list.sort(
        [](const CAsteroid &a1, const CAsteroid &a2
        ){
    return a1.GetYPos() > a2.GetYPos();
  });

  auto shot_list = state->shot_list();

  if(asteroid_list.size() >= 2)
    if(asteroid_list.front().getRect().y < asteroid_list.back().getRect().y)
      throw std::string("List is not properly sorted");

//  auto asteroids_iter = asteroid_list.begin();
//  while(asteroids_iter != asteroid_list.end()){
//    auto shots_iter = shot_list.begin();
//    bool asteroid_iter_was_advanced = false;
//    while(shots_iter != shot_list.end()){
//      if(shots_iter->GetRect().overlapsX(asteroids_iter->getRect())){
//        shots_iter = shot_list.erase(shots_iter);
//        asteroids_iter = asteroid_list.erase(asteroids_iter);
//        asteroid_iter_was_advanced = true;
//      }
//      else{
//        shots_iter++;
//      }
//    }
//    if(!asteroid_iter_was_advanced)
//      asteroids_iter++;
//  }


  const double overlap_pixels = 2.0;

  for(const auto& asteroid : asteroid_list){
    auto RectAsteroid = asteroid.getRect();
    for(const auto& shot : shot_list){
      auto RectShot = shot.GetRect();
      if	(RectShot.overlapsX(RectAsteroid)){
        continue;
        throw std::string("Asteroid overlaps shot, should not happen!");
      }
      if(RectAsteroid.overlapsX(Rectangle(state->player_pos(), 0, 64, 64), overlap_pixels)){
        hash += 1;
      }
      else if(state->player_pos() > RectAsteroid.x){
        hash += 2;
      }
      else{
        hash += 3;
      }
      return;
    }
  }
}

void stateWeaponsArray(const StatePtr& state, StateHash &hash){
  hash <<= 1;
  hash += (state->weapons_array_cooldown() > 0.5f) ? 1u : 0u;
}

void stateSpaceStationHealth(const StatePtr& state, StateHash &hash){
  hash <<= 2;
  hash += state->space_station_health() <= 0 ? 0 : state->space_station_health() / 34 + 1;
}

void statePlayerLives(const StatePtr& state, StateHash &hash){
  hash <<= 1;
  hash += state->player_lives() > 1;
}

void statePlayerPos(const StatePtr &state, StateHash &hash){
  hash <<= 4;
  hash += state->player_pos() / 16;
}

void States::safeState(const StatePtr &state, const StateHash hash)
{
  hashed_states_.insert({hash, state});
}

StatePtr States::buildState(
    const std::list<CAsteroid> &asteroid_list,
    const float player_pos,
    const std::list<CShot> &shot_list,
    const std::list<CExplosion> &explosion_list,
    const int player_lives,
    const int space_station_health,
    const float weapons_array_cooldown)
{
  const auto valid_action_choices = validActions(player_pos, weapons_array_cooldown);
  std::list<ActionPtr> valid_actions;
  for(const auto& action_choice : valid_action_choices)
    valid_actions.push_back(std::make_shared<Action>(action_choice));
  return std::make_shared<State>(valid_actions, asteroid_list, player_pos, shot_list, explosion_list, player_lives, space_station_health, weapons_array_cooldown);
}


size_t States::hashState1(const StatePtr &state)
{
  StateHash hash = 1;
  statePossibleActions(state, hash);
//  stateAsteroids(state, hash);
  stateWeaponsArray(state, hash);
  stateNextAsteroidThatDoesNotGetHit(state, hash);
  stateAsteroidsGoingToHitTheShip(state, hash);
  return hash;
}

size_t States::hashState2(const StatePtr &state)
{

}

size_t States::hashState3(const StatePtr &state)
{
  size_t hash = hashState2(state);
  hash <<= 1;
  hash += state->player_lives() > 1;
  return hash;
}


size_t States::hashState4(const StatePtr &state)
{
  size_t hash = hashState3(state);

  std::vector<int> asteroid_per_spawnpoint(8, 0);
  for(const auto& asteroid : state->asteroid_list()){
    asteroid_per_spawnpoint[(int(asteroid.GetXPos())-68)/100]++;
  }

  std::vector<int> shots_per_spawnpoint(8, 0);
  for(const auto& shot : state->shot_list()){
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

  std::cout << num_asteroids_getting_hit << std::endl;

  return hash;
}

size_t States::hashState5(const StatePtr &state)
{
  size_t hash = hashState3(state);

  hash <<= 4;
  hash += state->player_pos() / 16;

  return hash;
}

size_t States::hashState(const StatePtr& state,
                         const size_t environment_number)
{
  if(environment_number == 1)
    return hashState1(state);
  if(environment_number == 2)
    return hashState2(state);
  if(environment_number == 3)
    return hashState3(state);
  if(environment_number == 4)
    return hashState4(state);
  if(environment_number == 5)
    return hashState5(state);
  throw false;
}

StatePtr States::getState(const std::list<CAsteroid> &asteroid_list,
                          const float player_pos,
                          const std::list<CShot>& shot_list,
                          const std::list<CExplosion> &explosion_list,
                          const int player_lives,
                          const int space_station_health,
                          const float weapons_array_cooldown,
                          const size_t environment_number)
{
  StatePtr possibly_new_state = buildState(
        asteroid_list,
        player_pos,
        shot_list,
        explosion_list,
        player_lives,
        space_station_health,
        weapons_array_cooldown);
  const size_t hash = hashState(possibly_new_state, environment_number);
  possibly_new_state->setHash(hash);

  if(hashed_states_.find(hash) == hashed_states_.end()){
    hashed_states_[hash] = possibly_new_state;
    std::cout << "New state: " << hash << std::endl;
  }
  else {
    assert(possibly_new_state->actions().size() == hashed_states_[hash]->actions().size());
  }
  hashed_states_[hash]->visit();
  return hashed_states_[hash];
}

Environment States::loadStates(const int test_cases_id)
{
  hashed_states_.clear();
  return DatabaseConnection::loadStates(hashed_states_, test_cases_id);

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
