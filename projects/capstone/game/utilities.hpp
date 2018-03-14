#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <chrono>
#include <random>

inline size_t getRandomNumberBetween(size_t min, size_t max)
{
    static long int seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::minstd_rand RNG(seed);

    if(max == min)
        return min;
    return min + RNG() % (max - min);
}

#endif // UTILITIES_HPP
