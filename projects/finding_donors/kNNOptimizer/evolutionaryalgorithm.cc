#include "evolutionaryalgorithm.h"

#include <limits>

#include <math/math.h>

#include "censusdata.h"


constexpr int EvolutionaryAlgorithm::max_generations_;

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
  , generation_count_(0)
{

}

DistanceMetric EvolutionaryAlgorithm::determineBestDistanceMetric(const std::vector<CensusData> &census_data)
{
  std::vector<DistanceMetric> population = initPopulation();

  double old_distance_between_classes = std::numeric_limits<double>::lowest();
  double new_distance_between_classes = eval(population, census_data);

  std::cout << "Initial highest distance between classes: " << new_distance_between_classes << std::endl;

  do{
    population = select(population);
    crossover(population);
    mutate(population);
    old_distance_between_classes = new_distance_between_classes;
    new_distance_between_classes = eval(population, census_data);
    std::cout << "Highes distance between classes: " << new_distance_between_classes << std::endl;
    generation_count_++;
  } while(/*old_distance_between_classes < new_distance_between_classes
          &&*/ generation_count_ < max_generations_);


  return *std::max_element(population.begin(), population.end(), [](const DistanceMetric & d1, const DistanceMetric &d2) {
      return d1.distanceBetweenClasses() < d2.distanceBetweenClasses();
  });

}

std::vector<DistanceMetric> EvolutionaryAlgorithm::initPopulation()
{
  std::vector<DistanceMetric> population;
  for(int i = 0; i < population_size_; i++)
      population.push_back(DistanceMetric(CensusData::categories.size()));
  return population;
}

DistanceMetric EvolutionaryAlgorithm::tournamentSelection(const std::vector<DistanceMetric> & population)
{
   DistanceMetric best = population[Utilities::Math::getRandomNumberBetween(0, population_size_)];

    for (size_t i = 0; i < tournament_size_ - 1; i++) {
        const DistanceMetric & competition = population[Utilities::Math::getRandomNumberBetween(0, population_size_)];
        if (competition.distanceBetweenClasses() > best.distanceBetweenClasses())
            best = competition;
    }
    return best;
}

std::vector<DistanceMetric> EvolutionaryAlgorithm::select(const std::vector<DistanceMetric> &population)
{
    std::vector<DistanceMetric> new_population;
    for(int i = 0; i < population_size_; i++)
        new_population.push_back(tournamentSelection(population));

    return new_population;
}

void EvolutionaryAlgorithm::crossover(std::vector<DistanceMetric> &population)
{
    for(size_t i = 0; i < population_size_ * crossover_percentage_; i+=2)
        population.at(i).crossover(population.at(i+1));
}

void EvolutionaryAlgorithm::mutate(std::vector<DistanceMetric> &population)
{
    for(DistanceMetric & d : population)
        while (Utilities::Math::getRandomDoubleBetween(0, 1.0) < mutation_rate_)
            d.mutate();
}



double EvolutionaryAlgorithm::eval(std::vector<DistanceMetric> &population, const std::vector<CensusData> &census_data)
{
  for(DistanceMetric & d : population){
    d.distanceBetweenClasses(eval(d, census_data));
  }
  return std::max_element(population.begin(), population.end(), [](const DistanceMetric & d1, const DistanceMetric &d2) {
      return d1.distanceBetweenClasses() < d2.distanceBetweenClasses();
  })->distanceBetweenClasses();
}

double EvolutionaryAlgorithm::eval(DistanceMetric &distance_metric, const std::vector<CensusData> &census_data)
{
  std::vector<double> intra_class_distances;
  std::vector<double> inter_class_distances;
  for(size_t i = 0; i < census_data.size(); i++){
    for(size_t j = 0; j < i; j++){
      const CensusData & census_data1 = census_data[i];
      const CensusData & census_data2 = census_data[j];
      double distance = distance_metric.calculateDistance(census_data1, census_data2);
      if(census_data1.earnsMoreThan50k() == census_data2.earnsMoreThan50k())
        intra_class_distances.push_back(distance);
      else
        inter_class_distances.push_back(distance);
    }
  }

  return Utilities::Math::mean(inter_class_distances) - Utilities::Math::mean(intra_class_distances);
}
