#include "distancemetric.h"

#include <math/math.h>

DistanceMetric::DistanceMetric(const int size)
  : weights(size)
{
  for(size_t i = 0; i < size; i++){
    weights[i] = Utilities::Math::getRandomDoubleBetween(0.0, 1.0);
  }

  weights = Utilities::Math::normalized(weights);

}

double DistanceMetric::calculateDistance(const CensusData &census_data1, const CensusData &census_data2)
{
  if(census_data1.features().size() != census_data2.features().size())
    throw std::logic_error("Census data feature vectors don't have the same size");
  if(weights.size() != census_data2.features().size())
    throw std::logic_error("Weights vector size doesn't match feature vector size");

  double distance = 0.0;
  for(size_t i = 0; i < weights.size(); i++)
    distance += weights[i] * (census_data1.features().at(i) - census_data2.features().at(i));
  return distance;
}
