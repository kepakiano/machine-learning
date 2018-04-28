#include "state.h"

#include <algorithm>


State::State(const size_t hash,
             const std::list<ActionPtr>& valid_actions)
    : hash_(hash)
    , actions_(valid_actions)
    , visited_(0)
{
    for(const auto& action : valid_actions)
        actions_map_.insert({action->choice(), action});
}
