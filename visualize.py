import matplotlib.pyplot as plt
import numpy as np
import sys
import re
import os
import math

def parse_file(filename):
    data = []
    with open(filename, "r") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            # Regex: e.g. [100 ns] 79281
            match = re.match(r"\[(\d+)\s*ns\]\s+(\d+)", line)
            if match:
                time = int(match.group(1))
                count = int(match.group(2))
                data.append((time, count))
    return data

def plot_histogram(data, filename, plot_name):
    if not data:
        print("No valid data found.")
        return
    
    times = [d[0] for d in data]
    counts = [d[1] for d in data]

    labels = [f"{t} ns" for t in times]

    # Weighted average
    avg = np.average(times, weights=counts)
    wcet = max(times)

    # Plot
    plt.figure(figsize=(10, 6))
    bars = plt.bar(range(len(times)), counts, color="skyblue", edgecolor="black")

    plt.yscale("log")  # logarithmic scale

    plt.xlabel("Time interval [ns]")
    plt.ylabel("Frequency (log scale)")
    plt.title(f"Histogram ({plot_name})")

    # Dynamic x-axis tick selection
    max_labels = 20  # maximum number of x-labels to show
    step = max(1, math.ceil(len(times) / max_labels))
    plt.xticks(range(0, len(times), step), labels[::step], rotation=45, ha="right")

    # Annotation (top right corner inside the plot)
    plt.annotate(
        f"Average: {avg:.2f} ns\nWCET: {wcet} ns",
        xy=(0.98, 0.95),
        xycoords="axes fraction",
        ha="right",
        va="top",
        fontsize=11,
        bbox=dict(boxstyle="round,pad=0.3", facecolor="white", alpha=0.7)
    )

    plt.tight_layout()

    # Save as PNG
    base, _ = os.path.splitext(filename)
    out_file = f"{base}_{plot_name}.png"
    out_file_common = f"{base}_output.png"
    plt.savefig(out_file, dpi=150)  # dpi kannst du nach Wunsch ändern
    plt.savefig(out_file_common, dpi=150)
    plt.close()

    print(f"Plot saved as {out_file}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print(f"Usage: python {sys.argv[0]} <file.txt> <name>")
        sys.exit(1)

    filename = sys.argv[1]
    plot_name = sys.argv[2]
    data = parse_file(filename)
    plot_histogram(data, filename, plot_name)
