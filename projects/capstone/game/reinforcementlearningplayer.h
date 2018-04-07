#ifndef REINFORCEMENTLEARNINGPLAYER_H
#define REINFORCEMENTLEARNINGPLAYER_H

#include "player.hpp"
#include "state.h"

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
    
    State buildState(const std::list<CAsteroid> &asteroid_list);
    Action getBestAction();
    void generateState();
    Action chooseAction(const State& state);
    void learn(const State& state,
               const Action& action,
               const double reward);
    
};

#endif // REINFORCEMENTLEARNINGPLAYER_H
