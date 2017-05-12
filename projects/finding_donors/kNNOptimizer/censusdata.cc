#include "censusdata.h"

std::vector<std::string> CensusData::categories;

CensusData::CensusData(const std::vector<double> &features, bool earns_more_than_50k)
  : features_(features)
  , earns_more_than_50k_(earns_more_than_50k)
{

}
