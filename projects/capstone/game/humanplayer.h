#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "player.hpp"

class HumanPlayer : public CPlayer
{
public:
    HumanPlayer();

protected:
    Action getAction() override;
};

#endif // HUMANPLAYER_H
