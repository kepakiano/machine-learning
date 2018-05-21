#include "reinforcementlearningplayer.h"

#include <algorithm>
#include <iostream>

#include "states.h"
#include "utilities.hpp"

ReinforcementLearningPlayer::ReinforcementLearningPlayer(bool learning,
                                                         double alpha,
                                                         double gamma,
                                                         double epsilon,
                                                         size_t environment_number)
    : epsilon(epsilon)
    , alpha(alpha)
    , gamma(gamma)
    , learning(learning)
    , environment_number(environment_number)
    , current_state(nullptr)
    , current_action(nullptr)
{

}

/**
 * @brief ReinforcementLearningPlayer::getBestActions Given the hash of a State
 * this method returns all the actions with the best rewards.
 */
std::list<ActionPtr> ReinforcementLearningPlayer::getBestActions(const StatePtr& state) const
{
    std::list<ActionPtr> best_actions;
    for(const auto& action : state->actions()){
        if(best_actions.empty()){
            best_actions.push_back(action);
        }
        else if(action->reward() > (*best_actions.begin())->reward()){
            best_actions.clear();
            best_actions.push_back(action);
        } else if (action->reward() < (*best_actions.begin())->reward()){

        } else {
            best_actions.push_back(action);
        }
    }
    return best_actions;
}

ActionPtr ReinforcementLearningPlayer::chooseAction(const StatePtr &state) const
{
  ActionPtr action = nullptr;
  if(learning && getRandomDoubleBetween(0.0, 1.0) < epsilon){
      const auto& valid_actions = state->actions();
      action = (*randomChoice(valid_actions.begin(), valid_actions.end()));
  }
  else {
      const auto& best_actions = getBestActions(state);
      action = (*randomChoice(best_actions.begin(), best_actions.end()));
  }
  return action;
}

ActionChoice ReinforcementLearningPlayer::chooseAction() const
{
  return chooseAction(current_state)->choice();
}

void ReinforcementLearningPlayer::learn(const double reward,
                                        const StatePtr& new_state)
{
  if(!learning)
    return;

    const double next_Q = getBestActions(new_state).front()->reward();
//    const double next_Q = chooseAction(new_state)->reward();
    const double Q_st_at = current_action->reward();
    const double new_reward = Q_st_at + alpha * (reward + gamma * next_Q - Q_st_at);

    current_action->setReward(new_reward);
}

void ReinforcementLearningPlayer::computeState(const std::list<CAsteroid> &asteroid_list,
                                               const std::list<CExplosion> &explosion_list)
{
    current_state = States::getState(asteroid_list,
                 m_fXPos,
                 m_ShotList,
                                     explosion_list,
                 m_Leben,
                 m_Lebensenergie_Raumstation,
                 m_fShotCooldownTimer,
                 environment_number
                                     );
}

