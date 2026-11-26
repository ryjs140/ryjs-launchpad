param([string]$SnapshotData,[switch]$SkipBuild)
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
if (!$SnapshotData) {
    $SnapshotData = Join-Path $root 'build\synthetic-layout'
    New-Item -ItemType Directory -Path $SnapshotData -Force | Out-Null
    $utf8 = New-Object System.Text.UTF8Encoding($false)
    [IO.File]::WriteAllText((Join-Path $SnapshotData 'groups-v1-pinned.txt'), "schema`t1`tgroups`r`nfolder`tTools`r`nin`tC:\Fixture\First.exe`r`nin`tC:\Fixture\Second.exe`r`nfolder`tEmpty`r`n", $utf8)
    [IO.File]::WriteAllText((Join-Path $SnapshotData 'layout-allApps.txt'), "folder`tTools`r`nin`tC:\Fixture\First.exe`r`nin`tC:\Fixture\Second.exe`r`n", $utf8)
    [IO.File]::WriteAllText((Join-Path $SnapshotData 'pins.txt'), "pin`tc:\fixture\first.exe`r`npin`tc:\fixture\second.exe`r`n", $utf8)
}
$snapshotPath = (Resolve-Path -LiteralPath $SnapshotData).Path
$clang = Join-Path $env:ProgramFiles 'Windhawk\Compiler\bin\clang++.exe'
if (!$SkipBuild) { & (Join-Path $PSScriptRoot 'Build.ps1') -Verify -Smoke; if ($LASTEXITCODE) { throw 'Build/smoke failed' } }
$common = @('-std=c++23','-target','x86_64-w64-mingw32','-DUNICODE','-D_UNICODE','-DWINVER=0x0A00','-D_WIN32_WINNT=0x0A00','-D_WIN32_IE=0x0A00','-DNTDDI_VERSION=0x0A000008','-D__USE_MINGW_ANSI_STDIO=0','-O2','-static')
$libraries = @('-lgdi32','-lshell32','-lshcore','-ldwmapi','-ld2d1','-ldwrite','-ld3d11','-ldxgi','-ldcomp','-lole32','-luuid','-lcomdlg32','-luxtheme','-lpropsys','-lpowrprof','-lwindowscodecs','-limm32','-lcomctl32','-ladvapi32','-lwinhttp','-lshlwapi')
$test = Join-Path $root 'build\test_storage.exe'
$arguments = $common + @('-include','windows.h','-I',(Join-Path $PSScriptRoot 'preview'),'-municode',(Join-Path $PSScriptRoot 'test_storage.cpp'),'-o',$test) + $libraries
& $clang @arguments
if ($LASTEXITCODE) { throw 'Storage test compilation failed' }
& $test $snapshotPath (Join-Path $root 'build\storage-test-cases') | Tee-Object -FilePath (Join-Path $root 'build\storage-tests.log')
if ($LASTEXITCODE) { throw 'Storage regression failed' }
$buttonTest = Join-Path $root 'build\test_start_button.exe'
$arguments = $common + @((Join-Path $PSScriptRoot 'test_start_button.cpp'),'-lcomctl32','-lole32','-loleaut32','-lruntimeobject','-o',$buttonTest)
& $clang @arguments
if ($LASTEXITCODE) { throw 'Start button test compilation failed' }
& $buttonTest | Tee-Object -FilePath (Join-Path $root 'build\start-button-tests.log')
if ($LASTEXITCODE) { throw 'Start button regression failed' }
& python (Join-Path $PSScriptRoot 'test_layout_backup.py')
if ($LASTEXITCODE) { throw 'Backup regression failed' }
