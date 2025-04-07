#ifndef MAQ_H
#define MAQ_H

// Multi-Armed Qini (maq).
// https://github.com/grf-labs/maq
// Distributed under the MIT License.

#include "Data.hpp"
#include "SolverOptions.hpp"
#include "Solver.hpp"
#include <iostream>

namespace mckp {

solution_path run(
  std::vector<std::vector<unsigned int>>& treatment_id_arrays,
  std::vector<std::vector<double>>& reward_arrays,
  std::vector<std::vector<double>>& cost_arrays,
  double budget,
  unsigned int num_threads
) {
  SolverOptions options(budget, num_threads);

  std::vector<std::vector<Treatment>> treatment_arrays = process_data(
    treatment_id_arrays,
    reward_arrays,
    cost_arrays
  );
  std::cout << "Data successfully pre-processed" << std::endl;

  std::cout << "Initializing solver" << std::endl;
  Solver maq(treatment_arrays, options);
  std::cout << "Solver initialized" << std::endl;

  std::cout << "Fitting solver" << std::endl;
  return maq.fit();
}

solution_path run_raw(
  unsigned int** treatment_id_ptrs, 
  size_t* treatment_id_lengths,
  double** reward_ptrs, 
  size_t* reward_lengths,
  double** cost_ptrs, 
  size_t* cost_lengths,
  size_t num_units,
  double budget,
  unsigned int num_threads
) {
  SolverOptions options(budget, num_threads);

  std::vector<std::vector<Treatment>> treatment_arrays = process_data_raw(
    treatment_id_ptrs,
    treatment_id_lengths,
    reward_ptrs,
    cost_ptrs,
    num_units
  );

  Solver maq(treatment_arrays, options);

  return maq.fit();
}

} // namespace mckp

#endif // MAQ_H
