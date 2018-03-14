#include "humanplayer.h"

#include <iostream>

HumanPlayer::HumanPlayer()
{

}

Action HumanPlayer::getAction()
{
    if(g_pFramework->KeyDown(SDLK_SPACE) == false)
            m_bShotLock = false;
    if (g_pFramework->KeyDown(SDLK_SPACE) && m_bShotLock == false && isShootingPossible())
        return Action::SHOOT;
    if(g_pFramework->KeyDown(SDLK_LEFT))
        return Action::MOVE_LEFT;
    if (g_pFramework->KeyDown(SDLK_RIGHT))
        return Action::MOVE_RIGHT;
    return Action::PASS;
}
