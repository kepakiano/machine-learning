#include "humanplayer.h"

#include <iostream>

HumanPlayer::HumanPlayer()
{

}

ActionChoice HumanPlayer::getAction(const std::list<CAsteroid> &)
{
    if(g_pFramework->KeyDown(SDLK_SPACE) == false)
            m_bShotLock = false;
    if (g_pFramework->KeyDown(SDLK_SPACE) && m_bShotLock == false && isShootingPossible())
        return ActionChoice::SHOOT;
    if(g_pFramework->KeyDown(SDLK_LEFT))
        return ActionChoice::MOVE_LEFT;
    if (g_pFramework->KeyDown(SDLK_RIGHT))
        return ActionChoice::MOVE_RIGHT;
    return ActionChoice::PASS;
}
