# Build selected programs in the hello_world directory.

$ErrorActionPreference = 'Stop'

$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$CommonRules = Join-Path $RepoRoot 'winbuild_common.ps1'

. $CommonRules

Push-Location $PSScriptRoot
try {
    # Ordinary C++ program
    Build-Serial `
        -Source 'hello_world.cpp' `
        -Output 'hello_world.exe'

    # OpenMP program
    Build-OpenMP `
        -Source 'hello_world_OMP.cpp' `
        -Output 'hello_world_OMP.exe'

    # MPI program
    Build-MSMPI `
        -Source 'hello_world_MPI.cpp' `
        -Output 'hello_world_MPI.exe'

    # Uncomment this if you also want the ordinary prime example:
    Build-Serial -Source 'prime.cpp' -Output 'prime.exe'
}
finally {
    Pop-Location
}