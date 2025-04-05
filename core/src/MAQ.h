#ifndef MAQ_H
#define MAQ_H

// Multi-Armed Qini (maq).
// https://github.com/grf-labs/maq
// Distributed under the MIT License.

#include "Data.hpp"
#include "SolverOptions.hpp"
#include "Solver.hpp"

namespace maq {

solution_path run(
    std::vector<std::vector<unsigned int>>& treatment_id_arrays,
    std::vector<std::vector<double>>& reward_arrays,
    std::vector<std::vector<double>>& cost_arrays,
    double budget,
    unsigned int num_threads
    ) {
  SolverOptions options(budget, num_threads);

  // Data data(treatment_id_arrays, reward_arrays, cost_arrays);
  std::vector<std::vector<Treatment>> treatment_arrays = process_data(
    treatment_id_arrays,
    reward_arrays,
    cost_arrays
  );

  Solver maq(treatment_arrays, options);

  return maq.fit();
}

} // namespace maq

#endif // MAQ_H
