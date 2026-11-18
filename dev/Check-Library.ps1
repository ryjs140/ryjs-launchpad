param([string]$UpstreamPath, [switch]$CompileOnly)
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$upstream = if ($UpstreamPath) { (Resolve-Path -LiteralPath $UpstreamPath).Path } else { Join-Path $root 'build\library-upstream' }
if (!(Test-Path -LiteralPath (Join-Path $upstream '.git'))) {
    & git clone --depth 1 --filter=blob:none --sparse https://github.com/ramensoftware/windhawk-mods.git $upstream
    if ($LASTEXITCODE) { throw '官方检查工具下载失败' }
    & git -C $upstream sparse-checkout set .github scripts
    if ($LASTEXITCODE) { throw '官方检查工具检出失败' }
}
& python (Join-Path $PSScriptRoot 'generate_resources.py')
if ($LASTEXITCODE) { throw '生成资源失败' }
$stage = Join-Path $root 'build\check-snapshot'
$scriptDir = Join-Path $stage 'tools\library-check'
$modDir = Join-Path $stage 'mods\local\ryjs-launchpad'
New-Item -ItemType Directory -Path $scriptDir,$modDir -Force | Out-Null
Copy-Item -LiteralPath (Join-Path $root 'tools\library-check\check.py') -Destination $scriptDir
Copy-Item -LiteralPath (Join-Path $root 'tools\library-check\compile_isolated.py') -Destination $scriptDir
Copy-Item -LiteralPath (Join-Path $root 'build\ryjs-launchpad.wh.cpp') -Destination $modDir
# The official validator uses Python 3.12 syntax. Reuse the installed YAML
# package's pure Python fallback; nothing is installed globally.
$yamlParent = & python -c 'import pathlib,yaml; print(pathlib.Path(yaml.__file__).parent.parent)'
if ($LASTEXITCODE) { throw '请先安装 dev/requirements.txt 所列依赖' }
$oldPythonPath = $env:PYTHONPATH
$oldRunnerTemp = $env:RUNNER_TEMP
try {
    $env:PYTHONPATH = $yamlParent
    $env:RUNNER_TEMP = Join-Path $root 'build\library-check-work'
    Push-Location $stage
    if (!$CompileOnly) {
        & py -3.12 (Join-Path $scriptDir 'check.py') validate --upstream $upstream --only ryjs-launchpad --label ryjs-0.1-metadata | Tee-Object -FilePath (Join-Path $root 'build\library-validation.log')
        if ($LASTEXITCODE) { throw '官方元数据检查存在问题，请查看 build/library-validation.log' }
    }
    # Local verification must never install into the user's Windhawk instance.
    & py -3.12 (Join-Path $scriptDir 'check.py') compile --compile-script --upstream $upstream --windhawk (Join-Path $env:ProgramFiles 'Windhawk') --only ryjs-launchpad --label ryjs-official-compile | Tee-Object -FilePath (Join-Path $root 'build\library-compile.log')
    if ($LASTEXITCODE) { throw '官方编译检查存在问题，请查看 build/library-compile.log' }
} finally {
    Pop-Location
    $env:PYTHONPATH = $oldPythonPath
    $env:RUNNER_TEMP = $oldRunnerTemp
}
