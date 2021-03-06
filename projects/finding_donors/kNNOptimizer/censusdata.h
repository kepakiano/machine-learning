#ifndef CENSUSDATA_H
#define CENSUSDATA_H

#include <vector>
#include <string>

class CensusData
{
public:
  CensusData(const std::vector<double> & features, bool earns_more_than_50k);

  static std::vector<std::string> categories;
  bool earnsMoreThan50k() const {return earns_more_than_50k_;}
  std::vector<double> features() const {return features_;}

private:
  std::vector<double> features_;
  bool earns_more_than_50k_;
};

#endif // CENSUSDATA_H
