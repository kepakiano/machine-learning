#include "states.h"
#include <algorithm>
#include <cassert>
#include "databaseconnection.h"

//std::unordered_map<StateHash, StatePtr> States::hashed_states_;

/**
 * 1. Add information where the next unshot asteroid _that can be reached_ is
 * 2. Make the agent aware that there might be an asteroid behind the next unshot asteroid
 * 3. Add player's lives to state
 * 4. Add the info that there is an asteroid in front of the ship
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


  const double update_time = 0.016666;
  std::vector<bool> ship_gets_hit = {false, false, false};
  std::vector<double> new_ship_positions = {state->player_pos() + 300.0 * update_time, state->player_pos(), state->player_pos() - 300.0 * update_time};
  auto asteroid_list = state->asteroid_list();
  for(auto& asteroid : asteroid_list){
    asteroid.Update(update_time);
    const auto& RectAsteroid = asteroid.getRect();
    for(size_t i = 0; i < new_ship_positions.size(); ++i){
      const double& player_pos = new_ship_positions[i];

      if(RectAsteroid.y > 466 &&
         RectAsteroid.x + 64 - 10 > player_pos &&
         RectAsteroid.x < (player_pos + 54)){
        ship_gets_hit[i] = true;
      }
    }
  }
  for(bool collision : ship_gets_hit){
    hash <<= 1;
    hash += collision;
  }

//  const float overlap = 3.0f;
//  for(const auto& asteroid : state->asteroid_list()){
//    if(asteroid.getRect().y > 460 &&
//       asteroid.getRect().x + asteroid.getRect().w + overlap > state->player_pos() &&
//       asteroid.getRect().x + asteroid.getRect().w < state->player_pos()){
//      hash += 1;
//      break;
//    }
//  }

//  hash <<= 1;
//  for(const auto& asteroid : state->asteroid_list()){
//    if(asteroid.getRect().y > 460 &&
//       asteroid.getRect().x - overlap < state->player_pos() + 64 &&
//       asteroid.getRect().x > state->player_pos() + 64){
//      hash += 1;
//      break;
//    }
//  }

//  hash <<= 1;
//  for(const auto& asteroid : state->asteroid_list()){
//    if(asteroid.getRect().y > 460 &&
//       asteroid.getRect().overlapsX(Rectangle(state->player_pos(), 0, 64, 64))){
//      hash += 1;
//      break;
//    }
//  }
}

void States::removeUnreachableAsteroids(std::list<CAsteroid>& asteroid_list, const double player_pos)
{
  for(auto iter = asteroid_list.begin(); iter != asteroid_list.end();){
    if((466-iter->getRect().y)/iter->speed() < (std::abs(iter->getRect().x -player_pos)-54)/300.0){
      iter = asteroid_list.erase(iter);
    }
    else{
      ++iter;
    }
  }

}

void States::removeAsteroidsAndShotsThatWillGetHit(std::list<CAsteroid> &asteroid_list, std::list<CShot> &shot_list)
{
  asteroid_list.sort(
        [](const CAsteroid &a1, const CAsteroid &a2
        ){
    return a1.GetYPos() > a2.GetYPos();
  });

  auto asteroids_iter = asteroid_list.begin();
  while(asteroids_iter != asteroid_list.end()){
    auto shots_iter = shot_list.begin();
    bool asteroid_iter_was_advanced = false;
    while(shots_iter != shot_list.end()){
      if(shots_iter->GetRect().overlapsX(asteroids_iter->getRect()) &&
         shots_iter->GetRect().y > asteroids_iter->getRect().y){
        shots_iter = shot_list.erase(shots_iter);
        asteroids_iter = asteroid_list.erase(asteroids_iter);
        asteroid_iter_was_advanced = true;

        if(asteroids_iter == asteroid_list.end())
          return;
      }
      else{
        shots_iter++;
      }
    }
    if(!asteroid_iter_was_advanced)
      asteroids_iter++;
  }
}

void States::stateAsteroids(const StatePtr& state, StateHash &hash){
  auto asteroid_list = state->asteroid_list();
  auto shot_list = state->shot_list();

  removeAsteroidsAndShotsThatWillGetHit(asteroid_list, shot_list);
  removeUnreachableAsteroids(asteroid_list, state->player_pos());

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
}

void States::stateExplosions(const StatePtr& state, StateHash &hash){
  hash <<= 2;
  hash += std::min(4, int(state->explosion_list().size()));
}

void States::stateNextAsteroidThatDoesNotGetHit(const StatePtr& state, StateHash &hash)
{
  std::list<CAsteroid> asteroid_list = state->asteroid_list();
  std::list<CShot> shot_list = state->shot_list();

  removeAsteroidsAndShotsThatWillGetHit(asteroid_list, shot_list);
  removeUnreachableAsteroids(asteroid_list, state->player_pos());

  if(asteroid_list.size() >= 2)
    if(asteroid_list.front().getRect().y < asteroid_list.back().getRect().y)
      throw std::string("List is not properly sorted");

  const double overlap_pixels = 2.0;

  hash <<= 2;
  for(const auto& asteroid : asteroid_list){
    const auto& RectAsteroid = asteroid.getRect();
    for(const auto& shot : shot_list){
      const auto& RectShot = shot.GetRect();
      if	(RectShot.overlapsX(RectAsteroid) && RectAsteroid.y < RectShot.y){
//        continue;
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
  stateWeaponsArray(state, hash);
  stateNextAsteroidThatDoesNotGetHit(state, hash);
  return hash;
}

size_t States::hashState2(const StatePtr &state)
{
  StateHash hash = hashState1(state);
  stateAsteroidsGoingToHitTheShip(state, hash);
  return hash;

}

size_t States::hashState3(const StatePtr &state)
{
  StateHash hash = hashState2(state);
  statePlayerLives(state, hash);
  return hash;
}

size_t States::hashState4(const StatePtr &state)
{
  size_t hash = hashState3(state);
  stateSpaceStationHealth(state, hash);

  return hash;
}

size_t States::hashState5(const StatePtr &state)
{
  size_t hash = hashState4(state);
  stateExplosions(state, hash);
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
  size_t num_actions = 0;
  double sum_reward = 0.0;
  for(const auto& p : hashedStates()){
    sum_reward += std::accumulate(p.second->actions().begin(),
                                   p.second->actions().end(),
                                   0.0,
                                   [](double s, const auto& action_ptr){
      return s + action_ptr->reward();
    });
    num_actions += p.second->actions().size();
  }
  std::cout << "Total actions: " << num_actions << std::endl;
  std::cout << "Total reward: " << sum_reward << std::endl;
//  for(const auto& state_pair : hashed_states_)
//    std::cout << state_pair.second->hash() << " " << state_pair.second->visited() << std::endl;
}
