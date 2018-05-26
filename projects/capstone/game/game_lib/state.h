#ifndef STATE_H
#define STATE_H

#include <cmath>
#include <list>
#include <map>
#include <memory>
#include <unordered_map>

#include "action.h"
#include "actionchoice.h"
#include "asteroid.hpp"
#include "explosion.hpp"
#include "shot.hpp"

class State;

using StatePtr = std::shared_ptr<State>;
using StateHash = unsigned int long;

class State
{
public:
  State( const std::list<ActionPtr> &valid_actions,
         const std::list<CAsteroid> &asteroid_list,
         const float player_pos,
         const std::list<CShot> &shot_list,
         const std::list<CExplosion> &explosion_list,
         const int player_lives,
         const int space_station_health,
         const float weapons_array_cooldown);
  
  State(const int hash,
        const std::list<ActionPtr> actions);

  StateHash hash() const {return hash_;}
  void setHash(StateHash hash) {hash_ = hash;}
  const std::list<ActionPtr> & actions() const {return actions_;}
  ActionPtr action(ActionChoice choice) {return actions_map_.at(choice);}

  const std::list<CAsteroid> &asteroid_list() const {return asteroid_list_;}
  float player_pos() const {return player_pos_;}
  const std::list<CShot> &shot_list() const {return shot_list_;}
  const std::list<CExplosion> &explosion_list() const {return explosion_list_;}
  int player_lives() const {return player_lives_;}
  int space_station_health() const {return space_station_health_;}
  float weapons_array_cooldown() const {return weapons_array_cooldown_;}

  int visited() const {return visited_;}
  void visit() {visited_++;}

  bool operator==(const State& rhs) const {
    if(rhs.actions().size() != actions().size())
      return false;
    for(const auto& p : actions_map_){
      const auto& this_action = p.second;
      const auto& rhs_action = rhs.actions_map_.at(p.first);
      if(std::abs(this_action->reward() - rhs_action->reward()) > 0.0001)
        return false;
    }
    return true;
  }

private:
  StateHash hash_;
  std::map<ActionChoice, ActionPtr> actions_map_;
  std::list<ActionPtr> actions_;

  const std::list<CAsteroid> asteroid_list_;
  const float player_pos_;
  const std::list<CShot> shot_list_;
  const std::list<CExplosion> explosion_list_;
  const int player_lives_;
  const int space_station_health_;
  const float weapons_array_cooldown_;
  int visited_;
};

#endif // STATE_H
