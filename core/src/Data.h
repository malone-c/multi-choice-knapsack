#ifndef MAQ_DATA_H
#define MAQ_DATA_H

#include <cstddef>
#include <vector>

namespace maq {

/**
 * Read-only data wrapper for column or row major storage.
 *
 * Costs should be > 0
 * Weights should be > 0 and sum to 1
 * Clusters, if present, should be labeled as consecutive integers 0, ..., num_clusters
 *
 */
class Data {
public:
  inline Data(const double* data_reward,
              const double* data_reward_scores,
              const double* data_cost,
              const double* data_weight,
              const int* data_tie_breaker,
              const int* clusters,
              size_t num_rows,
              size_t num_cols,
              bool col_major);

  inline double get_reward(size_t row, size_t col) const;

  inline double get_reward_scores(size_t row, size_t col) const;

  inline double get_cost(size_t row, size_t col) const;

  inline int get_tie_breaker(size_t row) const;

  size_t num_rows;
  size_t num_cols;
  std::vector<std::vector<size_t>> samples_by_cluster;

private:
  inline size_t index(size_t row, size_t col) const;

  inline double get_weight(size_t row) const;

  const double* data_reward;
  const double* data_reward_scores;
  const double* data_cost;
  const double* data_weight;
  const int* data_tie_breaker;
  bool has_weight;
  bool has_tie_breaker;
  bool col_major;
};

inline Data::Data(const double* data_reward,
                  const double* data_reward_scores,
                  const double* data_cost,
                  const double* data_weight,
                  const int* data_tie_breaker,
                  const int* clusters,
                  size_t num_rows,
                  size_t num_cols,
                  bool col_major) :
                  num_rows(num_rows),
                  num_cols(num_cols),
                  data_reward(data_reward),
                  data_reward_scores(data_reward_scores),
                  data_cost(data_cost),
                  data_weight(data_weight),
                  data_tie_breaker(data_tie_breaker),
                  col_major(col_major) {

  this->has_weight = data_weight == nullptr ? false : true;
  this->has_tie_breaker = data_tie_breaker == nullptr ? false : true;

  // If clusters are present, then fill samples_by_cluster with samples belonging to each cluster.
  if (clusters != nullptr) {
    for (size_t sample = 0; sample < num_rows; sample++) {
      size_t cluster_id = clusters[sample];
      if (cluster_id + 1 > samples_by_cluster.size()) {
        samples_by_cluster.resize(cluster_id + 1);
      }
      samples_by_cluster[cluster_id].push_back(sample);
    }
  }
}

inline double Data::get_reward(size_t row, size_t col) const {
  return data_reward[index(row, col)] * get_weight(row);
}

inline double Data::get_reward_scores(size_t row, size_t col) const {
  return data_reward_scores[index(row, col)] * get_weight(row);
}

inline double Data::get_cost(size_t row, size_t col) const {
  return data_cost[index(row, col)] * get_weight(row);
}

inline int Data::get_tie_breaker(size_t row) const {
  if (!has_tie_breaker) {
    return row;
  } else {
    return data_tie_breaker[row];
  }
}

// these extra layers of runtime indirection in get methods add neglishable
// overhead wrt speed and don't really warrant the complexity of doing
// this with templates.
inline size_t Data::index(size_t row, size_t col) const {
  if (col_major) {
    return col * num_rows + row;
  } else {
    return row * num_cols + col;
  }
}

inline double Data::get_weight(size_t row) const {
  if (!has_weight) {
    return 1.0 / num_rows;
  } else {
    return data_weight[row];
  }
}

} // namespace maq

#endif // MAQ_DATA_H
