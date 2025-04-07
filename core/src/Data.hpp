#ifndef MAQ_DATA_HPP
#define MAQ_DATA_HPP

#include <cstddef>
#include <vector>
#include <string>
#include <iostream>

namespace mckp {

struct prediction {
  std::string product_id;
  double cost;
  double reward;
};

/**
 * Read-only data wrapper.
 *
 * Costs should be > 0
 * Weights should be > 0 and sum to 1
 * Clusters, if present, should be labeled as consecutive integers 0, ..., num_clusters
 *
 */

struct Treatment {
  unsigned int id;
  double reward;
  double cost;
};

// TODO: Optimise data by storing rewards and costs in std::vector<std::unordered_map<int double[2]>>
// and then looking them up by treatment ID, which is stored in std::vector<std::vector<int>> treatment_id_arrays 
// This should be more memory efficient but pruning will be more complex because we need to update the unordered maps.
// Trade-off is between memory and code complexity

std::vector<std::vector<Treatment>> process_data(
  std::vector<std::vector<unsigned int>>& treatment_id_arrays,
  std::vector<std::vector<double>>& reward_arrays,
  std::vector<std::vector<double>>& cost_arrays
) {
  Treatment treatment;
  size_t num_units = treatment_id_arrays.size();
  std::vector<std::vector<Treatment>> treatment_arrays;
  treatment_arrays.resize(num_units);
  for (size_t i = 0; i < num_units; ++i) {
    size_t num_treatments = treatment_id_arrays[i].size();

    for (size_t j = 0; j < num_treatments; ++j) {
  
      treatment = {
        treatment_id_arrays[i][j],
        reward_arrays[i][j],
        cost_arrays[i][j]
      };
      
      treatment_arrays[i].push_back(treatment);
    };
  }
  return treatment_arrays;
}

std::vector<std::vector<Treatment>> process_data_raw(
    unsigned int** treatment_id_ptrs, 
    size_t* treatment_id_lengths,
    double** reward_ptrs, 
    double** cost_ptrs,
    size_t num_units
) {
    std::vector<std::vector<Treatment>> treatment_arrays;
    treatment_arrays.resize(num_units);
    
    for (size_t i = 0; i < num_units; ++i) {
        size_t num_treatments = treatment_id_lengths[i];
        treatment_arrays[i].reserve(num_treatments);
        
        for (size_t j = 0; j < num_treatments; ++j) {
            Treatment treatment = {
                treatment_id_ptrs[i][j],
                reward_ptrs[i][j],
                cost_ptrs[i][j]
            };
            treatment_arrays[i].push_back(treatment);
        }
    }
    
    return treatment_arrays;
}

// TODO: This is not used ATM. Integrate or remove it.
class Data {
public:
  Data(
    std::vector<std::vector<unsigned int>>& treatment_id_arrays,
    std::vector<std::vector<double>>& reward_arrays,
    std::vector<std::vector<double>>& cost_arrays
  ) {
    size_t num_units = treatment_id_arrays.size();
    std::vector<std::vector<Treatment>> treatment_arrays = process_data(
      treatment_id_arrays,
      reward_arrays,
      cost_arrays
    );
  }

  int get_tie_breaker(size_t row) const { return row; }

  size_t get_num_units() const { return treatment_arrays.size(); }

private:
  std::vector<std::vector<Treatment>> treatment_arrays;
};

} // namespace mckp

#endif // MAQ_DATA_HPP
