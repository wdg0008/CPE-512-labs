# Overall build orchestrator for Visual Studio on Windows

param(
    [string]$Target
)

$ErrorActionPreference = 'Stop'

$BuildDirectories = @(
    '1D_heat_transfer'
    '2D_heat_transfer'
    'add_numbers'
    'hello_world'
    'histogram'
    'tsp_example'
)

if ($Target -eq 'clean') {
    $ArtifactExtensions = @(
        '.exe'
        '.obj'
        '.out'
    )

    $Artifacts = foreach ($DirectoryName in $BuildDirectories) {
        $DirectoryPath = Join-Path $PSScriptRoot $DirectoryName

        if (Test-Path -LiteralPath $DirectoryPath -PathType Container) {
            Get-ChildItem -LiteralPath $DirectoryPath -Recurse -File -Force |
                Where-Object { $_.Extension -in $ArtifactExtensions }
        }
    }

    foreach ($Artifact in $Artifacts) {
        Remove-Item -LiteralPath $Artifact.FullName -Force
        Write-Host "Removed $($Artifact.FullName)"
    }

    exit 0
}

foreach ($DirectoryName in $BuildDirectories) {
    $DirectoryPath = Join-Path $PSScriptRoot $DirectoryName
    $BuildScript = Join-Path $DirectoryPath 'winbuild.ps1'

    if (Test-Path -LiteralPath $BuildScript -PathType Leaf) {
        Write-Host "`nInvoking $BuildScript..." -ForegroundColor Cyan
        & $BuildScript
    }
}
