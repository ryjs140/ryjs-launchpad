# Installs the Lumiflow keys helper: copies ryjs-launchpad-keys.exe to
# Program Files (only administrators can change it there), adds a scheduled task
# that starts it as administrator when you sign in, and starts it now.
# The legacy RYJSLaunchpad task and folder are migrated on the next install.
# Run as administrator. -Uninstall removes the task, the program and the folder.
param(
    [string]$Exe = (Join-Path $PSScriptRoot 'ryjs-launchpad-keys.exe'),
    [switch]$Uninstall
)
$ErrorActionPreference = 'Stop'
$dir = Join-Path $env:ProgramFiles 'Lumiflow'
$legacyDir = Join-Path $env:ProgramFiles 'RYJSLaunchpad'
$task = 'LumiflowKeys'
$legacyTask = 'RYJSLaunchpadKeys'
$expectedDirectory = [IO.Path]::GetFullPath((Join-Path $env:ProgramFiles 'Lumiflow'))
if ([IO.Path]::GetFullPath($dir) -ne $expectedDirectory) { throw 'Unsafe helper directory' }

if ($Uninstall) {
    foreach ($name in @($task, $legacyTask)) {
        Unregister-ScheduledTask -TaskName $name -Confirm:$false -ErrorAction SilentlyContinue
    }
    Get-Process ryjs-launchpad-keys -ErrorAction SilentlyContinue | Stop-Process -Force
    foreach ($path in @($dir, $legacyDir)) {
        Remove-Item -LiteralPath $path -Recurse -Force -ErrorAction SilentlyContinue
    }
    "removed"
    exit 0
}
# Stop and remove the old task before replacing the helper so its restart policy
# cannot race the new installation.
Unregister-ScheduledTask -TaskName $legacyTask -Confirm:$false -ErrorAction SilentlyContinue
Get-Process ryjs-launchpad-keys -ErrorAction SilentlyContinue | Stop-Process -Force
New-Item $dir -ItemType Directory -Force | Out-Null
Start-Sleep -Milliseconds 300  # The stopped copy lets go of the file.
Copy-Item $Exe "$dir\ryjs-launchpad-keys.exe" -Force
Remove-Item -LiteralPath $legacyDir -Recurse -Force -ErrorAction SilentlyContinue
$user = "$env:USERDOMAIN\$env:USERNAME"
$action = New-ScheduledTaskAction -Execute "$dir\ryjs-launchpad-keys.exe"
$trigger = New-ScheduledTaskTrigger -AtLogOn -User $user
$principal = New-ScheduledTaskPrincipal -UserId $user -LogonType Interactive -RunLevel Highest
# No time limit, normal priority (a task's default is below normal), and a
# restart if it ever stops.
$settings = New-ScheduledTaskSettingsSet -ExecutionTimeLimit ([TimeSpan]::Zero) -Priority 4 `
    -RestartCount 3 -RestartInterval (New-TimeSpan -Minutes 1) -AllowStartIfOnBatteries -DontStopIfGoingOnBatteries
Register-ScheduledTask -TaskName $task -Action $action -Trigger $trigger -Principal $principal `
    -Settings $settings -Force | Out-Null
Start-ScheduledTask -TaskName $task
Start-Sleep -Seconds 1
$p = Get-Process ryjs-launchpad-keys -ErrorAction SilentlyContinue
if (-not $p) { throw 'ryjs-launchpad-keys.exe did not start' }
"running, process $($p.Id)"
