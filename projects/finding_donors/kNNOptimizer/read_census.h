#ifndef READ_CENSUS_H
#define READ_CENSUS_H

#include <vector>
#include <fstream>
#include <algorithm>

#include <IO/io.h>
#include "censusdata.h"

std::vector<CensusData> read_census()
{
  std::vector<CensusData> census_data;
  const std::string census_features_filename = "../census_train_features.csv";
  const std::string census_labels_filename = "../census_train_labels.csv";
  std::ifstream census_features_file(census_features_filename);
  std::ifstream census_labels_file(census_labels_filename);
  if(!census_features_file){
    std::cerr << "Error opening file " << census_features_filename << std::endl;
    return census_data;
  }
  if(!census_labels_file){
    std::cerr << "Error opening file " << census_labels_filename << std::endl;
    return census_data;
  }

  std::string next_line_features;
  std::string next_line_labels;
  bool first_line = true;
  size_t i = 0;
  while(std::getline(census_features_file, next_line_features)){
    std::vector<std::string> tokenized_features_line = Utilities::IO::splitString(next_line_features, ',');

    const std::string index_features = tokenized_features_line.at(0);
    // Remove the column containing the index
    tokenized_features_line.erase(tokenized_features_line.begin(), tokenized_features_line.begin()+1);
    if(first_line){
      CensusData::categories = tokenized_features_line;
      first_line = false;
    }
    else{
      if(!std::getline(census_labels_file, next_line_labels)){
        std::cerr << "Labels file shorter than features file. Aborting." << std::endl;
        return census_data;
      }
      std::vector<std::string> tokenized_labels_line = Utilities::IO::splitString(next_line_labels, ',');
      const std::string index_labels = tokenized_labels_line.at(0);
      if(index_features != index_labels){
        std::cerr << "Index of feature file (" << index_features << ") and labels file (" << index_labels << ") mismatch. Aborting.";
        return census_data;
      }

      std::vector<double> features(tokenized_features_line.size());
      std::transform(tokenized_features_line.begin(), tokenized_features_line.end(), features.begin(), [](const std::string& val)
      {
          return std::stod(val);
      });
      bool earns_more_than_50k = tokenized_labels_line.at(1) == "1";
      census_data.push_back(CensusData(features, earns_more_than_50k));
    }
    if(++i == 100)
        break;

  }
  std::cout << census_data.size() << std::endl;
  return census_data;
}

#endif // READ_CENSUS_H
