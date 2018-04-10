#ifndef REINFORCEMENTLEARNINGPLAYER_H
#define REINFORCEMENTLEARNINGPLAYER_H

#include <unordered_map>

#include "action.h"
#include "player.hpp"
#include "state.h"

class ReinforcementLearningPlayer : public CPlayer
{
public:
    ReinforcementLearningPlayer(bool learning);
    virtual ~ReinforcementLearningPlayer () = default;

    ActionChoice getAction(const std::list<CAsteroid> & asteroid_list) override;
    
    void reset() {
        epsilon *= 0.99;
    }

private:
    int getAsteroidState(std::list<CAsteroid> asteroid_list);
    
    double epsilon;
    double alpha;
    double gamma;
    
    bool learning;

//    std::unordered_map<StateHash, std::list<ActionPtr>> states_to_actions_;
    std::unordered_map<StateHash, StatePtr> states_;
    
    State buildState(const std::list<CAsteroid> &asteroid_list);
    std::list<ActionPtr> getBestActions(const StateHash hash);
    ActionChoice chooseAction(const StatePtr &state);
    void learn(const StatePtr& state,
               const ActionChoice& action,
               const double reward);
    
};

#endif // REINFORCEMENTLEARNINGPLAYER_H
