#include "reinforcementlearningplayer.h"

#include <algorithm>
#include <iostream>

#include "utilities.hpp"

ReinforcementLearningPlayer::ReinforcementLearningPlayer(bool learning)
    : epsilon(1.0)
    , alpha(0.8)
    , gamma(0.8)
    , current_state(nullptr)
    , current_action(nullptr)
{

}

int ReinforcementLearningPlayer::getAsteroidState(std::list<CAsteroid> asteroid_list)
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
    int next_asteroid_position = 0;
    int second_next_asteroid_position = 0;

    for(const CAsteroid &asteroid : asteroid_list){

        Rectangle RectAsteroid = asteroid.getRect();

        bool asteroid_is_in_front_of_the_ship = RectAsteroid.x + 64 > m_fXPos && RectAsteroid.x < m_fXPos + 64;
        there_is_an_asteroid_in_front_of_the_ship |= asteroid_is_in_front_of_the_ship;
        if(!asteroid_is_in_front_of_the_ship){
            if(next_asteroid_position == 0){
                if(RectAsteroid.x < m_fXPos)
                    next_asteroid_position = 1;
                else
                    next_asteroid_position = 2;
                continue;
            }
            if(next_asteroid_position != 0 && second_next_asteroid_position == 0){
                if(RectAsteroid.x < m_fXPos)
                    second_next_asteroid_position = 1;
                else
                    second_next_asteroid_position = 2;
                continue;
            }
        }
    }
    int state_code = (there_is_an_asteroid_in_front_of_the_ship ? 1 : 0);
    state_code += next_asteroid_position * 10;
    state_code += second_next_asteroid_position * 100;

    return state_code;
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
    current_state = State::getState(asteroid_list,
                 m_fXPos,
                 m_ShotList,
                 m_Leben,
                 m_Lebensenergie_Raumstation,
                 m_fShotCooldownTimer
                 );
    
}
