#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <cstddef>

struct Environment {
    size_t environment_id;
    double alpha;
    double gamma;

    Environment(size_t s, double a, double g)
        : environment_id(s), alpha(a), gamma(g)
    {}
};
#endif // ENVIRONMENT_H
