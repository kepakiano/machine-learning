#include "randomplayer.h"

#include "utilities.hpp"

RandomPlayer::RandomPlayer()
    : last_action(PASS), frames_left_in_current_action(0)
{

}

ActionChoice RandomPlayer::chooseAction()
{
    if(frames_left_in_current_action == 0){
        if(getRandomNumberBetween(0, 2) == 0)
            last_action = MOVE_LEFT;
        else
            last_action = MOVE_RIGHT;
        frames_left_in_current_action = getRandomNumberBetween(60, 300);
    }
    if(getRandomNumberBetween(0, 20) == 0 && isShootingPossible())
        return SHOOT;

    frames_left_in_current_action--;
    return last_action;
}
