#include "humanplayer.h"

#include <iostream>

HumanPlayer::HumanPlayer()
{

}

ActionChoice HumanPlayer::chooseAction() const
{
    if (g_pFramework->KeyDown(SDLK_SPACE) && isShootingPossible())
        return ActionChoice::SHOOT;
    if(g_pFramework->KeyDown(SDLK_LEFT))
        return ActionChoice::MOVE_LEFT;
    if (g_pFramework->KeyDown(SDLK_RIGHT))
        return ActionChoice::MOVE_RIGHT;
    return ActionChoice::PASS;
}
