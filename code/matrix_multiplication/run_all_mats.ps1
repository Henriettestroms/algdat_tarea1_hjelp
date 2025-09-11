# code/matrix_multiplication/run_all_mats.ps1
$ErrorActionPreference = "Stop"

function Resolve-Python {
  foreach ($c in @("python","py")) { try { & $c --version *> $null; if ($LASTEXITCODE -eq 0) { return $c } } catch {} }
  throw "Python ikke i PATH."
}

# --- stier relativt til denne ps1-filen ---
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path        # code\matrix_multiplication
$BinDir    = Join-Path $ScriptDir "bin"
$ExePath   = Join-Path $BinDir    "matmul.exe"

$Driver    = Join-Path $ScriptDir "matrix_multiplication.cpp"
$NaiveCpp  = Join-Path $ScriptDir "algorithms\naive.cpp"
$StrasCpp  = Join-Path $ScriptDir "algorithms\strassen.cpp"

# ✱ ENDRING: bruk data under code\matrix_multiplication\data
$DataDir   = Join-Path $ScriptDir "data"                             # ✱ ENDRING
$InDir     = Join-Path $DataDir "matrix_input"                       # ✱ ENDRING
$OutDir    = Join-Path $DataDir "matrix_output"                      # ✱ ENDRING
$MeasDir   = Join-Path $DataDir "measurements"                       # ✱ ENDRING
$PlotsDir  = Join-Path $DataDir "plots"                              # ✱ ENDRING

# ✱ ENDRING: scripts ligger her
$PyGen     = Join-Path $ScriptDir "scripts\matrix_generator.py"      # ✱ ENDRING
$PyPlot    = Join-Path $ScriptDir "scripts\plot_generator.py"           # ✱ ENDRING

# --- bygg ---
if (-not (Test-Path $BinDir)) { New-Item -ItemType Directory -Path $BinDir | Out-Null }
Write-Host "==> Kompilerer matrix_multiplication..." -ForegroundColor Cyan
& g++ -O2 -std=gnu++17 `
  $Driver `
  $NaiveCpp `
  $StrasCpp `
  -o $ExePath
if ($LASTEXITCODE -ne 0) { throw "Kompilering feilet." }

# --- sørg for data-mapper før generering ---
foreach ($d in @($InDir,$OutDir,$MeasDir,$PlotsDir)) { if (-not (Test-Path $d)) { New-Item -ItemType Directory -Path $d | Out-Null } }

# --- generer matriser ---
$py = Resolve-Python
Write-Host "==> Genererer matriser..." -ForegroundColor Cyan
& $py $PyGen

# --- kjør alle par *_1.txt + *_2.txt ---
$lefts  = Get-ChildItem $InDir -Filter "*_1.txt" | Sort-Object Name
$algos  = @("naive","strassen")

Write-Host "==> Kjører matrise-multiplikasjon..." -ForegroundColor Cyan
foreach ($l in $lefts) {
  $base = [IO.Path]::GetFileNameWithoutExtension($l.Name) -replace "_1$",""
  $r = Join-Path $InDir ($base + "_2.txt")
  if (-not (Test-Path $r)) { Write-Warning "Mangler høyrematrise for $($l.Name)"; continue }

  $nStr = ($base -split "_")[0]
  if (-not ($nStr -as [int])) { Write-Warning "Klarte ikke tolke n i $base"; continue }

  # bygg stdin: n + A + B
  $tmp = Join-Path $env:TEMP ("mm_{0}.txt" -f ([guid]::NewGuid()))
  "$nStr" | Out-File -FilePath $tmp -Encoding ascii
  Get-Content $l.FullName | Add-Content -Path $tmp -Encoding ascii
  Get-Content $r         | Add-Content -Path $tmp -Encoding ascii

  foreach ($a in $algos) {
    Write-Host ("  -> {0,-8} : {1}" -f $a, $base)
    $cmd = "type ""$tmp"" | ""$ExePath"" --algo=$a"
    cmd /c $cmd | Out-Null
  }
  Remove-Item $tmp -Force
}

# --- plott ---
Write-Host "==> Lager plott..." -ForegroundColor Cyan
& $py $PyPlot

Write-Host ""
Write-Host "Ferdig " -ForegroundColor Green
Write-Host "Input:        $(Resolve-Path $InDir)"
Write-Host "Output:       $(Resolve-Path $OutDir)"
Write-Host "Measurements: $(Resolve-Path $MeasDir)"
Write-Host "Plots:        $(Resolve-Path $PlotsDir)"