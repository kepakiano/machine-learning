#ifndef EVOLUTIONARYALGORITHM_H
#define EVOLUTIONARYALGORITHM_H

#include <vector>

#include "distancemetric.h"
#include "censusdata.h"

class EvolutionaryAlgorithm
{
public:
  EvolutionaryAlgorithm(int population_size, int tournament_size, double crossover_percentage, double mutation_rage, bool elite_selection);

public:
  DistanceMetric determineBestDistanceMetric(const std::vector<CensusData> & census_data);

private:
  std::vector<DistanceMetric> initPopulation();
  DistanceMetric tournamentSelection(const std::vector<DistanceMetric> & population);
  std::vector<DistanceMetric> select(const std::vector<DistanceMetric> & population);
  void crossover(std::vector<DistanceMetric> &population);
  void mutate(std::vector<DistanceMetric> &population);
  double eval(std::vector<DistanceMetric> &population, const std::vector<CensusData> & census_data);
  double eval(DistanceMetric &distance_metric, const std::vector<CensusData> & census_data);

  const int population_size_;
  const int tournament_size_;
  const double crossover_percentage_;
  const double mutation_rate_;
  const bool elite_selection_;

  int generation_count_;
  static constexpr int max_generations_ = 100;
};

#endif // EVOLUTIONARYALGORITHM_H
