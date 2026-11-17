param([switch]$Rebuild, [switch]$Settings, [ValidateSet('auto','zh-CN','en')][string]$Language = 'auto')
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$exe = Join-Path $root 'build\RYJSLaunchpadPreview.exe'
if ($Rebuild -or !(Test-Path -LiteralPath $exe)) {
    # Only close this workspace's own preview, never Windhawk or Explorer.
    Get-Process RYJSLaunchpadPreview -ErrorAction SilentlyContinue | Where-Object { $_.Path -eq $exe } | Stop-Process
    & (Join-Path $PSScriptRoot 'Build.ps1') -Verify
}
$launchArgs = @()
if ($Language -ne 'auto') { $launchArgs += @('--lang', $Language) }
if ($Settings) { $launchArgs += '--settings' }
# A visible interactive preview was explicitly requested; show its native UI.
if ($launchArgs.Count) { Start-Process -FilePath $exe -ArgumentList $launchArgs -WindowStyle Normal }
else { Start-Process -FilePath $exe -WindowStyle Normal }
