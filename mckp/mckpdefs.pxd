from libcpp cimport bool
from libcpp.pair cimport pair
from libcpp.vector cimport vector

ctypedef pair[vector[vector[double]], vector[vector[size_t]]] solution_path

cdef extern from "MAQ.h" namespace "mckp":
    solution_path run(
        vector[vector[unsigned int]]& treatment_id_arrays,
        vector[vector[double]]& reward_arrays,
        vector[vector[double]]& cost_arrays,
        double budget,
        unsigned int num_threads
    )