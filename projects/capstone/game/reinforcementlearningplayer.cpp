#include "reinforcementlearningplayer.h"

#include <algorithm>
#include <iostream>

#include "utilities.hpp"

ReinforcementLearningPlayer::ReinforcementLearningPlayer(bool learning)
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

State ReinforcementLearningPlayer::buildState(const std::list<CAsteroid> &asteroid_list)
{
    return State::buildState(asteroid_list,
                 m_fXPos,
                 m_ShotList,
                 m_Leben,
                 m_Lebensenergie_Raumstation,
                 m_fShotCooldownTimer
                 );
  

}

/**
 * @brief ReinforcementLearningPlayer::getBestActions Given the hash of a State
 * this method returns all the actions with the best rewards.
 */
std::list<ActionPtr> ReinforcementLearningPlayer::getBestActions(const StateHash hash)
{
    std::list<ActionPtr> best_actions;
    for(const auto& action : states_[hash]->actions()){
        if(action->reward() > (*best_actions.begin())->reward()){
            best_actions.clear();
            best_actions.push_back(action);
        } else if (action->reward() < (*best_actions.begin())->reward()){

        } else {
            best_actions.push_back(action);
        }
    }
    return best_actions;
}

ActionChoice ReinforcementLearningPlayer::chooseAction(const StatePtr& state)
{
    /**

        """ The choose_action function is called when the agent is asked to choose
            which action to take, based on the 'state' the smartcab is in. """

        # Set the agent state and default action
        self.state = state
        self.next_waypoint = self.planner.next_waypoint()
        action = None

        ###########
        ## TO DO ##
        ###########
        # When not learning, choose a random action
        # When learning, choose a random action with 'epsilon' probability
        # Otherwise, choose an action with the highest Q-value for the current state
        # Be sure that when choosing an action with highest Q-value that you randomly select between actions that "tie".
        if not self.learning or random.random() < self.epsilon:
            action = random.choice(self.valid_actions)
        else:
            action = random.choice(self.get_best_actions(state))

        return action
    */

    if(learning && getRandomDoubleBetween(0.0, 1.0) < epsilon){
        const auto& valid_actions = state->actions();
        return (*randomChoice(valid_actions.begin(), valid_actions.end()))->choice();
    }
    else {
        const auto& best_actions = getBestActions(state->hash());
        return (*randomChoice(best_actions.begin(), best_actions.end()))->choice();
    }
}

void ReinforcementLearningPlayer::learn(const StatePtr& state,
                                        const ActionChoice &choice,
                                        const double reward)
{
    /*
        """ The learn function is called after the agent completes an action and
            receives a reward. This function does not consider future rewards
            when conducting learning. """

        ###########
        ## TO DO ##
        ###########
        # When learning, implement the value iteration update rule
        #   Use only the learning rate 'alpha' (do not use the discount factor 'gamma')
        Q_st_at = self.Q[state][action]
        r_t = reward

        if self.learning:
            self.Q[state][action] = Q_st_at + self.alpha * (r_t - Q_st_at)
*/
    ActionPtr action = state->action(choice);
    const double Q_st_at = action->reward();
    const double new_reward = Q_st_at + alpha * (reward - Q_st_at);

    action->setReward(new_reward);
}

ActionChoice ReinforcementLearningPlayer::getAction(const std::list<CAsteroid> &asteroid_list)
{
    /**
      The following variables comprise the state of the game:
      1. Positions of asteroids
      2. Player position
      3. Position of shots
      4. Health of the ship
      5. Health of the space station
      6. Cooldown time of the weapons array
      */

    // We need to bring the complexity of the game state down to something the agent can process.
    // Therefore, some simplicfications:
    // The asteroid position will be broken down into the following:

    // Is there an asteroid in front of the ship?
    // Is the closest asteroid which is not in front of the ship to the right/left/doesn't exist?
    // Is the second closest asteroid which is not in front of the ship to the right/left/doesn't exist?

    const int state_code = getAsteroidState(asteroid_list);

    if(state_code % 2 == 1 && isShootingPossible())
        return SHOOT;
    if(state_code % 100 / 10 % 3 == 2){
        return MOVE_RIGHT;
    }
    if(state_code % 100 / 10 % 3 == 1)
        return MOVE_LEFT;
    return PASS;



}
