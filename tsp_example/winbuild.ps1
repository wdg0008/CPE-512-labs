# Build selected programs in the tsp_example directory.

$ErrorActionPreference = "Stop"

$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot "../config")).Path
$CommonRules = Join-Path $RepoRoot "winbuild_common.ps1"

. $CommonRules

$serialTargets = @(
    "tsp_serial"
)

$openMpTargets = @(
    "tsp_OMP"
)

foreach ($target in $serialTargets) {
    Build-Serial `
        -Source (Join-Path $PSScriptRoot "$target.cpp") `
        -Output (Join-Path $PSScriptRoot "$target.exe")
}

foreach ($target in $openMpTargets) {
    Build-OpenMP `
        -Source (Join-Path $PSScriptRoot "$target.cpp") `
        -Output (Join-Path $PSScriptRoot "$target.exe")
}
