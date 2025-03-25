#ifndef MAQ_SOLVEROPTIONS_HPP
#define MAQ_SOLVEROPTIONS_HPP

#include <thread>

namespace maq {

struct SolverOptions {
  SolverOptions(
    double budget,
    bool paired_inference,
    unsigned int num_bootstrap,
    unsigned int num_threads,
    unsigned int random_seed
  ):
      budget(budget),
      paired_inference(paired_inference),
      num_bootstrap(num_bootstrap),
      random_seed(random_seed) {
    if (num_threads == 0) {
      num_threads = std::thread::hardware_concurrency();
    }
    this->num_threads = num_threads;
  }

  double budget;
  bool paired_inference;
  unsigned int num_bootstrap;
  unsigned int num_threads;
  unsigned int random_seed;
};

} // namespace maq

#endif // MAQ_SOLVEROPTIONS_HPP
