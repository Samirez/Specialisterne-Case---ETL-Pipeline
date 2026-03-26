[CmdletBinding()]
param(
    [switch]$Build,
    [switch]$Down,
    [switch]$Logs
)

$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = Resolve-Path (Join-Path $scriptDir "..\..")
$composeFile = Join-Path $repoRoot "compose.yaml"

if (-not (Test-Path $composeFile)) {
    throw "compose.yaml not found at expected path: $composeFile"
}

Push-Location $repoRoot
try {
    if ($Down) {
        Write-Host "Stopping containers..." -ForegroundColor Yellow
        docker compose -f $composeFile down
        exit $LASTEXITCODE
    }

    if ($Build) {
        Write-Host "Starting containers with build..." -ForegroundColor Cyan
        docker compose -f $composeFile up --build -d
    }
    else {
        Write-Host "Starting containers..." -ForegroundColor Cyan
        docker compose -f $composeFile up -d
    }

    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }

    Write-Host "Containers are up." -ForegroundColor Green

    if ($Logs) {
        Write-Host "Streaming logs (Ctrl+C to stop)..." -ForegroundColor Yellow
        docker compose -f $composeFile logs -f
        exit $LASTEXITCODE
    }

    Write-Host "Use -Logs to follow logs, or -Down to stop." -ForegroundColor Gray
}
finally {
    Pop-Location
}