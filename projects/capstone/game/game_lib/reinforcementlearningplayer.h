#ifndef REINFORCEMENTLEARNINGPLAYER_H
#define REINFORCEMENTLEARNINGPLAYER_H

#include <unordered_map>

#include "action.h"
#include "player.hpp"
#include "state.h"

class ReinforcementLearningPlayer : public CPlayer
{
public:
    ReinforcementLearningPlayer(bool learning, double alpha, double gamma, double epsilon);
    virtual ~ReinforcementLearningPlayer() override = default;

    void computeState(const std::list<CAsteroid> & asteroid_list) override;
    virtual StatePtr getCurrentState() const override {return current_state;}
    ActionChoice chooseAction() override;
    void learn(const double reward,
               const StatePtr& new_state) override;
    
    void reset() {
//        epsilon *= 0.99;
    }

private:
    double epsilon;
    double alpha;
    double gamma;
    
    bool learning;
    
    State buildState(const std::list<CAsteroid> &asteroid_list);
    std::list<ActionPtr> getBestActions(const StatePtr &state);

    StatePtr current_state;
    ActionPtr current_action;
    
};

#endif // REINFORCEMENTLEARNINGPLAYER_H
