#ifndef RANDOMPLAYER_H
#define RANDOMPLAYER_H

#include "player.hpp"

class RandomPlayer : public CPlayer
{
public:
    RandomPlayer();

    ActionChoice chooseAction() override;

private:
    ActionChoice last_action;

    int frames_left_in_current_action;
};

#endif // RANDOMPLAYER_H
