#ifndef STATE_H
#define STATE_H

#include <list>
#include <map>
#include <memory>
#include <unordered_map>

#include "action.h"
#include "actionchoice.h"
#include "asteroid.hpp"
#include "shot.hpp"

class State;

using StatePtr = std::shared_ptr<State>;
using StateHash = unsigned int long;

class State
{
public:
  State(const size_t hash, const std::list<ActionPtr> &valid_actions);

  StateHash hash() const {return hash_;}
  std::list<ActionPtr> actions() const {return actions_;}
  ActionPtr action(ActionChoice choice) {return actions_map_[choice];}

  static StatePtr getState(const std::list<CAsteroid> &asteroid_list,
                          const float player_pos,
                          const std::list<CShot>& shot_list,
                          const int player_lives,
                          const int space_station_health,
                          const float weapons_array_cooldown);

  static void loadStates();
  static void saveStates();

private:
  static size_t getAsteroidState(std::list<CAsteroid> asteroid_list,
                       const float player_position);

  StateHash hash_;
  std::map<ActionChoice, ActionPtr> actions_map_;
  std::list<ActionPtr> actions_;

  static std::list<ActionChoice> validActions(const float player_pos,
                                              const float weapons_array_cooldown);
  static std::unordered_map<StateHash, StatePtr> hashed_states_;
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


};

#endif // STATE_H
