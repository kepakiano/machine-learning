#include "distancemetric.h"

#include <math/math.h>

DistanceMetric::DistanceMetric(const int size)
    : weights_(size)
{
    for(size_t i = 0; i < size; i++){
        weights_[i] = Utilities::Math::getRandomDoubleBetween(0.0, 1.0);
    }
    normalizeWeights();

}

DistanceMetric::DistanceMetric(const DistanceMetric &rhs)
    : weights_(rhs.weights_)
    , distance_between_classes_(rhs.distance_between_classes_)
{

}

double DistanceMetric::calculateDistance(const CensusData &census_data1, const CensusData &census_data2)
{
    if(census_data1.features().size() != census_data2.features().size())
        throw std::logic_error("Census data feature vectors don't have the same size");
    if(weights_.size() != census_data2.features().size())
        throw std::logic_error("Weights vector size doesn't match feature vector size");

    double distance = 0.0;
    for(size_t i = 0; i < weights_.size(); i++)
        distance += std::pow(weights_[i] * (census_data1.features().at(i) - census_data2.features().at(i)), 2.0);
    return distance;
}

void DistanceMetric::normalizeWeights()
{
    weights_ = Utilities::Math::normalized(weights_);
}

void DistanceMetric::crossover(DistanceMetric &rhs)
{
    int crossover_point = Utilities::Math::getRandomNumberBetween(0, weights_.size());
    for(int i = crossover_point; i < weights_.size(); i++){
        double temp = weights_[i];
        weights_[i] = rhs.weights_[i];
        rhs.weights_[i] = temp;
    }

    normalizeWeights();
    rhs.normalizeWeights();
}

void DistanceMetric::mutate()
{
    weights_[Utilities::Math::getRandomNumberBetween(0, weights_.size())] = Utilities::Math::getRandomDoubleBetween(0, 1.0);
    normalizeWeights();
}
