## README, how to run the code`INF-2025-1-TAREA-1`

### Run the sorting algorithms:

To run the sorting algorithms and plot the result, you must run the code from the sorting map.

```bash
\code\sorting
```

To remove old plots and files you run:
```bash
Remove-item .\data\plots\* -Force -Recurse -ErrorAction SilentlyContinue

Remove-item .\data\array_output\* -Force -Recurse -ErrorAction SilentlyContinue

Remove-item .\data\measurements\* -Force -Recurse -ErrorAction SilentlyContinue
```

To generate the array inputs: 

```Bash
python .\scripts\array_generator.py
```

Run the algorithms with all the datasets by running: 

```Bash
$algos = "quick","merge","insertion","panda","stdsort"
Get-ChildItem .\data\array_input\*.txt | Sort-Object Name | ForEach-Object {
    $in = $_.FullName
    foreach ($a in $algos) {
        Get-Content $in -Raw | .\bin\sort.exe --algo=$a
    }
}

```
And lastley plot the plots: 

```Bash
python .\scripts\plot_generator.py
```
### Run the matrix algorithms

To rin the matrix algorithms and plot the result you must run the code from the matrix_multiplication map.

First (if needed), to remove old files you run:

```Bash
Remove-Item .\data\matrix_input\* -Force -ErrorAction SilentlyContinue
Remove-Item .\data\matrix_output\* -Force -ErrorAction SilentlyContinue
Remove-Item .\data\measurements\* -Force -ErrorAction SilentlyContinue
```
Then you run the powershell script:

````Bash
powershell -ExecutionPolicy Bypass -File .\run_all_mats.ps1
````

