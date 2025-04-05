#ifndef MAQ_CONVEX_HULL_HPP
#define MAQ_CONVEX_HULL_HPP

#include <algorithm>
#include <cstddef>
#include <numeric>
#include <vector>
#include <deque>

#include "Data.hpp"

/*
Find the upper left convex hull on the (cost, reward) plane for each unit. This takes
O(num_rows * num_arms * log(num_arms)) time using the Graham scan for finding a convex hull,
with the the "angle calculation" replaced by a variant of the "LP dominance" criteria
(11.6) and (11.7) in Kellerer et al. (2004, Chapter 11).

Consider 3 points j, k, l ordered according to cost j < k < l.
This algorithm maintains a stack with the first two points on top, then iteratively
proceeds by checking if point k should be replaced by or augmented by point l.
*/

namespace maq {

inline bool candidate_dominates_last_selection(
  const std::vector<Treatment>& selections,
  Treatment candidate
) {
  if (selections.size() < 1) {
    return false;
  }
  Treatment arm_j = {0, 0, 0}; // dummy point
  if (selections.size() >= 2) {
    arm_j = selections[selections.size() - 2]; // next to top
  }
  Treatment arm_k = selections[selections.size() - 1]; // top
  if (arm_k.reward <= 0) {
    return true;
  }

  // C++: a/0 = Inf if a > 0, a/0 = -Inf if a <0, and 0/0 = NaN (all logical operators on NaN evaluate to false)
  // return (reward_l - reward_k) / (cost_l - cost_k) > (reward_k - reward_j) / (cost_k - cost_j);
  return (candidate.reward - arm_k.reward) * (arm_k.cost - arm_j.cost) > (arm_k.reward - arm_j.reward) * (candidate.cost - arm_k.cost);
}

void convex_hull(std::vector<std::vector<Treatment>> treatment_arrays) {
  for (size_t unit = 0; unit < treatment_arrays.size(); unit++) {

    std::deque<Treatment> candidates(treatment_arrays[unit].begin(), treatment_arrays[unit].end());
    std::vector<Treatment>& selections = treatment_arrays[unit];
    selections.clear();

    std::iota(candidates.begin(), candidates.end(), 0); // fill with 0, ..., K - 1

    // Get sort order by increasing cost
    std::sort(candidates.begin(), candidates.end(), [&](const Treatment lhs, const Treatment rhs) {
      return lhs.cost < rhs.cost;
    });

    // Push first positive reward point onto stack
    // size_t start = 0;
    while (candidates.size() > 0 and candidates[0].reward <= 0) {
      candidates.pop_front();
    }
    if (candidates.size() == 0) {
      continue;
    }

    selections.push_back(candidates[0]);
    candidates.pop_front();

    while (candidates.size() > 0) {
      Treatment candidate = candidates[0];
      while (candidate_dominates_last_selection(selections, candidate)) {
        selections.pop_back(); // remove point_k
      }

      if (candidate.reward > 0) {
        if (candidates.size() < 1 || candidate.reward > selections.back().reward) {
          selections.push_back(candidate);
        }
      }
    }
  }
}

} // namespace maq

#endif // MAQ_CONVEX_HULL_HPP
