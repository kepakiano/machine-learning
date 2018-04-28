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

inline double getRandomDoubleBetween(double min, double max)
{
  long int seed = std::chrono::system_clock::now().time_since_epoch().count();
  double lower_bound = min;
  double upper_bound = max;
  std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
  std::default_random_engine re(seed);
  return unif(re);
}

template<typename T>
auto randomChoice(T begin, T end){
    std::advance(begin, getRandomNumberBetween(0, std::distance(begin, end)));
    return begin;
}

#endif // UTILITIES_HPP
