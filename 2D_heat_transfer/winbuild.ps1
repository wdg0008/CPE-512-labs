# Build selected programs in the 2D_heat_transfer directory.

$ErrorActionPreference = "Stop"

$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot "../config")).Path
$CommonRules = Join-Path $RepoRoot "winbuild_common.ps1"

. $CommonRules

$serialTargets = @(
    "heat_2d_serial"
)

$openMpTargets = @(
    "heat_2d_OMP"
)

$msmpiTargets = @(
    "heat_2D_loc_blk_MPI",
    "heat_2D_loc_blk_MPI_rev1",
    "heat_2D_loc_blk_MPI_rev2",
    "heat_2d_sync_MPI",
    "heat_2d_usr_buf_MPI",
    "heat_2d_non_blk_MPI",
    "heat_2d_snd_rcv_MPI"
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

foreach ($target in $msmpiTargets) {
    Build-MSMPI `
        -Source (Join-Path $PSScriptRoot "$target.cpp") `
        -Output (Join-Path $PSScriptRoot "$target.exe")
}