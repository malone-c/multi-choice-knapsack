import pyarrow as pa
import mckp

# Create a test ListArray: [[1, 2], [3], [4, 5, 6]]
offer_ids = pa.array([[1, 2], [3], [4, 5, 6]], type=pa.list_(pa.uint32()))
rewards = pa.array([[10, 20], [30], [40, 50, 60]], type=pa.list_(pa.float64()))
costs = pa.array([[100, 200], [300], [400, 500, 600]], type=pa.list_(pa.float64()))

print("Python ListArray:", offer_ids)
print("\nC++ Analysis:")

mckp.mckp.solve(
    offer_ids,
    rewards,
    costs,
    1000,
    0,
)