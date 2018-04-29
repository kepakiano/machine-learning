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
std::list<ActionPtr> ReinforcementLearningPlayer::getBestActions(const StatePtr& state)
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

ActionChoice ReinforcementLearningPlayer::chooseAction()
{
    if(learning && getRandomDoubleBetween(0.0, 1.0) < epsilon){
        const auto& valid_actions = current_state->actions();
        current_action = (*randomChoice(valid_actions.begin(), valid_actions.end()));
    }
    else {
        const auto& best_actions = getBestActions(current_state);
        current_action = (*randomChoice(best_actions.begin(), best_actions.end()));
    }
    return current_action->choice();
}

void ReinforcementLearningPlayer::learn(const double reward,
                                        const StatePtr& new_state)
{
    const double next_Q = getBestActions(new_state).front()->reward();
    const double Q_st_at = current_action->reward();
    const double new_reward = Q_st_at + alpha * (reward + gamma * next_Q - Q_st_at);

    current_action->setReward(new_reward);
}

void ReinforcementLearningPlayer::computeState(const std::list<CAsteroid> &asteroid_list)
{
    current_state = States::getState(asteroid_list,
                 m_fXPos,
                 m_ShotList,
                 m_Leben,
                 m_Lebensenergie_Raumstation,
                 m_fShotCooldownTimer
                 );    
}
