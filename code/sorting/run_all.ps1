# run_all.ps1 — robust kjede: bygg -> finn riktig data-mappe -> kjør alle algoritmer -> lag plott
# Kjør denne fra hvor som helst; scriptet navigerer selv.

$ErrorActionPreference = "Stop"

function Resolve-Python {
  foreach ($c in @("python", "py")) {
    try { & $c --version *> $null; if ($LASTEXITCODE -eq 0) { return $c } } catch {}
  }
  throw "Fant ikke Python i PATH. Installer Python, åpne nytt PowerShell-vindu og prøv igjen."
}

# --- 0) Finn viktige stier ---
$ScriptDir   = Split-Path -Parent $MyInvocation.MyCommand.Path           # ...\code\sorting
$RepoRoot    = Resolve-Path (Join-Path $ScriptDir "..\..") | ForEach-Object { $_.Path } # prosjektrot
$ExePath     = Join-Path $ScriptDir "bin\sort.exe"
$CppFiles    = @(
  (Join-Path $ScriptDir "sorting.cpp"),
  (Join-Path $ScriptDir "algorithms\insertionsort.cpp"),
  (Join-Path $ScriptDir "algorithms\mergesort.cpp"),
  (Join-Path $ScriptDir "algorithms\quicksort.cpp"),
  (Join-Path $ScriptDir "algorithms\pandasort.cpp")
)

# --- 1) Bygg C++ hvis nødvendig ---
if (-not (Test-Path $ExePath)) {
  if (-not (Test-Path (Join-Path $ScriptDir "bin"))) { New-Item -ItemType Directory -Path (Join-Path $ScriptDir "bin") | Out-Null }
  Write-Host "==> Kompilerer..." -ForegroundColor Cyan
  & g++ -O2 -std=gnu++17 @CppFiles -o $ExePath
  if ($LASTEXITCODE -ne 0) { throw "Kompilering feilet." }
} else {
  Write-Host "==> Hopper over kompilering (bin\sort.exe finnes)" -ForegroundColor DarkGray
}

# --- 2) Oppdag eksisterende data-layout ---
# Kandidater (i rekkefølge): 
#   A) <repo>\data\array_input
#   B) <repo>\data\sorting\array_input
#   C) <script>\data\array_input  (fallback)
$Candidates = @(
  @{ Root = $RepoRoot;               Input = "data\array_input" },
  @{ Root = $RepoRoot;               Input = "data\sorting\array_input" },
  @{ Root = $ScriptDir;              Input = "data\array_input" }
)

$Choice = $null
foreach ($c in $Candidates) {
  $dir = Join-Path $c.Root $c.Input
  if (Test-Path $dir) { $Choice = $c; break }
}

if (-not $Choice) {
  # Ikke funnet noen eksisterende input-mappe: lag A) på rotnivå for å unngå å opprette under code\sorting
  $Choice = @{ Root = $RepoRoot; Input = "data\array_input" }
  New-Item -ItemType Directory -Path (Join-Path $Choice.Root $Choice.Input) -Force | Out-Null
  Write-Host "==> Opprettet $(Join-Path $Choice.Root $Choice.Input)" -ForegroundColor Yellow
}

$RootDir   = $Choice.Root
$InputDir  = Join-Path $RootDir $Choice.Input
# Output/Målinger/Plots skal alltid ligge under samme 'data\'-rot som input:
$OutputDir = Join-Path $RootDir ($Choice.Input -replace "array_input$","array_output")
$MeasDir   = Join-Path $RootDir ($Choice.Input -replace "array_input$","measurements")
$PlotsDir  = Join-Path $RootDir ($Choice.Input -replace "array_input$","plots")

# Sørg for at destinasjonene finnes (men vi lager dem under brukerens eksisterende data-rot)
foreach ($d in @($OutputDir,$MeasDir,$PlotsDir)) { New-Item -ItemType Directory -Path $d -Force | Out-Null }

Write-Host "==> Bruker data-rot: $RootDir" -ForegroundColor Cyan
Write-Host "    Input:        $InputDir"
Write-Host "    Output:       $OutputDir"
Write-Host "    Measurements: $MeasDir"
Write-Host "    Plots:        $PlotsDir"

# --- 3) Generer input-arrays (valgfritt): kopier til valgt InputDir hvis generatoren skriver et annet sted ---
$py = Resolve-Python
Write-Host "==> Genererer input (hvis scriptet ditt lager i annen mappe, kopieres de inn)..." -ForegroundColor Cyan
& $py (Join-Path $ScriptDir "scripts\array_generator.py") 2>$null | Out-Null

# Finn mulige genererte steder og kopier inn til valgt InputDir (uten å overskrive nyere filer)
$GenSpots = @(
  (Join-Path $ScriptDir "data\array_input"),
  (Join-Path $RepoRoot  "data\array_input"),
  (Join-Path $RepoRoot  "data\sorting\array_input")
) | Where-Object { Test-Path $_ }

foreach ($spot in $GenSpots) {
  if ($spot -ne $InputDir) {
    Get-ChildItem $spot -Filter *.txt -File -ErrorAction SilentlyContinue | ForEach-Object {
      $dst = Join-Path $InputDir $_.Name
      if (-not (Test-Path $dst)) { Copy-Item $_.FullName -Destination $dst }
    }
  }
}

# --- 4) Kjør alle algoritmer på alle input-filer, med korrekt arbeidsmappe ---
$inputs = Get-ChildItem $InputDir -Filter *.txt | Sort-Object Name
if ($inputs.Count -eq 0) {
  throw "Ingen inputfiler i $InputDir. Legg inn minst én .txt (første tall = n, så n verdier)."
}

$algos = @("quick","merge","insertion","panda","stdsort")
Write-Host "==> Kjører algoritmer med arbeidskatalog = $RootDir (slik at ./data/... havner riktig)..." -ForegroundColor Cyan

Push-Location $RootDir
try {
  foreach ($f in $inputs) {
    foreach ($a in $algos) {
      Write-Host ("  -> {0,-9} : {1}" -f $a, $f.Name)
      # Bruk cmd.exe type for robust stdin-redirect til .exe i PowerShell
      $cmd = "type ""$($f.FullName)"" | ""$ExePath"" --algo=$a"
      cmd /c $cmd | Out-Null
    }
  }
}
finally {
  Pop-Location
}

# --- 5) Lag plott direkte mot valgt Measurements/Plots (uavhengig av gamle hardkodede stier) ---
Write-Host "==> Lager plott (direkte fra $MeasDir til $PlotsDir)..." -ForegroundColor Cyan

$pyCode = @"
import os, re, sys
import pandas as pd
import matplotlib.pyplot as plt

MEAS_DIR = r'$MeasDir'
PLOTS_DIR = r'$PlotsDir'
os.makedirs(PLOTS_DIR, exist_ok=True)

def read_measurement_file(path):
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

files = [os.path.join(MEAS_DIR, f) for f in os.listdir(MEAS_DIR) if f.endswith(".txt")]
if not files:
    print(f"No .txt files found in {MEAS_DIR}")
    sys.exit(0)

frames = []
for f in files:
    try:
        df = read_measurement_file(f)
        frames.append(df)
        for algo, d in df.groupby("algo"):
            d = d.sort_values("n")
            plt.figure(figsize=(7,5))
            # vis tid i sekunder for mer intuitiv skala
            plt.plot(d["n"], d["time_ms"]/1000.0, marker="o", label="Runtime (s)")
            if "memory_kB" in d.columns:
                plt.plot(d["n"], d["memory_kB"], marker="s", label="Memory (kB)")
            plt.title(f"{algo} results")
            plt.xlabel("Input size (n)")
            plt.ylabel("Runtime (s) / Memory (kB)")
            plt.legend(); plt.grid(True); plt.tight_layout()
            plt.savefig(os.path.join(PLOTS_DIR, f"{algo}.png")); plt.close()
    except Exception as e:
        print(f"Skipping {f}: {e}")

if not frames:
    print("No valid measurement data.")
    sys.exit(0)

df_all = pd.concat(frames, ignore_index=True)

def plot_compare(df_all, metric, title, outpath, to_seconds=False):
    plt.figure(figsize=(7,5))
    for algo, d in df_all.groupby("algo"):
        d = d.sort_values("n")
        y = d[metric]/1000.0 if to_seconds else d[metric]
        plt.plot(d["n"], y, marker="o", label=algo)
    plt.title(title)
    plt.xlabel("Input size (n)")
    plt.ylabel("time (s)" if to_seconds else metric)
    plt.legend(); plt.grid(True); plt.tight_layout()
    plt.savefig(outpath); plt.close()

# sammenligninger
plot_compare(df_all, "time_ms", "Runtime vs n (all algorithms)",
             os.path.join(PLOTS_DIR, "runtime_vs_n.png"), to_seconds=True)
if "memory_kB" in df_all.columns:
    plot_compare(df_all, "memory_kB", "Memory vs n (all algorithms)",
                 os.path.join(PLOTS_DIR, "memory_vs_n.png"))
print(f"Saved plots to {PLOTS_DIR}")
"@

$tmpPy = Join-Path $env:TEMP ("plots_{0}.py" -f ([guid]::NewGuid()))
Set-Content -LiteralPath $tmpPy -Value $pyCode -Encoding UTF8
& $py $tmpPy
Remove-Item $tmpPy -Force
