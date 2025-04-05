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

namespace maq {

/**
 * Fit a Multi-Armed Qini curve.
 *
 * The solution_path is a pair where the first entry is a set of vectors containing the path
 * {spend, gain, std.err} and the second pair the path of the corresponding allocations {unit
 * index, arm index}. The second entry in fit()’s return pair is a set of vectors with the
 * bootstrapped gain path. Since the spend path in general can be an irregular grid (each
 * unit/arm may have different cost), these paths are interpolated and indexed to the original
 * full-sample spend path. This entry is non-empty if options.paired_inference = true, and
 * enables paired-wise tests with other cost curves that are fit with the same bootstrap seed
 * (thus yielding valid standard errors that account for the correlation of two curves estimated
 * on the same bootstrap sample).
 *
 * The vectors {spend, gain} trace out the path of integer solutions to the underlying LP, and
 * essentially act as a sufficient statistic that gives the solution to any point on the cost
 * curves. For example, the LP solution for a budget  equal to some arbitrary point between
 * spend[j] and spend[j+1] is given by the linear interpolation between gain[j] and gain[j+1].
 * Likewise, the vectors {unit index, arm index} implicitly act as similar compressed statistics
 * that can yield the solution \pi matrix at any budget level. These steps are performed in the
 * wrapper code.
 *
 */
class Solver {
  public:
    Solver(
      std::vector<std::vector<Treatment>>& treatment_arrays,
      const SolverOptions& options
    ) : 
      treatment_arrays(treatment_arrays),
      options(options) {}

    solution_path fit() {
      convex_hull(treatment_arrays); // Prune in-place
      return compute_path(treatment_arrays, options.budget);
    }

    // TODO: Write a method to support predictions
    std::vector<std::vector<Treatment>> treatment_arrays;
    SolverOptions options;
};

} // namespace maq

#endif // MAQ_SOLVER_HPP
