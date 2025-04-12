import numpy as np
import pyarrow as pa
import polars as pl
from typing import cast

from .ext import solver_cpp

class Solver:
    def __init__(self):
        self.path = None
        self._is_fit = False
        self.budget = None
        # TODO: Manage treatment ID <-> int mapping here
        # TODO: Manage patient ID <-> int mapping here

    def solve(
        self,
        data: pl.DataFrame,
        budget: np.float64 = np.finfo(np.float64).max,
        n_threads: int = 0,
    ):
        """Solve the multi-armed knapsack problem using a path algorithm."""
        # TODO: Data validation
            # Check typing of columns (list types)
            # Check inner arrays have consistent lengths across outer arrays
                # e.g. all(len(treatment_id_arrays[i]) == len(reward_arrays[i]) == len(cost_arrays[i]) for i in range(len(treatment_id_arrays)))
            # Check for nans/nulls
        assert np.isscalar(budget), "budget should be a scalar."
        assert n_threads >= 0, "n_threads should be >=0."

        table: pa.Table = pl.DataFrame(data).to_arrow()
        
        assert 'treatment_id' in table.column_names, "table must contain a treatment_id column."
        assert 'reward' in table.column_names, "table must contain a reward column."
        assert 'cost' in table.column_names, "table must contain a cost column."

        treatment_id_arrays = table.column("treatment_id").cast(pa.list_(pa.uint32())).combine_chunks()
        reward_arrays = table.column("reward").cast(pa.list_(pa.float64())).combine_chunks()
        cost_arrays = table.column("cost").cast(pa.list_(pa.float64())).combine_chunks()

        self.path = solver_cpp(
            treatment_id_arrays,
            reward_arrays,
            cost_arrays,
            self.budget,
            n_threads,
        )
        return self.path

    # TODO
    def predict(self, spend, prediction_type="matrix"):
        """Predict the underlying treatment allocation pi_B.

        Parameters
        ----------
        spend : scalar
            The budget constraint level B to predict at.

        type : str
            If "matrix", then represent the underlying treatment allocation as a num_samples * K
            matrix, where for row i, the k-th element is 1 if assigning the k-th arm to unit i is
            optimal at a given spend, and 0 otherwise (with all entries 0 if the control arm is assigned).
            If "vector" then represent the underlying treatment allocation as a num_samples-length
            vector where entries take values in the set k=(0, 1, ..., K) where k=0 represents the
            control arm. If at a given spend, the treatment allocation is fractional (i.e., for a
            single unit i there is not sufficient budget left to assign the i-th unit an initial arm,
            or upgrade to the next costlier arm), then the returned vector is the treatment
            allocation in the solution path where the allocation is integer-valued but incurs a cost
            (slightly) less than 'spend'.

        Returns
        -------
        pi_B : ndarray
            The treatment allocation at a given spend per unit.
        """

        assert np.isscalar(spend), "spend should be a scalar."
        assert self._is_fit, "Solver object is not fit."
        if not self._path["complete_path"] and spend > self.budget:
            raise ValueError("maq path is not fit beyond given spend level.")

        # Binary search to get the point in the path when we hit this budget
        spend_path = self._path["spend"]
        path_idx = np.searchsorted(spend_path, spend, side="right") - 1
        if path_idx < 0:
            return np.zeros(self._dim[0], dtype="int")

        ipath = self._path["ipath"][: path_idx + 1]
        kpath = self._path["kpath"][: path_idx + 1]
        
        # Get indices of first instances of unique units in reverse order (last time the treatment was set for each unit)
        ix = np.unique(ipath[::-1], return_index=True)[1]

        if prediction_type == "vector":
            pi_vec = np.zeros(self._dim[0], dtype="int")
            pi_vec[ipath[::-1][ix]] = kpath[::-1][ix] + 1
            return pi_vec

        pi_mat = np.zeros(self._dim, dtype="double")
        pi_mat[ipath[::-1][ix], kpath[::-1][ix]] = 1

        if path_idx == spend_path.shape[0] - 1:
            return pi_mat

        # fractional adjustment?
        spend_diff = spend - spend_path[path_idx]
        next_unit = self._path["ipath"][path_idx + 1]
        next_arm = self._path["kpath"][path_idx + 1]
        prev_arm = np.nonzero(pi_mat[next_unit,])[0]  # already assigned?

        fraction = spend_diff / (spend_path[path_idx + 1] - spend_path[path_idx])
        pi_mat[next_unit, next_arm] = fraction
        if prev_arm.shape[0] > 0:
            pi_mat[next_unit, prev_arm[0]] = 1 - fraction

        return pi_mat



    def plot(self, horizontal_line=True, show_ci=True, max_points=10_000, **kwargs):
        """Plot the Qini curve (requires matplotlib).

        If the underlying policy involves treating zero units (as would be the case if all
        reward estimates are negative), then nothing is plot.

        Parameters
        ----------
        horizontal_line : bool
            Whether to draw a horizontal line where the Qini curve plateaus.
            Only applies if adding to an existing plot where the x-axis extends beyond this curve
            and the maq object is fit with a maximum budget that is sufficient to treat all units
            that are expected to benefit.
        show_ci : bool
            Whether to show estimated 95% confidence bars.
        **kwargs : additional arguments passed to matplotlib.pyplot
        """
        try:
            import matplotlib.pyplot as plt
            from matplotlib import rcParams
        except ImportError:
            raise ImportError("plot method requires matplotlib.")
    
        max_points = max_points or len(self.path_spend_)
        step_size = max(1, len(self.path_spend_) // max_points)

        # matplotlib by default extends the axis xlim by 5% (rcParams["axes.xmargin"])
        xscaling = 1 + rcParams["axes.xmargin"]
        if "color" not in kwargs:
            kwargs["color"] = "black"

        plt.plot(self.path_spend_[::step_size], self.path_gain_[::step_size], **kwargs)
        if "label" in kwargs:
            plt.legend(loc="upper left")
            del kwargs["label"]

        if horizontal_line and self._path["complete_path"]:
            plt.hlines(
                y=self.path_gain_[-1],
                xmin=self.path_spend_[-1],
                xmax=plt.xlim()[1] / xscaling,
                **kwargs
            )

        plt.xlabel("spend")
        plt.ylabel("gain")

    @property
    def path_spend_(self):
        assert self._is_fit, "Solver object is not fit."
        return self._path["spend"]

    @property
    def path_gain_(self):
        assert self._is_fit, "Solver object is not fit."
        return self._path["gain"]

    @property
    def path_std_err_(self):
        assert self._is_fit, "Solver object is not fit."
        return self._path["std_err"]

    @property
    def path_allocated_unit_(self):
        assert self._is_fit, "Solver object is not fit."
        return self._path["ipath"]

    @property
    def path_allocated_arm_(self):
        assert self._is_fit, "Solver object is not fit."
        return self._path["kpath"]

    def __repr__(self):
        if self._is_fit:
            return "MAQ object with {} units and {} arms.".format(
                self._dim[0], self._dim[1]
            )
        else:
            return "MCKP solver object (not fit)."
