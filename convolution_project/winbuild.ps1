# Build selected programs in the 2D_heat_transfer directory.

$ErrorActionPreference = "Stop"

$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot "../config")).Path
$CommonRules = Join-Path $RepoRoot "winbuild_common.ps1"

. $CommonRules

$serialTargets = @(
    "conv_serial",
    "conv_span_serial"
)

$openMpTargets = @(
    "conv_OMP"
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

Build-C `
    -Source (Join-Path $PSScriptRoot "conv_serial_c.c") `
    -Output (Join-Path $PSScriptRoot "conv_serial_c.exe")
Build-C `
    -Source (Join-Path $PSScriptRoot "conv_dynamic_serial_c.c") `
    -Output (Join-Path $PSScriptRoot "conv_dynamic_serial_c.exe")
Build-C `
    -Source (Join-Path $PSScriptRoot "conv_OMP_c.c") `
    -Output (Join-Path $PSScriptRoot "conv_OMP_c.exe") `
    -ExtraFlags "/openmp"
Build-C `
    -Source (Join-Path $PSScriptRoot "conv_dynamic_OMP_c.c") `
    -Output (Join-Path $PSScriptRoot "conv_dynamic_OMP_c.exe") `
    -ExtraFlags "/openmp"
