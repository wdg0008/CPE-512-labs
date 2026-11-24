# Build selected programs in the hello_world directory.

$ErrorActionPreference = "Stop"

$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot "../config")).Path
$CommonRules = Join-Path $RepoRoot "winbuild_common.ps1"

. $CommonRules

$serialTargets = @(
    "hello_world"
)

$openMpTargets = @(
    "hello_world_OMP"
)

$msmpiTargets = @(
    "hello_world_MPI",
    "prime"
)

foreach ($target in $serialTargets) {
    Build-CXX `
        -Source (Join-Path $PSScriptRoot "$target.cpp") `
        -Output (Join-Path $PSScriptRoot "$target.exe")
}

foreach ($target in $openMpTargets) {
    Build-CXX `
        -Source (Join-Path $PSScriptRoot "$target.cpp") `
        -Output (Join-Path $PSScriptRoot "$target.exe") `
        -ExtraFlags "/openmp"
}

foreach ($target in $msmpiTargets) {
    Build-MSMPI `
        -Source (Join-Path $PSScriptRoot "$target.cpp") `
        -Output (Join-Path $PSScriptRoot "$target.exe")
}