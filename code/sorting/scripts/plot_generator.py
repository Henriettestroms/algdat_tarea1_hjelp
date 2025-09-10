# code/sorting/scripts/plot_generator.py
import os
import re
import pandas as pd
import matplotlib.pyplot as plt

# ✱ Finn data-mappene relativt til denne fila, ikke cwd
BASE_DIR  = os.path.dirname(os.path.abspath(__file__))
DATA_DIR  = os.path.normpath(os.path.join(BASE_DIR, "..", "data"))
MEAS_DIR  = os.path.join(DATA_DIR, "measurements")
PLOTS_DIR = os.path.join(DATA_DIR, "plots")

os.makedirs(PLOTS_DIR, exist_ok=True)

def read_measurement_file(path):
    """
    Returnerer DataFrame med kolonner: algo, n, time_ms, memory_kB.
    Støtter to formater:
      A) per-algofil (filnavn = algo), kolonner: n time_ms memory_kB
      B) samlet fil der første kolonne er 'algo': algo n time_ms memory_kB
    """
    fname = os.path.basename(path)
    df = pd.read_csv(path, sep=r"\s+", header=None)
    if df.shape[1] == 3:
        algo = re.sub(r"\.txt$", "", fname)
        df.columns = ["n", "time_ms", "memory_kB"]
        df.insert(0, "algo", algo)
    elif df.shape[1] >= 4:
        df = df.iloc[:, :4]
        df.columns = ["algo", "n", "time_ms", "memory_kB"]
    else:
        raise ValueError(f"Unrecognized format in {fname}")
    return df

def plot_single_algo(df_algo, title, outpath):
    df_algo = df_algo.sort_values("n")
    plt.figure(figsize=(7,5))
    # ✱ Vis tid i SEKUNDER
    plt.plot(df_algo["n"], df_algo["time_ms"]/1000.0, marker="o", label="Runtime (s)")
    if "memory_kB" in df_algo.columns:
        plt.plot(df_algo["n"], df_algo["memory_kB"], marker="s", label="Memory (kB)")
    plt.title(title)
    plt.xlabel("Input size (n)")
    plt.ylabel("Runtime (s) / Memory (kB)")
    plt.xscale("log")  # ✱ log-x så 10, 1e3, 1e5 blir synlig struktur
    plt.legend()
    plt.grid(True, which="both")
    plt.tight_layout()
    plt.savefig(outpath)
    plt.close()

def plot_compare(df_all, metric, title, outpath, to_seconds=False):
    plt.figure(figsize=(7,5))
    for algo, d in df_all.groupby("algo"):
        d = d.sort_values("n")
        y = d[metric]/1000.0 if to_seconds else d[metric]
        plt.plot(d["n"], y, marker="o", label=algo)
    plt.title(title)
    plt.xlabel("Input size (n)")
    plt.ylabel("time (s)" if to_seconds else metric)
    plt.xscale("log")  # ✱
    plt.legend()
    plt.grid(True, which="both")
    plt.tight_layout()
    plt.savefig(outpath)
    plt.close()

def main():
    print(f"[plot] MEAS_DIR = {MEAS_DIR}")
    print(f"[plot] PLOTS_DIR = {PLOTS_DIR}")

    if not os.path.isdir(MEAS_DIR):
        print(f"[plot] Measurements dir not found: {MEAS_DIR}")
        return

    files = [os.path.join(MEAS_DIR, f) for f in os.listdir(MEAS_DIR) if f.endswith(".txt")]
    if not files:
        print(f"[plot] No .txt files found in {MEAS_DIR}")
        return
    print(f"[plot] Found {len(files)} measurement file(s):")
    for f in files: print("   -", os.path.basename(f))

    frames = []
    for f in files:
        try:
            df = read_measurement_file(f)
            frames.append(df)
        except Exception as e:
            print(f"[plot] Skipping {f}: {e}")

    if not frames:
        print("[plot] No valid measurement data after parsing.")
        return

    df_all = pd.concat(frames, ignore_index=True)

    # ✱ Aggreger per (algo, n) for å unngå “loddrette hopp” når samme n har mange målinger
    agg = df_all.groupby(["algo", "n"], as_index=False).median(numeric_only=True)

    # Per-algoritme (median)
    for algo, d in agg.groupby("algo"):
        out = os.path.join(PLOTS_DIR, f"{algo}.png")
        plot_single_algo(d, f"{algo} (median over repeats)", out)
        print(f"[plot] saved {out}")

    # Sammenligningsplot (median), tid i sekunder
    out1 = os.path.join(PLOTS_DIR, "runtime_vs_n.png")
    plot_compare(agg, "time_ms", "Runtime vs n (all algorithms)", out1, to_seconds=True)
    print(f"[plot] saved {out1}")

    # ✱ (valgfritt) normaliseringsplott: tid/(n log n) og tid/n^2
    import numpy as np
    def safe_log2(x):
        x = np.maximum(2, x)
        return np.log2(x)

    plt.figure(figsize=(7,5))
    for algo, d in agg.groupby("algo"):
        d = d.sort_values("n")
        y = (d["time_ms"] / (d["n"] * safe_log2(d["n"])))
        plt.plot(d["n"], y, marker="o", label=algo)
    plt.title("time / (n log2 n)")
    plt.xlabel("n"); plt.ylabel("scaled time")
    plt.xscale("log")
    plt.legend(); plt.grid(True, which="both")
    plt.tight_layout()
    out2 = os.path.join(PLOTS_DIR, "runtime_over_nlogn.png")
    plt.savefig(out2); plt.close()
    print(f"[plot] saved {out2}")

    plt.figure(figsize=(7,5))
    for algo, d in agg.groupby("algo"):
        d = d.sort_values("n")
        y = (d["time_ms"] / (d["n"]**2))
        plt.plot(d["n"], y, marker="o", label=algo)
    plt.title("time / n^2")
    plt.xlabel("n"); plt.ylabel("scaled time")
    plt.xscale("log")
    plt.legend(); plt.grid(True, which="both")
    plt.tight_layout()
    out3 = os.path.join(PLOTS_DIR, "runtime_over_n2.png")
    plt.savefig(out3); plt.close()
    print(f"[plot] saved {out3}")

if __name__ == "__main__":
    main()