#ifndef MAQ_SOLVER_HPP
#define MAQ_SOLVER_HPP

// Multi-Armed Qini (maq).
// https://github.com/grf-labs/maq
// Distributed under the MIT License.

#include <cmath>
#include <future>
#include <random>
#include <vector>

#include "convex_hull.hpp"
#include "compute_path.hpp"
#include "SolverOptions.hpp"

namespace mckp {

class Solver {
  public:
    Solver(
      std::vector<std::vector<TreatmentView>>& treatment_arrays,
      const SolverOptions& options
    ) : 
      treatment_arrays(treatment_arrays),
      options(options) {}

    solution_path fit() {
      convex_hull(treatment_arrays); // Prune in-place
      return compute_path(treatment_arrays, options.budget);
    }

    // TODO: Write a method to support predictions
    std::vector<std::vector<TreatmentView>> treatment_arrays;
    SolverOptions options;
};

} // namespace mckp

#endif // MAQ_SOLVER_HPP
