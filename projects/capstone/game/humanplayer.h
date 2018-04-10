#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "player.hpp"

class HumanPlayer : public CPlayer
{
public:
    HumanPlayer();

    virtual ActionChoice getAction(const std::list<CAsteroid> &) override;
};

#endif // HUMANPLAYER_H
