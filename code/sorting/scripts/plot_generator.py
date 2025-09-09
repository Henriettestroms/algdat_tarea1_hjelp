import os
import pandas as pd
import matplotlib.pyplot as plt

MEASUREMENTS_DIR = "./data/sorting/measurements/"
PLOTS_DIR = "./data/sorting/plots/"

os.makedirs(PLOTS_DIR, exist_ok=True)

def read_measurements(filename):
    """
    Reads a result file where each line contains:
    n runtime_ms memory_kB
    """
    df = pd.read_csv(filename, sep=" ", header=None,
                     names=["n", "time_ms", "memory_kB"])
    return df

def plot_results(df, title, output_file):
    plt.figure(figsize=(8,5))
    
    # Plot runtime vs input size
    plt.plot(df["n"], df["time_ms"], marker="o", label="Runtime (ms)")
    
    # Optionally: plot memory usage as well
    plt.plot(df["n"], df["memory_kB"], marker="s", label="Memory (kB)")
    
    plt.xlabel("Input size (n)")
    plt.ylabel("Runtime / Memory")
    plt.title(title)
    plt.legend()
    plt.grid(True)
    
    plt.savefig(output_file)
    plt.close()

def main():
    # Iterate over all measurement files in the folder
    for fname in os.listdir(MEASUREMENTS_DIR):
        if fname.endswith(".txt"):
            filepath = os.path.join(MEASUREMENTS_DIR, fname)
            df = read_measurements(filepath)
            
            output_path = os.path.join(PLOTS_DIR, fname.replace(".txt", ".png"))
            plot_results(df, f"Results for {fname}", output_path)
            print(f"Saved plot to {output_path}")

if __name__ == "__main__":
    main()
