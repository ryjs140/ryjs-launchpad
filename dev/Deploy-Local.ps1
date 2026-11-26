param(
    [string]$Id = 'local@ryjs-launchpad',
    [switch]$SkipCopy
)
# Installs the freshly generated single-file mod source into the local Windhawk
# installation and asks Windhawk itself to compile it. Must run elevated:
# ModsSource and Engine\Mods live under ProgramData.
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$cli = Join-Path $env:ProgramFiles 'Windhawk\windhawk-cli.exe'
$source = Join-Path $root 'ryjs-launchpad.wh.cpp'
$modsSource = Join-Path $env:ProgramData "Windhawk\ModsSource\$Id.wh.cpp"
$modsDir = Join-Path $env:ProgramData 'Windhawk\Engine\Mods\64'
$regPath = "HKLM:\SOFTWARE\Windhawk\Engine\Mods\$Id"
$stamp = Get-Date -Format 'yyyyMMdd-HHmmss'
$backup = Join-Path $root "build\deploy-backup-$stamp"
New-Item -ItemType Directory -Path $backup -Force | Out-Null
$log = Join-Path $backup 'deploy.log'
function Say([string]$text) {
    $text | Tee-Object -FilePath $log -Append
}

Say "source: $source"
if (!(Test-Path -LiteralPath $source)) { throw "未找到生成好的源码：$source" }
if (!(Test-Path -LiteralPath $cli)) { throw "未找到 windhawk-cli.exe：$cli" }
if (!(Test-Path -LiteralPath $modsSource)) { throw "未找到已安装的本地 mod：$modsSource" }

# Backups first, so a failed compile can always be rolled back by hand.
Copy-Item -LiteralPath $modsSource -Destination (Join-Path $backup "$Id.wh.cpp.bak") -Force
$library = (Get-ItemProperty -Path $regPath).LibraryFileName
Copy-Item -LiteralPath (Join-Path $modsDir $library) -Destination (Join-Path $backup "$library.bak") -Force
Get-ItemProperty -Path $regPath | ConvertTo-Json -Depth 4 | Set-Content -LiteralPath (Join-Path $backup 'registry.json') -Encoding utf8
Say "backup: $backup ($library)"

$wasEnabled = ((& $cli mod list --json | ConvertFrom-Json).data.mods |
    Where-Object { $_.id -eq $Id }).enabled
Say "enabled before: $wasEnabled"

if (!$SkipCopy) {
    Copy-Item -LiteralPath $source -Destination $modsSource -Force
    Say "copied source: $((Get-Item -LiteralPath $modsSource).Length) bytes"
}

$started = Get-Date
& $cli mod compile $Id 2>&1 | ForEach-Object { Say ([string]$_) }
$code = $LASTEXITCODE
if ($code -ne 0 -and $wasEnabled) {
    # An enabled mod is loaded into Explorer; unload it and try again.
    Say "compile exit $code while enabled, retrying with the mod disabled"
    & $cli mod disable $Id --yes 2>&1 | ForEach-Object { Say ([string]$_) }
    & $cli mod compile $Id 2>&1 | ForEach-Object { Say ([string]$_) }
    $code = $LASTEXITCODE
    & $cli mod enable $Id --yes 2>&1 | ForEach-Object { Say ([string]$_) }
    if ($LASTEXITCODE -ne 0) { Say 'WARNING: could not re-enable the mod' }
}
if ($code -ne 0) { throw "Windhawk 编译失败（exit $code），日志：$log" }

$config = Get-ItemProperty -Path $regPath
$now = Join-Path $modsDir $config.LibraryFileName
if (!(Test-Path -LiteralPath $now)) { throw "编译后找不到注册表指向的 DLL：$now" }
$file = Get-Item -LiteralPath $now
if ($file.LastWriteTime -lt $started.AddSeconds(-2)) {
    throw "DLL 没有更新：$now（$($file.LastWriteTime)）"
}
Say "installed: $($file.Name) ($($file.Length) bytes, $($file.LastWriteTime))"
Say "enabled after: $(((& $cli mod list --json | ConvertFrom-Json).data.mods | Where-Object { $_.id -eq $Id }).enabled)"
Say 'DONE'
