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

  int visited() const {return visited_;}
  void visit() {visited_++;}

private:
  StateHash hash_;
  std::map<ActionChoice, ActionPtr> actions_map_;
  std::list<ActionPtr> actions_;
  int visited_;
};

#endif // STATE_H
