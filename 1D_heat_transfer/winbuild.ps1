# Build selected programs in the hello_world directory.

$ErrorActionPreference = "Stop"

$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot "../config")).Path
$CommonRules = Join-Path $RepoRoot "winbuild_common.ps1"

. $CommonRules

$serialTargets = @(
    "heat_1d_serial"
)

# all the other examples use pthreads, which MSVC can't handle

foreach ($target in $serialTargets) {
    Build-Serial `
        -Source (Join-Path $PSScriptRoot "$target.cpp") `
        -Output (Join-Path $PSScriptRoot "$target.exe")
}
