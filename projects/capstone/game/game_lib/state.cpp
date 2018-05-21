#include "state.h"

#include <algorithm>


State::State(const std::list<ActionPtr>& valid_actions,
             const std::list<CAsteroid> &asteroid_list,
             const float player_pos,
             const std::list<CShot> &shot_list,
              const std::list<CExplosion> &explosion_list,
             const int player_lives,
             const int space_station_health,
             const float weapons_array_cooldown)
    :
      actions_(valid_actions)
    , asteroid_list_(asteroid_list)
    , player_pos_(player_pos)
    , shot_list_(shot_list)
    , explosion_list_(explosion_list)
    , player_lives_(player_lives)
    , space_station_health_(space_station_health)
    , weapons_array_cooldown_(weapons_array_cooldown)
    , visited_(0)
{
    for(const auto& action : valid_actions)
      actions_map_.insert({action->choice(), action});
}

State::State(const int hash, const std::list<ActionPtr> actions)
  : hash_(hash)
  , actions_(actions)
  , player_pos_(-1.0)
  , player_lives_(-1)
  , space_station_health_(-1)
  , weapons_array_cooldown_(-0.5)
  , visited_(0)
{
  for(const auto& action : actions)
    actions_map_.insert(std::make_pair(action->choice(), action));
  
}
