
param(
    [ValidateSet('all', 'serial', 'openmp', 'mpi', 'clean')]
    [string]$Target = 'all'
)

$ErrorActionPreference = 'Stop'

$Root   = $PSScriptRoot
$OutDir = Join-Path $Root 'build'

if ($Target -eq 'clean') {
    if (Test-Path $OutDir) {
        Remove-Item $OutDir -Recurse -Force
    }
    exit 0
}

# Must be run from a Visual Studio Developer Command Prompt.
if (-not (Get-Command cl.exe -ErrorAction SilentlyContinue)) {
    throw 'cl.exe not found. Run this script from a Visual Studio Developer Command Prompt.'
}

New-Item -ItemType Directory -Force -Path $OutDir |
    Out-Null

# Add portable, non-pthreads sources here.
$SerialTargets = @(
    @{
        Name   = 'hello_world'
        Source = 'hello_world\hello_world.cpp'
    },
    @{
        Name   = 'prime'
        Source = 'hello_world\prime.cpp'
    }
)

$OpenMPTargets = @(
    @{
        Name   = 'hello_world_OMP'
        Source = 'hello_world\hello_world_OMP.cpp'
    }
)

# Use the standard MPI C API for the MS-MPI target.
$MPITargets = @(
    @{
        Name   = 'hello_world_MPI'
        Source = 'hello_world\hello_world_MPI.cpp'
    }
)

$SelectedTargets = switch ($Target) {
    'serial' { $SerialTargets }
    'openmp' { $OpenMPTargets }
    'mpi'    { $MPITargets }
    'all'    { $SerialTargets + $OpenMPTargets + $MPITargets }
}

foreach ($Item in $SelectedTargets) {
    $SourcePath = Join-Path $Root $Item.Source
    $OutputPath = Join-Path $OutDir "$($Item.Name).exe"

    if (-not (Test-Path $SourcePath)) {
        throw "Source file not found: $SourcePath"
    }

    # Shared settings for all builds.
    $ClArgs = @(
        '/nologo'
        '/EHsc'
        '/std:c++20'
        '/W4'
        '/O2'
    )

    # Target-specific settings.
    switch -Regex ($Item.Name) {
        '_OMP$' {
            $ClArgs += '/openmp'
            $ClArgs += $SourcePath
            $ClArgs += "/Fe:$OutputPath"
        }
        '_MPI$' {
            if (-not $env:MSMPI_INC -or
                -not $env:MSMPI_LIB64) {
                throw 'MSMPI_INC or MSMPI_LIB64 is missing.'
            }

            $ClArgs += "/I$env:MSMPI_INC"
            $ClArgs += "/I$(Join-Path $env:MSMPI_INC 'x64')"
            $ClArgs += $SourcePath
            $ClArgs += "/Fe:$OutputPath"
            $ClArgs += '/link'
            $ClArgs += "/LIBPATH:$env:MSMPI_LIB64"
            $ClArgs += 'msmpi.lib'
        }
        default {
            $ClArgs += $SourcePath
            $ClArgs += "/Fe:$OutputPath"
        }
    }

    Write-Host "`nBuilding $($Item.Name)..." -ForegroundColor Cyan

    & cl.exe @ClArgs

    if ($LASTEXITCODE -ne 0) {
        throw "Compilation failed for $($Item.Name) (exit code $LASTEXITCODE)."
    }

    Write-Host "Built: $OutputPath" -ForegroundColor Green
}
