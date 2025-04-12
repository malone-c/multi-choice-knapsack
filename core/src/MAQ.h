#ifndef MAQ_H
#define MAQ_H

// Fork of https://github.com/grf-labs/maq
// Distributed under the MIT License.

#include "Data.hpp"
#include "SolverOptions.hpp"
#include "Solver.hpp"
#include <iostream>

namespace mckp {

solution_path run(
  std::vector<std::vector<uint32_t>>& treatment_id_arrays,
  std::vector<std::vector<double>>& reward_arrays,
  std::vector<std::vector<double>>& cost_arrays,
  double budget,
  uint32_t num_threads
) {
  SolverOptions options(budget, num_threads);

  std::vector<std::vector<TreatmentView>> treatment_arrays = process_data(
    treatment_id_arrays,
    reward_arrays,
    cost_arrays
  );
  std::cout << "Data successfully pre-processed" << std::endl;

  std::cout << "Initializing solver" << std::endl;
  Solver solver(treatment_arrays, options);
  std::cout << "Solver initialized" << std::endl;

  std::cout << "Fitting solver" << std::endl;
  return solver.fit();
}

} // namespace mckp

#endif // MAQ_H
