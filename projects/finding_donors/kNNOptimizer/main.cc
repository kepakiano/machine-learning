#include <iostream>

#include "read_census.h"
#include "distancemetric.h"
#include "evolutionaryalgorithm.h"

int main(int argc, char *argv[])
{
  std::vector<CensusData> census_data = read_census();
  EvolutionaryAlgorithm evo(50, 5, 0.3, 0.3, true);
  DistanceMetric best_distance_metric = evo.determineBestDistanceMetric(census_data);
  return 0;
}
