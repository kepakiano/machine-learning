#ifndef DISTANCEMETRIC_H
#define DISTANCEMETRIC_H

#include <vector>

#include "censusdata.h"
class DistanceMetric
{
public:
  explicit DistanceMetric(const int size);

  double distanceBetweenClasses() const {return distance_between_classes_;}
  void distanceBetweenClasses(double d) {distance_between_classes_ = d;}

  double calculateDistance(const CensusData & census_data1, const CensusData & census_data2);

private:
  std::vector<double> weights;

  double distance_between_classes_;
};

#endif // DISTANCEMETRIC_H
