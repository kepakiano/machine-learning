#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "player.hpp"

class HumanPlayer : public CPlayer
{
public:
    HumanPlayer();

    virtual void computeState(const std::list<CAsteroid> &,
                              const std::list<CExplosion> &) override {}
    virtual void learn(const double, const StatePtr &) override {}
    virtual StatePtr getCurrentState() const override { return nullptr;}
    virtual ActionChoice chooseAction() const override;
};

#endif // HUMANPLAYER_H
