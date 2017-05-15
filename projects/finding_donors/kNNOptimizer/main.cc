#include <iostream>

#include "read_census.h"
#include "distancemetric.h"
#include "evolutionaryalgorithm.h"

/**
  * TODO: Die Kategorien occupation, education_level, workclass, marital-status, relationship und native-country
  * spannen jeweils einen Unterraum auf.
  * Das Abstandsmaß muss die Abhängigkeit dieser Variablen berücksichtigen, indem für diese Kategorien ein eigenes Abstandsmaß gelernt wird
 */
int main(int argc, char *argv[])
{
  std::vector<CensusData> census_data = read_census();
  for(auto & s : CensusData::categories)
      std::cout << s << std::endl;
  return 0;
  EvolutionaryAlgorithm evo(50, 4, 0.7, 0.5, true);
  DistanceMetric best_distance_metric = evo.determineBestDistanceMetric(census_data);
  for(auto d : best_distance_metric.weights())
      std::cout << d << std::endl;

  std::vector<std::pair<double, std::string>> pairs;
  for(size_t i = 0; i < best_distance_metric.weights().size(); i++){
      pairs.push_back({best_distance_metric.weights()[i], CensusData::categories[i]});
  }
  std::sort(pairs.begin(), pairs.end(), [](const auto & p1, const auto & p2){return p1.first < p2.first;});
  for(const auto & p : pairs){
      std::cout << p.second << ": " << p.first << std::endl;
  }
  return 0;
}
