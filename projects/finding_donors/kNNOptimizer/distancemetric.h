#ifndef DISTANCEMETRIC_H
#define DISTANCEMETRIC_H

#include <vector>

#include "censusdata.h"
class DistanceMetric
{
public:
  explicit DistanceMetric(const int size);
  DistanceMetric(const DistanceMetric & rhs);

  double calculateDistance(const CensusData & census_data1, const CensusData & census_data2);

  void crossover(DistanceMetric & rhs);
  void mutate();

  double distanceBetweenClasses() const {return distance_between_classes_;}
  void distanceBetweenClasses(double d) {distance_between_classes_ = d;}

  std::vector<double> weights() const {return weights_;}


private:
  std::vector<double> weights_;
  double distance_between_classes_;

  void normalizeWeights();
};

#endif // DISTANCEMETRIC_H
