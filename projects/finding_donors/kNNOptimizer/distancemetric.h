#ifndef DISTANCEMETRIC_H
#define DISTANCEMETRIC_H

#include <vector>

class DistanceMetric
{
public:
  DistanceMetric();

private:
  std::vector<double> weights;
};

#endif // DISTANCEMETRIC_H
