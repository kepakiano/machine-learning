#ifndef REINFORCEMENTLEARNINGPLAYER_H
#define REINFORCEMENTLEARNINGPLAYER_H

#include "player.hpp"

class ReinforcementLearningPlayer : public CPlayer
{
public:
    ReinforcementLearningPlayer(bool learning);
    virtual ~ReinforcementLearningPlayer () = default;

    Action getAction(const std::list<CAsteroid> & asteroid_list) override;
    
    void reset() {
        epsilon *= 0.99;
    }

private:
    int getAsteroidState(std::list<CAsteroid> asteroid_list);
    
    double epsilon;
    double alpha;
    double gamma;
    
    bool learning;
};

#endif // REINFORCEMENTLEARNINGPLAYER_H
