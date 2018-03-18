#ifndef REINFORCEMENTLEARNINGPLAYER_H
#define REINFORCEMENTLEARNINGPLAYER_H

#include "player.hpp"

class ReinforcementLearningPlayer : public CPlayer
{
public:
    ReinforcementLearningPlayer();

    Action getAction(const std::list<CAsteroid> & asteroid_list) override;
};

#endif // REINFORCEMENTLEARNINGPLAYER_H
