import csv
import sys
from typing import Dict, List

import matplotlib.pyplot as plt


def read_timings(csv_path: str) -> (List[int], Dict[str, List[float]]):
    """Read timing data from a CSV file.

    Parameters
    ----------
    csv_path: str
        Path to a CSV file containing columns like
        ``n,quick,merge,insertion,panda`` where ``n`` is the
        input size and the remaining columns represent algorithm
        runtimes.

    Returns
    -------
    Tuple of ``n`` values and a mapping from algorithm name to a list
    of runtimes.
    """
    sizes: List[int] = []
    timings: Dict[str, List[float]] = {}
    with open(csv_path, newline="") as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            sizes.append(int(row["n"]))
            for key, value in row.items():
                if key == "n":
                    continue
                timings.setdefault(key, []).append(float(value))
    return sizes, timings


def generate_plot(csv_path: str, output_path: str) -> None:
    """Generate a PNG plot of algorithm runtimes.

    Parameters
    ----------
    csv_path: str
        Path to the CSV input file.
    output_path: str
        Path where the PNG file will be saved.
    """
    sizes, timings = read_timings(csv_path)

    for name, values in timings.items():
        plt.plot(sizes, values, label=name)

    plt.xlabel("Input size (n)")
    plt.ylabel("Runtime (s)")
    plt.title("Algorithm runtime comparison")
    plt.legend()
    plt.grid(True)
    plt.savefig(output_path)
    plt.close()


def main(argv: List[str]) -> int:
    if len(argv) != 3:
        print("Usage: python plot_generator.py <timings.csv> <output.png>")
        return 1

    csv_path, output_path = argv[1], argv[2]
    generate_plot(csv_path, output_path)
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))