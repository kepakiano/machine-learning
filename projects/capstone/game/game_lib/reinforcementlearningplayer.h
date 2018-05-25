#ifndef REINFORCEMENTLEARNINGPLAYER_H
#define REINFORCEMENTLEARNINGPLAYER_H

#include <unordered_map>

#include "action.h"
#include "player.hpp"
#include "state.h"
#include "states.h"

class ReinforcementLearningPlayer : public CPlayer
{
public:
    ReinforcementLearningPlayer(bool learning, double alpha,
                                double gamma, double epsilon,
                                size_t environment_number,
                                States& states);
    virtual ~ReinforcementLearningPlayer() override = default;

    void computeState(const std::list<CAsteroid> & asteroid_list,
                      const std::list<CExplosion> &explosion_list) override;
    virtual StatePtr getCurrentState() const override {return current_state;}
    ActionChoice chooseAction() const override;
    ActionPtr chooseAction(const StatePtr& state) const;
    void learn(const double reward,
               const StatePtr& new_state) override;

    virtual void setCurrentAction(ActionPtr action) override {
      current_action = action;
    }


private:
    double epsilon;
    double alpha;
    double gamma;
    
    bool learning;

    size_t environment_number;
    
    State buildState(const std::list<CAsteroid> &asteroid_list);
    std::list<ActionPtr> getBestActions(const StatePtr &state) const;

    StatePtr current_state;
    ActionPtr current_action;

    States& states;
};

#endif // REINFORCEMENTLEARNINGPLAYER_H
