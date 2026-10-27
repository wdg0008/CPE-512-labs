# Build selected programs in the add_numbers directory.

$ErrorActionPreference = "Stop"

$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot "../config")).Path
$CommonRules = Join-Path $RepoRoot "winbuild_common.ps1"

. $CommonRules

$serialTargets = @(
    "add_num_serial",
    "add_num_recursive_serial",
    "add_num_rec_level_serial"
)


$msmpiTargets = @(
    "add_num_MPI"
)

foreach ($target in $serialTargets) {
    Build-Serial `
        -Source (Join-Path $PSScriptRoot "$target.cpp") `
        -Output (Join-Path $PSScriptRoot "$target.exe")
}

foreach ($target in $msmpiTargets) {
    Build-MSMPI `
        -Source (Join-Path $PSScriptRoot "$target.cpp") `
        -Output (Join-Path $PSScriptRoot "$target.exe")
}