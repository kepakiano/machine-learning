#include "evolutionaryalgorithm.h"

EvolutionaryAlgorithm::EvolutionaryAlgorithm(
    int population_size,
    int tournament_size,
    double crossover_percentage,
    double mutation_rage,
    bool elite_selection
    )
  : population_size_(population_size)
  , tournament_size_(tournament_size)
  , crossover_percentage_(crossover_percentage)
  , mutation_rate_(mutation_rage)
  , elite_selection_(elite_selection)
{

}

DistanceMetric EvolutionaryAlgorithm::determineBestDistanceMetric(const std::vector<CensusData> &census_data)
{

}
