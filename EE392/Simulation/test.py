from model import FlashModule
from time import sleep
import matplotlib.pyplot as plt
import argparse
import numpy as np

# Arguments
parser = argparse.ArgumentParser(
    description="Flash LIM Architecture Test", add_help=True
)
parser.add_argument(
    "-correct_key", type=int, help="Correct Key set by the designer", default=4000
)
parser.add_argument("-debug", type=bool, help="Debug mode", default=False)
parser.add_argument("-num_strings", type=int, help="Number of strings", default=10)
parser.add_argument("-key_space", type=int, help="Key space", default=100)
args = parser.parse_args()

flash = FlashModule(
    0, args.correct_key, args.num_strings
)  # initialize with correct key, simulating designing
flash.debug = args.debug

"""
Functions are represented as a list of tuples, where each tuple corresponds to a NAND string's input, with first value of tuple signifying the number of zeros and second value signifying the number of ones to be inserted.
Every function values can be decomposed in this manner, for example:
AB'C + D'C'
for the values of A=0, B=1, C=1, D=0
will correspond to:
[(2, 1), (1, 1)]
"""
functions = [[(3, 2), (5, 1)], [(2, 4), (5, 6)]]
colors = ["r", "c", "g"]  # need to have at least as many colors as functions
symbols = ["o", "^"]

fig, ax = plt.subplots()
for f_idx in range(len(functions)):
    print(f"For function {f_idx}:")
    key_streak = []
    rng = range(
        args.correct_key - int(args.key_space / 2),
        args.correct_key + int(args.key_space / 2),
    )
    for i in rng:
        flash.reinit(i)
        streak = 0
        while streak <= flash.block.num_pages:
            Y, Yhat = flash.write(functions[f_idx])
            if Y != Yhat:
                if flash.debug:
                    print("--------------------")
                break
            else:
                if flash.debug and i == args.correct_key:
                    print(f"Valid with Y = {Y}, now for key {i}: streak = {streak}")
            # sleep(0.5)
            streak += 1
        key_streak.append(streak)

    print(key_streak)
    ax.stem(
        list(rng),
        key_streak,
        use_line_collection=True,
        linefmt=f"{colors[f_idx]}-",
        markerfmt=f"{colors[f_idx]}{symbols[f_idx%2]}",
        basefmt=f"{colors[f_idx]} ",
        label=f"y{f_idx+1}",
    )

ax.axvspan(
    args.correct_key - 1,
    args.correct_key + 1,
    color="yellow",
    alpha=0.5,
    label="Correct Key",
)

usage_per_string = np.sum(flash.block.usage_count, axis=0)
print("Usage count per string:", usage_per_string)
print(
    "Each flip requires 3ms to complete, therefore total time taken:",
    np.sum(usage_per_string) * 3,
    "ms",
)
ax.set_title("Key Streaks for different functions")
ax.set_xlabel("Key")
ax.set_ylabel("Streak")
ax.legend(loc="upper left")
plt.tight_layout()
plt.show()
