#include "reinforcementlearningplayer.h"

#include <algorithm>
#include <iostream>

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
  
  
}

Action ReinforcementLearningPlayer::getAction(const std::list<CAsteroid> &asteroid_list)
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
