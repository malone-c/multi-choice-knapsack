import pyarrow as pa
import mckp

# Create a test ListArray: [[1, 2], [3], [4, 5, 6]]
# treatment_ids = pa.array([[0, 1, 2], [0, 1, 2], [0, 1, 2]], type=pa.list_(pa.uint32()))
# rewards = pa.array([[0, 110, 200], [0, 230, 300], [0, 210, 500]], type=pa.list_(pa.float64()))
# costs = pa.array([[0, 100, 200], [0, 200, 300], [0, 200, 400]], type=pa.list_(pa.float64()))
# budget_constraint = 500


# Create test data with more customers and smaller numbers
treatment_ids = pa.array([
    [0, 1, 2, 3], 
    [0, 1, 2], 
    [0, 1, 2], 
    [0, 1, 2], 
    [0, 1, 2]
], type=pa.list_(pa.uint32()))

rewards = pa.array([
    [0, 15, 22, 30], 
    [0, 18, 32], 
    [0, 10, 19], 
    [0, 17, 28], 
    [0, 8, 18]
], type=pa.list_(pa.float64()))

costs = pa.array([
    [0, 10, 20, 21], 
    [0, 15, 25], 
    [0, 8, 16], 
    [0, 12, 22], 
    [0, 7, 14]
], type=pa.list_(pa.float64()))

budget_constraint = 50

print("Python ListArray:", treatment_ids)
print("\nC++ Analysis:")

results = mckp.mckp.solve(
    treatment_ids,
    rewards,
    costs,
    budget_constraint,
    0,
)

print(dir(results))
print(results)

print(f"{results['spend'][-2]=}")
print(f"{results['gain'][-2]=}")