param([switch]$Verify, [switch]$Smoke)
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$compilerRoot = Join-Path $env:ProgramFiles 'Windhawk\Compiler'
$clang = Join-Path $compilerRoot 'bin\clang++.exe'
if (!(Test-Path -LiteralPath $clang)) { throw '未找到 Windhawk 编译器。请安装 Windhawk，或更新 dev/Build.ps1 中的 compilerRoot。' }
$build = Join-Path $root 'build'
New-Item -ItemType Directory -Path $build -Force | Out-Null
& python (Join-Path $PSScriptRoot 'generate_resources.py')
if ($LASTEXITCODE) { throw '资源生成失败' }
$common = @('-std=c++23','-target','x86_64-w64-mingw32','-DUNICODE','-D_UNICODE','-DWINVER=0x0A00','-D_WIN32_WINNT=0x0A00','-D_WIN32_IE=0x0A00','-DNTDDI_VERSION=0x0A000008','-D__USE_MINGW_ANSI_STDIO=0','-Wno-pragma-pack','-Wno-pragma-system-header-outside-header','-O2')
$libraries = @('-lgdi32','-lshell32','-lshcore','-ldwmapi','-ld2d1','-ldwrite','-ld3d11','-ldxgi','-ldcomp','-lole32','-luuid','-lcomdlg32','-luxtheme','-lpropsys','-lpowrprof','-lwindowscodecs','-limm32','-lcomctl32','-ladvapi32')
$exe = Join-Path $build 'RYJSLaunchpadPreview.exe'
$argsPreview = $common + @('-include','windows.h','-I', (Join-Path $PSScriptRoot 'preview'),'-municode','-mwindows','-static','-o',$exe,(Join-Path $PSScriptRoot 'preview\main.cpp')) + $libraries
[IO.File]::WriteAllText((Join-Path $build 'compile-preview.log'), '')
$ErrorActionPreference = 'Continue'
& $clang @argsPreview 2>&1 | ForEach-Object { $_.ToString() } | Tee-Object -FilePath (Join-Path $build 'compile-preview.log')
$ErrorActionPreference = 'Stop'
if ($LASTEXITCODE) { throw "预览编译失败：$LASTEXITCODE" }
Add-Content -LiteralPath (Join-Path $build 'compile-preview.log') -Value 'PASS: preview compilation and linking'
if ($Verify) {
    $source = Join-Path $build 'ryjs-launchpad.wh.cpp'
    $engineBase = Join-Path $env:ProgramFiles 'Windhawk\Engine'
    $engineLib = Get-ChildItem -LiteralPath $engineBase -Filter windhawk.lib -Recurse | Where-Object { $_.Directory.Name -eq '64' } | Sort-Object FullName -Descending | Select-Object -First 1 -ExpandProperty FullName
    if (!$engineLib) { throw '未找到 x64 Windhawk 导入库，无法链接模块。' }
    # Match the installed Windhawk compiler flags; output stays in this workspace.
    $argsModule = $common + @('-DWH_MOD','-include',(Join-Path $PSScriptRoot 'mod_build.h'),'-include','windhawk_api.h','-I',(Join-Path $compilerRoot 'include'),'-shared','-Wl,--export-all-symbols','-o',(Join-Path $build 'ryjs-launchpad.dll'),$source,$engineLib) + $libraries
    [IO.File]::WriteAllText((Join-Path $build 'compile-module.log'), '')
    $ErrorActionPreference = 'Continue'
    & $clang @argsModule 2>&1 | ForEach-Object { $_.ToString() } | Tee-Object -FilePath (Join-Path $build 'compile-module.log')
    $ErrorActionPreference = 'Stop'
    if ($LASTEXITCODE) { throw "正式模块编译或链接失败：$LASTEXITCODE" }
    Add-Content -LiteralPath (Join-Path $build 'compile-module.log') -Value 'PASS: Windhawk x64 module compilation and linking'
}
Write-Host "已编译：$exe"
if ($Smoke) {
    $p = Start-Process -FilePath $exe -ArgumentList '--smoke' -WindowStyle Hidden -PassThru -Wait
    if ($p.ExitCode) { throw "离屏验证失败：$($p.ExitCode)，请查看 build/preview-data/preview.log" }
    Write-Host '离屏渲染、真实应用枚举和中英文验证通过。'
}
