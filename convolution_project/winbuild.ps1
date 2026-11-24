# Build selected programs in the 2D_heat_transfer directory.

$ErrorActionPreference = "Stop"

$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot "../config")).Path
$CommonRules = Join-Path $RepoRoot "winbuild_common.ps1"

. $CommonRules

$serialTargets = @(
)

$openMpTargets = @(
)

$msmpiTargets = @(
)

Build-C `
    -Source (Join-Path $PSScriptRoot "conv_serial_c.c") `
    -Output (Join-Path $PSScriptRoot "conv_serial_c.exe")
