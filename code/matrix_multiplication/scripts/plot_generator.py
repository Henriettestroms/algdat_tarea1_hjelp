# code/scripts/matrix_plot.py
import os
import re
import pandas as pd
import matplotlib.pyplot as plt

BASE_DIR  = os.path.dirname(os.path.abspath(__file__))
DATA_DIR  = os.path.normpath(os.path.join(BASE_DIR, "..", "data"))
MEAS_DIR  = os.path.join(DATA_DIR, "measurements")
PLOTS_DIR = os.path.join(DATA_DIR, "plots")
os.makedirs(PLOTS_DIR, exist_ok=True)

def read_meas(path):
    fname = os.path.basename(path)
    algo = re.sub(r"^matrix_", "", os.path.splitext(fname)[0])
    df = pd.read_csv(path, sep=r"\s+", header=None)
    # format: n time_ms mem_kB
    if df.shape[1] >= 3:
        df = df.iloc[:, :3]
        df.columns = ["n", "time_ms", "memory_kB"]
        df.insert(0, "algo", algo)
        return df
    raise ValueError(f"Unrecognized format in {fname}")

def main():
    files = [os.path.join(MEAS_DIR, f) for f in os.listdir(MEAS_DIR)
             if f.startswith("matrix_") and f.endswith(".txt")]
    if not files:
        print(f"[matrix_plot] no files like matrix_*.txt in {MEAS_DIR}")
        return

    frames = []
    for f in files:
        try:
            frames.append(read_meas(f))
        except Exception as e:
            print(f"[matrix_plot] skip {f}: {e}")

    if not frames:
        print("[matrix_plot] no valid data")
        return

    df = pd.concat(frames, ignore_index=True)

    # median per (algo,n)
    agg = df.groupby(["algo", "n"], as_index=False).median(numeric_only=True)

    # per-algo
    for algo, d in agg.groupby("algo"):
        d = d.sort_values("n")
        plt.figure(figsize=(7,5))
        plt.plot(d["n"], d["time_ms"]/1000.0, marker="o", label="Runtime (s)")
        plt.title(f"Matrix {algo} (median)")
        plt.xlabel("n"); plt.ylabel("Runtime (s)")
        plt.xscale("log"); plt.grid(True, which="both"); plt.legend()
        plt.tight_layout()
        out = os.path.join(PLOTS_DIR, f"matrix_{algo}.png")
        plt.savefig(out); plt.close()
        print(f"[matrix_plot] saved {out}")

    # compare
    plt.figure(figsize=(7,5))
    for algo, d in agg.groupby("algo"):
        d = d.sort_values("n")
        plt.plot(d["n"], d["time_ms"]/1000.0, marker="o", label=algo)
    plt.title("Matrix multiplication runtime vs n (median)")
    plt.xlabel("n"); plt.ylabel("Runtime (s)")
    plt.xscale("log"); plt.grid(True, which="both"); plt.legend()
    plt.tight_layout()
    out_all = os.path.join(PLOTS_DIR, "matrix_runtime_vs_n.png")
    plt.savefig(out_all); plt.close()
    print(f"[matrix_plot] saved {out_all}")

if __name__ == "__main__":
    main()
