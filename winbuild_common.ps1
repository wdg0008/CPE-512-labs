# Shared build rules for MSVC, OpenMP, and Microsoft MPI.

$script:CommonCxxFlags = @(
    '/nologo'
    '/EHsc'
    '/std:c++20'
    '/W3' # /W4 is too aggressive in most cases.
    '/permissive-'
)

function Invoke-MSVCBuild {
    [CmdletBinding()]
    param(
        [Parameter(Mandatory)]
        [string] $Source,

        [Parameter(Mandatory)]
        [string] $Output,

        [string[]] $CompilerFlags = @(),

        [string[]] $LinkerFlags = @()
    )

    $sourcePath = (Resolve-Path -LiteralPath $Source).Path
    $outputPath = $Output

    $arguments = @(
        $script:CommonCxxFlags
        $CompilerFlags
        "/Fe:$outputPath"
        $sourcePath
    )

    if ($LinkerFlags.Count -gt 0) {
        $arguments += '/link'
        $arguments += $LinkerFlags
    }

    Write-Host "Building $sourcePath -> $outputPath"

    & cl.exe @arguments

    if ($LASTEXITCODE -ne 0) {
        throw "MSVC compilation failed for '$sourcePath' (exit code $LASTEXITCODE)."
    }
}

function Build-Serial {
    [CmdletBinding()]
    param(
        [Parameter(Mandatory)]
        [string] $Source,

        [string] $Output,

        [string[]] $ExtraFlags = @()
    )

    if (-not $Output) {
        $Output = [System.IO.Path]::ChangeExtension(
            [System.IO.Path]::GetFileName($Source), '.exe'
        )
    }

    Invoke-MSVCBuild `
        -Source $Source `
        -Output $Output `
        -CompilerFlags $ExtraFlags
}

function Build-OpenMP {
    [CmdletBinding()]
    param(
        [Parameter(Mandatory)]
        [string] $Source,

        [string] $Output,

        [string[]] $ExtraFlags = @()
    )

    if (-not $Output) {
        $Output = [System.IO.Path]::ChangeExtension(
            [System.IO.Path]::GetFileName($Source), '.exe'
        )
    }

    Invoke-MSVCBuild `
        -Source $Source `
        -Output $Output `
        -CompilerFlags (@('/openmp') + $ExtraFlags)
}

function Build-MSMPI {
    [CmdletBinding()]
    param(
        [Parameter(Mandatory)]
        [string] $Source,

        [string] $Output,

        [string[]] $ExtraFlags = @()
    )

    if (-not $env:MSMPI_INC -or -not $env:MSMPI_LIB64) {
        throw 'MS-MPI environment is not configured. Use the Visual Studio Developer environment and check MSMPI_INC/MSMPI_LIB64.'
    }

    if (-not $Output) {
        $Output = [System.IO.Path]::ChangeExtension(
            [System.IO.Path]::GetFileName($Source), '.exe'
        )
    }

    $mpiCompilerFlags = @("/I$env:MSMPI_INC") + $ExtraFlags
    $mpiLinkerFlags = @("/LIBPATH:$env:MSMPI_LIB64", 'msmpi.lib')

    Invoke-MSVCBuild `
        -Source $Source `
        -Output $Output `
        -CompilerFlags $mpiCompilerFlags `
        -LinkerFlags $mpiLinkerFlags
}
