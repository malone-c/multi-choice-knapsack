#ifndef MAQ_SOLVEROPTIONS_HPP
#define MAQ_SOLVEROPTIONS_HPP

#include <thread>

namespace mckp {

struct SolverOptions {
  SolverOptions(
    double budget,
    uint32_t num_threads
  ):
      budget(budget)
      // paired_inference(paired_inference),
      // num_bootstrap(num_bootstrap),
      // random_seed(random_seed) 
    {
    if (num_threads == 0) {
      num_threads = std::thread::hardware_concurrency();
    }
    this->num_threads = num_threads;
  }

  double budget;
  uint32_t num_threads;
};

} // namespace mckp

#endif // MAQ_SOLVEROPTIONS_HPP
