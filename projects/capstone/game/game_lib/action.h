#ifndef ACTION_H
#define ACTION_H

#include <memory>

#include "actionchoice.h"

class Action;
using ActionPtr = std::shared_ptr<Action>;

class Action
{
public:
  Action(const ActionChoice choice,
         const double reward)
    : action_choice_(choice)
    , reward_(reward) {}
    Action(const ActionChoice choice)
      : Action(choice, 0.0) {}

    void setReward(const double reward){reward_ = reward;}
    double reward() const {return reward_;}
    ActionChoice choice() const {return action_choice_;}

private:
    const ActionChoice action_choice_;
    double reward_;
};

#endif // ACTION_H
