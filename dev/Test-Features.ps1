param([switch]$Build)
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
if ($Build) { & (Join-Path $PSScriptRoot 'Build.ps1') -Verify -Smoke }
New-Item -ItemType Directory -Path (Join-Path $root 'build') -Force | Out-Null
$clang = Join-Path $env:ProgramFiles 'Windhawk\Compiler\bin\clang++.exe'
$arguments = @('-std=c++23','-target','x86_64-w64-mingw32','-DUNICODE','-D_UNICODE','-DWINVER=0x0A00','-D_WIN32_WINNT=0x0A00','-D_WIN32_IE=0x0A00','-DNTDDI_VERSION=0x0A000008','-D__USE_MINGW_ANSI_STDIO=0','-O2','-static','-include','windows.h','-I',(Join-Path $PSScriptRoot 'preview'),'-municode',(Join-Path $PSScriptRoot 'test_features.cpp'),'-o',(Join-Path $root 'build\test_features.exe'),'-lgdi32','-lshell32','-lshcore','-ldwmapi','-ld2d1','-ldwrite','-ld3d11','-ldxgi','-ldcomp','-lole32','-luuid','-lcomdlg32','-luxtheme','-lpropsys','-lpowrprof','-lwindowscodecs','-limm32','-lcomctl32','-ladvapi32','-lwinhttp','-lshlwapi')
& $clang @arguments
if ($LASTEXITCODE) { throw 'Feature test compilation failed' }
& (Join-Path $root 'build\test_features.exe') | Tee-Object -FilePath (Join-Path $root 'build\feature-tests.log')
if ($LASTEXITCODE) { throw 'Feature regression failed' }
