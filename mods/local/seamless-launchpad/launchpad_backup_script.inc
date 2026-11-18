// Generated from dev/full_backup_worker.ps1 and the settings schema.
constexpr wchar_t kFullBackupScript[] = LR"RYJS_BACKUP(param([Parameter(Mandatory=$true)][string]$JobPath)
$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.IO.Compression.FileSystem
Add-Type -AssemblyName System.IO.Compression
$utf8 = New-Object Text.UTF8Encoding($false, $true)
$maxBytes = 256MB
$job = [IO.File]::ReadAllText($JobPath, $utf8) | ConvertFrom-Json
$work = Split-Path -Parent $JobPath

function Encode-Value([string]$value) {
    return -join ([Text.Encoding]::BigEndianUnicode.GetBytes($value) | ForEach-Object { $_.ToString('X2') })
}
function Decode-Value([string]$value) {
    if ($value -notmatch '^(?:[0-9A-Fa-f]{4})*$') { throw 'Invalid encoded setting.' }
    $bytes = New-Object byte[] ($value.Length / 2)
    for ($i=0; $i -lt $bytes.Length; $i++) { $bytes[$i] = [Convert]::ToByte($value.Substring($i*2,2),16) }
    return [Text.Encoding]::BigEndianUnicode.GetString($bytes)
}
function Hash-Bytes([byte[]]$bytes) {
    $sha = [Security.Cryptography.SHA256]::Create()
    try { return -join ($sha.ComputeHash($bytes) | ForEach-Object { $_.ToString('x2') }) }
    finally { $sha.Dispose() }
}
function Object-Map($value) {
    if ($null -eq $value -or $value -is [Array] -or $value -is [string]) { throw 'Expected an object.' }
    $map = @{}
    foreach ($p in $value.PSObject.Properties) { $map[$p.Name] = $p.Value }
    return $map
}
function Decode-Entry([string]$value) {
    if ($value -cmatch '%(?!(25|09|0A|0D))') { throw 'Invalid entry encoding.' }
    return $value.Replace('%09',"`t").Replace('%0A',"`n").Replace('%0D',"`r").Replace('%25','%')
}
function Read-Ini([byte[]]$bytes) {
    $text = if ($bytes.Length -ge 2 -and $bytes[0] -eq 255 -and $bytes[1] -eq 254) {
        [Text.Encoding]::Unicode.GetString($bytes,2,$bytes.Length-2)
    } else { $utf8.GetString($bytes).TrimStart([char]0xFEFF) }
    $ini = @{}
    $section = ''
    foreach ($line in ($text -split '\r?\n')) {
        if ($line -match '^\[([^\]]+)\]$') { $section=$matches[1]; $ini[$section]=@{} }
        elseif ($line -match '^([^=]+)=(.*)$' -and $section) { $ini[$section][$matches[1]]=$matches[2] }
    }
    return $ini
}
function Read-HostSettings {
    if ($job.preview) { return Object-Map $job.host_settings }
    $location = 'SOFTWARE\Windhawk\Engine\Mods\' + $job.mod_id + '\Settings'
    $script:registryHive='HKLM'
    $key = [Microsoft.Win32.Registry]::LocalMachine.OpenSubKey($location)
    if (!$key) { $key = [Microsoft.Win32.Registry]::CurrentUser.OpenSubKey($location); $script:registryHive='HKCU' }
    if (!$key) { throw 'Windhawk settings could not be read.' }
    try {
        $values=@{}
        foreach ($name in $key.GetValueNames()) {
            $kind=$key.GetValueKind($name)
            if ($kind -notin @('String','ExpandString','DWord')) { throw 'Unsupported registry setting type.' }
            $values[$name]=[string]$key.GetValue($name,$null,[Microsoft.Win32.RegistryValueOptions]::DoNotExpandEnvironmentNames)
        }
        return $values
    } finally { $key.Dispose() }
}
function Read-Tree([string]$root) {
    $root=[IO.Path]::GetFullPath($root).TrimEnd('\')
    $result=@{}
    $queue=New-Object 'Collections.Generic.Queue[string]'
    $queue.Enqueue($root)
    [long]$total=0
    while ($queue.Count) {
        $directory=$queue.Dequeue()
        if ((Get-Item -LiteralPath $directory -Force).Attributes -band [IO.FileAttributes]::ReparsePoint) { throw 'Linked data directories are unsupported.' }
        foreach ($entry in (Get-ChildItem -LiteralPath $directory -Force)) {
            if ($entry.Attributes -band [IO.FileAttributes]::ReparsePoint) { throw 'Linked data files are unsupported.' }
            if ($entry.PSIsContainer) { $queue.Enqueue($entry.FullName); continue }
            if ($entry.Name.StartsWith('performance') -or $entry.Extension -in @('.tmp','.download')) { continue }
            $total += $entry.Length
            if ($total -gt $maxBytes -or $result.Count -ge 4096) { throw 'Backup exceeds the size or file-count limit.' }
            $name='data/' + $entry.FullName.Substring($root.Length+1).Replace('\','/')
            $result[$name]=[IO.File]::ReadAllBytes($entry.FullName)
        }
    }
    if (!$result.Count) { throw 'No layout data was found.' }
    return $result
}
function Same-Files($a,$b) {
    if ($a.Count -ne $b.Count) { return $false }
    foreach ($name in $a.Keys) {
        if (!$b.ContainsKey($name) -or (Hash-Bytes $a[$name]) -ne (Hash-Bytes $b[$name])) { return $false }
    }
    return $true
}
function Same-Settings($a,$b) {
    if ($a.Count -ne $b.Count) { return $false }
    foreach ($name in $a.Keys) { if (!$b.ContainsKey($name) -or [string]$a[$name] -cne [string]$b[$name]) { return $false } }
    return $true
}
function Safe-Entry([string]$name) {
    if (!$name -or $name.Length -gt 1024 -or $name.Contains('\') -or $name.Contains(':') -or $name.Contains([string][char]0)) { throw 'Unsafe archive path.' }
    $parts=$name.Split('/')
    if ($parts[0] -notin @('data','assets','source','binary','metadata')) { throw 'Unknown archive entry category.' }
    foreach ($part in $parts) {
        if (!$part -or $part -in @('.','..') -or $part.EndsWith('.') -or $part.EndsWith(' ') -or $part -match '[<>"|?*\x00-\x1F]' -or $part -match '^(CON|PRN|AUX|NUL|COM[1-9]|LPT[1-9])(?:\.|$)') { throw 'Unsafe archive filename.' }
    }
}
function Validate-Data($manifest,$files) {
    $effective=Object-Map $manifest.effective_settings
    foreach ($p in $effective.GetEnumerator()) { if ($p.Value -isnot [string]) { throw 'Invalid effective setting.' } }
    $source=$effective['appSource']
    if ($source -notin @('allApps','desktop','both','pinned')) { throw 'Unsupported application source.' }
    $prefix=if ($effective['gridLayout'] -eq 'groups') { 'groups-v1-' } else { 'layout-' }
    if (!$files.ContainsKey('data/'+$prefix+$source+'.txt')) { throw 'Active layout is missing.' }
    if ($source -eq 'pinned' -and !$files.ContainsKey('data/pins.txt')) { throw 'Pinned-app data is missing.' }
    foreach ($name in $files.Keys) {
        if ($name -match '^data/groups-' -and $name -notmatch '^data/groups-v1-') { throw 'Unsupported group layout version.' }
        if ($name -eq 'data/ryjs-storage.ini') {
            $marker=Read-Ini $files[$name]
            if ($marker['storage']['schema'] -ne '1') { throw 'Unsupported storage version.' }
        }
        if ($name -eq 'data/ui-settings.ini') {
            $ini=Read-Ini $files[$name]
            if ($ini['meta'] -and $ini['meta']['version'] -ne '1') { throw 'Unsupported private-settings version.' }
        }
        if ($name -notmatch '^data/(groups-v1-|layout-|group-geometry-|entries-v1|pins\.txt|icons\.txt)' -or !$name.EndsWith('.txt')) { continue }
        $text=$utf8.GetString($files[$name]).TrimStart([char]0xFEFF)
        if ($text.Contains([string][char]0)) { throw 'Invalid text data.' }
        $rows=@($text -split '\r?\n' | Where-Object { $_ -ne '' })
        $leaf=$name.Substring(5)
        if ($leaf -match '^(groups-v1-|layout-)' -and !$rows.Count) { throw 'Empty layout file.' }
        $folder=$false
        $entryKeys=New-Object 'Collections.Generic.HashSet[string]' ([StringComparer]::Ordinal)
        $geometry=@{}
        for ($i=0; $i -lt $rows.Count; $i++) {
            $f=$rows[$i].Split("`t")
            if ($leaf.StartsWith('groups-v1-') -or $leaf.StartsWith('layout-')) {
                if ($i -eq 0 -and $leaf.StartsWith('groups-v1-')) {
                    if ($rows[$i] -ne "schema`t1`tgroups") { throw 'Unsupported group layout version.' }
                    continue
                }
                if ($f.Count -lt 2 -or !$f[1]) { throw 'Invalid layout record.' }
                if ($f[0] -eq 'folder') {
                    if ($f.Count -ne 2 -and !($f.Count -eq 3 -and $f[2] -eq 'inbox')) { throw 'Invalid folder record.' }
                    $folder=$true
                } elseif ($f[0] -eq 'in' -and $folder -and $f.Count -eq 2) { }
                elseif ($f[0] -in @('app','hidden') -and $f.Count -eq 2) { if ($f[0] -eq 'app') { $folder=$false } }
                else { throw 'Unsupported layout record.' }
            } elseif ($leaf -eq 'entries-v1.txt') {
                if ($i -eq 0) { if ($rows[$i] -ne "schema`t1`tentries") { throw 'Unsupported entry version.' }; continue }
                if ($f.Count -ne 7 -or $f[0] -ne 'entry' -or $f[2] -notin @('0','1') -or !$f[1] -or !$f[3] -or !$f[4]) { throw 'Invalid application entry.' }
                foreach ($value in $f) { $null=Decode-Entry $value }
                if (!$entryKeys.Add((Decode-Entry $f[1]))) { throw 'Duplicate application identity.' }
            } elseif ($leaf.StartsWith('group-geometry-')) {
                if ($i -eq 0) { if ($rows[$i] -ne "schema`t1`tgroup-geometry") { throw 'Unsupported geometry version.' }; continue }
                if ($f.Count -ne 8 -or $f[0] -ne 'group' -or $f[1] -notmatch '^\d+$' -or !$f[2] -or $f[7] -notin @('group','folder')) { throw 'Invalid group geometry.' }
                foreach ($value in $f[3..6]) {
                    $number=0.0
                    if (![double]::TryParse($value,[Globalization.NumberStyles]::Float,[Globalization.CultureInfo]::InvariantCulture,[ref]$number) -or [double]::IsNaN($number) -or [double]::IsInfinity($number) -or $number -lt 0 -or $number -gt 1000000) { throw 'Invalid group dimensions.' }
                }
                if ([double]::Parse($f[5],[Globalization.CultureInfo]::InvariantCulture) -lt 48 -or [double]::Parse($f[6],[Globalization.CultureInfo]::InvariantCulture) -lt 60) { throw 'Group dimensions are too small.' }
                $ordinal=0
                if (![int]::TryParse($f[1],[ref]$ordinal) -or $geometry.ContainsKey($ordinal)) { throw 'Duplicate or invalid group ordinal.' }
                $geometry[$ordinal]=Decode-Entry $f[2]
            } elseif ($leaf -eq 'pins.txt') {
                if ($f.Count -ne 2 -or $f[0] -ne 'pin' -or !$f[1]) { throw 'Invalid pin.' }
            } elseif ($leaf -eq 'icons.txt') {
                if ($f.Count -ne 3 -or $f[0] -ne 'icon' -or !$f[1] -or !$f[2]) { throw 'Invalid icon mapping.' }
            }
        }
        if ($leaf -in @('entries-v1.txt') -or $leaf.StartsWith('group-geometry-')) { if (!$rows.Count) { throw 'Empty versioned data.' } }
        if ($leaf.StartsWith('group-geometry-')) {
            $layout='data/'+$leaf.Substring('group-geometry-'.Length)
            if ($effective['gridLayout'] -eq 'groups') { $layout=$layout.Replace('data/layout-','data/groups-v1-') }
            if (!$files.ContainsKey($layout)) { throw 'Group geometry has no matching layout.' }
            $ordinal=0
            $folders=@{}
            foreach ($line in ($utf8.GetString($files[$layout]).TrimStart([char]0xFEFF) -split '\r?\n')) {
                $record=$line.Split("`t")
                if ($record[0] -eq 'folder') { $folders[$ordinal]=$record[1]; $ordinal++ }
                elseif ($record[0] -eq 'app') { $ordinal++ }
            }
            if ($folders.Count -ne $geometry.Count) { throw 'Group geometry is incomplete.' }
            foreach ($index in $folders.Keys) {
                if (!$geometry.ContainsKey($index) -or $geometry[$index] -cne $folders[$index]) { throw 'Group geometry does not match its layout.' }
            }
        }
    }
    $icons=Object-Map $manifest.icons
    foreach ($asset in $icons.Values) { if ($asset -isnot [string] -or !$asset.StartsWith('assets/') -or !$files.ContainsKey($asset)) { throw 'Custom icon asset is missing.' } }
    if ($files.ContainsKey('data/icons.txt')) {
        foreach ($line in ($utf8.GetString($files['data/icons.txt']) -split '\r?\n')) {
            if (!$line) { continue }
            $f=$line.Split("`t")
            if (!$icons.ContainsKey($f[2])) { throw 'Custom icon was not included in the backup.' }
        }
    }
}
function Read-Archive([string]$path) {
    if ((Get-Item -LiteralPath $path).Length -gt 320MB) { throw 'Archive exceeds size limit.' }
    $zip=[IO.Compression.ZipFile]::OpenRead($path)
    try {
        $names=New-Object 'Collections.Generic.HashSet[string]' ([StringComparer]::OrdinalIgnoreCase)
        [long]$total=0
        if ($zip.Entries.Count -gt 4096) { throw 'Too many archive entries.' }
        $files=@{}
        foreach ($entry in $zip.Entries) {
            if (!$names.Add($entry.FullName)) { throw 'Duplicate or case-conflicting archive path.' }
            if ($entry.FullName -ne 'manifest.json') { Safe-Entry $entry.FullName }
            if ((($entry.ExternalAttributes -shr 16) -band 61440) -eq 40960) { throw 'Linked archive entries are unsupported.' }
            $total += $entry.Length
            if ($entry.Length -gt $maxBytes -or $total -gt $maxBytes) { throw 'Expanded backup exceeds size limit.' }
            $stream=$entry.Open()
            $buffer=New-Object IO.MemoryStream
            try {
                $chunk=New-Object byte[] 16384
                while (($read=$stream.Read($chunk,0,$chunk.Length)) -gt 0) {
                    if ($buffer.Length+$read -gt $entry.Length -or $buffer.Length+$read -gt $maxBytes) { throw 'Expanded entry exceeds its declared size.' }
                    $buffer.Write($chunk,0,$read)
                }
                if ($buffer.Length -ne $entry.Length) { throw 'Archive entry length mismatch.' }
                $files[$entry.FullName]=$buffer.ToArray()
            } finally { $stream.Dispose(); $buffer.Dispose() }
        }
        if (!$files.ContainsKey('manifest.json')) { throw 'This is not a complete Launchpad backup.' }
        $manifest=$utf8.GetString($files['manifest.json']) | ConvertFrom-Json
        if ($manifest.schema -ne 1) { throw 'Unsupported backup version.' }
        if ($manifest.source_mod -notin @('local@ryjs-launchpad','ryjs-launchpad','local@seamless-launchpad-custom','seamless-launchpad-custom','local@seamless-launchpad','seamless-launchpad','local@fullscreen-start-plus','fullscreen-start-plus')) { throw 'Backup belongs to a different application.' }
        $expected=Object-Map $manifest.files
        if ($expected.Count+1 -ne $files.Count) { throw 'Archive entries differ from manifest.' }
        foreach ($name in $expected.Keys) {
            Safe-Entry $name
            if (!$files.ContainsKey($name) -or $files[$name].Length -ne $expected[$name].size -or (Hash-Bytes $files[$name]) -cne $expected[$name].sha256) { throw 'Backup checksum mismatch.' }
        }
        $files.Remove('manifest.json')
        Validate-Data $manifest $files
        return @{ manifest=$manifest; files=$files; hash=(Hash-Bytes ([IO.File]::ReadAllBytes($path))) }
    } finally { $zip.Dispose() }
}
function Export-Complete {
    $archivePath=[IO.Path]::GetFullPath($job.archive)
    $storagePath=[IO.Path]::GetFullPath($job.storage).TrimEnd('\')+'\'
    if ($archivePath.StartsWith($storagePath,[StringComparison]::OrdinalIgnoreCase) -or [IO.Path]::GetExtension($archivePath) -ne '.zip') { throw 'Choose a ZIP file outside application data.' }
    $files=Read-Tree $job.storage
    $snapshot=@{} + $files
    $hostSettings=Read-HostSettings
    $effective=Object-Map $job.defaults
    foreach ($key in $hostSettings.Keys) { $effective[$key]=[string]$hostSettings[$key] }
    if ($files.ContainsKey('data/ui-settings.ini')) {
        $ini=Read-Ini $files['data/ui-settings.ini']
        if ($ini['meta']['version'] -ne '1') { throw 'Unsupported private-settings version.' }
        foreach ($key in $ini['present'].Keys) {
            if ($ini['present'][$key] -ne '1') { continue }
            $baseline=if ($hostSettings.ContainsKey($key)) { [string]$hostSettings[$key] } else { '' }
            if ((Decode-Value $ini['host'][$key]) -ceq $baseline) { $effective[$key]=Decode-Value $ini['values'][$key] }
        }
    }
    $icons=@{}
    if ($files.ContainsKey('data/icons.txt')) {
        foreach ($line in ($utf8.GetString($files['data/icons.txt']) -split '\r?\n')) {
            if (!$line) { continue }
            $f=$line.Split("`t")
            if ($f.Count -ne 3 -or $f[0] -ne 'icon') { throw 'Invalid custom icon mapping.' }
            $blob=[IO.File]::ReadAllBytes($f[2])
            $name='assets/'+(Hash-Bytes $blob)+[IO.Path]::GetExtension($f[2]).ToLowerInvariant()
            $files[$name]=$blob
            $icons[$f[2]]=$name
        }
    }
    foreach ($item in @(@('source',$job.source),@('binary',$job.binary))) {
        if (!$item[1] -or !(Test-Path -LiteralPath $item[1] -PathType Leaf)) { throw 'Installed source or module could not be found.' }
        $files[$item[0]+'/'+[IO.Path]::GetFileName($item[1])]=[IO.File]::ReadAllBytes($item[1])
    }
    if (!$job.preview) {
        $regFile=Join-Path $work 'settings.reg'
        & "$env:SystemRoot\System32\reg.exe" export ($script:registryHive+'\SOFTWARE\Windhawk\Engine\Mods\'+$job.mod_id+'\Settings') $regFile /y | Out-Null
        if ($LASTEXITCODE) { throw 'Could not export Windhawk settings.' }
        $files['metadata/settings.reg']=[IO.File]::ReadAllBytes($regFile)
    }
    $instructions=@'
RYJS Launchpad 完整备份
在启动器菜单选择“导入完整备份”，选中本 ZIP，校验后确认即可。
导入仅恢复布局、设置和图标；source/binary 用于保留当前安装文件，不会执行或安装。
替换前的数据保留在原 data-v1 同级的 .before-import-* 目录。
'@
    $files['metadata/恢复说明.txt']=$utf8.GetBytes($instructions)
    $manifest=[ordered]@{schema=1; producer='ryjs-native-1'; source_mod=$job.mod_id; version=$job.version; created=[DateTimeOffset]::Now.ToString('o'); host_settings=$hostSettings; effective_settings=$effective; icons=$icons; files=@{}}
    foreach ($name in $files.Keys) { $manifest.files[$name]=@{size=$files[$name].Length;sha256=(Hash-Bytes $files[$name])} }
    Validate-Data ($manifest | ConvertTo-Json -Depth 8 | ConvertFrom-Json) $files
    if (!(Same-Files $snapshot (Read-Tree $job.storage)) -or !(Same-Settings $hostSettings (Read-HostSettings))) { throw 'Layout or settings changed during backup. Please retry.' }
    $temporary=$job.archive+'.pending-'+[Guid]::NewGuid().ToString('N')
    try {
        $stream=[IO.File]::Open($temporary,[IO.FileMode]::CreateNew)
        $zip=$null
        try {
            $zip=New-Object IO.Compression.ZipArchive($stream,[IO.Compression.ZipArchiveMode]::Create)
            $files['manifest.json']=$utf8.GetBytes(($manifest | ConvertTo-Json -Depth 8))
            foreach ($name in ($files.Keys | Sort-Object)) {
                $entry=$zip.CreateEntry($name,[IO.Compression.CompressionLevel]::Optimal)
                $output=$entry.Open()
                try { $output.Write($files[$name],0,$files[$name].Length) } finally { $output.Dispose() }
            }
        } finally { if($zip){$zip.Dispose()}; $stream.Dispose() }
        $verified=Read-Archive $temporary
        if (Test-Path -LiteralPath $job.archive) { [IO.File]::Replace($temporary,$job.archive,$null) }
        else { [IO.File]::Move($temporary,$job.archive) }
        return @{ summary=$job.archive; hash=$verified.hash; previous='' }
    } finally { if (Test-Path -LiteralPath $temporary) { try { Remove-Item -LiteralPath $temporary -Force } catch {} } }
}
function Import-Complete($checked) {
    if (!$job.expected_hash -or $checked.hash -cne $job.expected_hash) { throw 'Archive changed after confirmation.' }
    $destination=[IO.Path]::GetFullPath($job.storage).TrimEnd('\')
    if (!$job.preview) {
        $parent=Split-Path -Parent $destination
        if ([IO.Path]::GetFileName($destination) -ne 'data-v1' -or [IO.Path]::GetFileName($parent) -notin @('local@ryjs-launchpad','ryjs-launchpad')) { throw 'Import is restricted to RYJS storage.' }
        if (!(Test-Path -LiteralPath (Join-Path $destination 'ryjs-storage.ini'))) { throw 'Existing RYJS storage marker is missing.' }
    }
    $ancestor=$destination
    while ($ancestor) {
        if ((Test-Path -LiteralPath $ancestor) -and ((Get-Item -LiteralPath $ancestor -Force).Attributes -band [IO.FileAttributes]::ReparsePoint)) { throw 'Import destination contains a link.' }
        $ancestor=Split-Path -Parent $ancestor
    }
    $hostSettings=Read-HostSettings
    $stage=$destination+'.restore-'+[Guid]::NewGuid().ToString('N')
    [IO.Directory]::CreateDirectory($stage) | Out-Null
    foreach ($name in $checked.files.Keys) {
        if ($name.StartsWith('data/')) { $relative=$name.Substring(5) }
        elseif ($name.StartsWith('assets/')) { $relative=$name }
        else { continue } # Source, DLLs and registry exports are never installed or executed.
        $target=[IO.Path]::GetFullPath((Join-Path $stage $relative))
        if (!$target.StartsWith($stage+'\',[StringComparison]::OrdinalIgnoreCase)) { throw 'Import path escaped staging.' }
        [IO.Directory]::CreateDirectory((Split-Path -Parent $target)) | Out-Null
        [IO.File]::WriteAllBytes($target,$checked.files[$name])
    }
    $icons=Object-Map $checked.manifest.icons
    if ($icons.Count) {
        $iconFile=Join-Path $stage 'icons.txt'
        $lines=foreach ($line in ([IO.File]::ReadAllText($iconFile,$utf8) -split '\r?\n')) {
            if (!$line) { continue }
            $f=$line.Split("`t")
            if ($icons.ContainsKey($f[2])) { $f[2]=Join-Path $destination $icons[$f[2]] }
            $f -join "`t"
        }
        [IO.File]::WriteAllText($iconFile,($lines -join "`r`n")+"`r`n",$utf8)
    }
    $effective=Object-Map $checked.manifest.effective_settings
    $ui=Join-Path $stage 'ui-settings.ini'
    if (Test-Path -LiteralPath $ui) { [IO.File]::Copy($ui,(Join-Path $stage 'import-original-ui-settings.ini'),$true) }
    $text="[meta]`r`nversion=1`r`n"
    foreach ($section in @('host','values','present')) {
        $text+='['+$section+"]`r`n"
        foreach ($key in ($effective.Keys | Sort-Object)) {
            if ($key -match '[\r\n=\[\]]') { throw 'Invalid setting name.' }
            $value=if ($section -eq 'present') { '1' } elseif ($section -eq 'values') { Encode-Value ([string]$effective[$key]) } else { Encode-Value ([string]$hostSettings[$key]) }
            $text+=$key+'='+$value+"`r`n"
        }
    }
    [IO.File]::WriteAllText($ui,$text,[Text.Encoding]::Unicode)
    [IO.File]::WriteAllText((Join-Path $stage 'ryjs-storage.ini'),"[storage]`r`nschema=1`r`n",[Text.Encoding]::Unicode)
    if (!(Same-Settings $hostSettings (Read-HostSettings))) { throw 'Windhawk settings changed during import.' }
    $previous=$destination+'.before-import-'+[DateTime]::Now.ToString('yyyyMMdd-HHmmss')+'-'+[Guid]::NewGuid().ToString('N')
    $hadPrevious=Test-Path -LiteralPath $destination
    if ($hadPrevious) { [IO.Directory]::Move($destination,$previous) }
    try { [IO.Directory]::Move($stage,$destination) }
    catch {
        if ($hadPrevious -and !(Test-Path -LiteralPath $destination)) { [IO.Directory]::Move($previous,$destination) }
        throw
    }
    return @{summary=$destination; hash=$checked.hash; previous=$(if($hadPrevious){$previous}else{''})}
}
try {
    if ($job.mode -eq 'export') { $result=Export-Complete }
    elseif ($job.mode -in @('inspect','import')) {
        $checked=Read-Archive $job.archive
        if ($job.mode -eq 'import') { $result=Import-Complete $checked }
        else {
            $source=[string]$checked.manifest.source_mod
            $version=[string]$checked.manifest.version
            $result=@{summary=($source+' '+$version+"`r`n"+$checked.files.Count+' files');hash=$checked.hash;previous=''}
        }
    } else { throw 'Unknown backup operation.' }
    $resultText="[result]`r`nstatus=ok`r`nsummary="+(Encode-Value $result.summary)+"`r`nhash="+$result.hash+"`r`nprevious="+(Encode-Value $result.previous)+"`r`n"
} catch {
    $resultText="[result]`r`nstatus=error`r`nsummary="+(Encode-Value $_.Exception.Message)+"`r`n"
}
[IO.File]::WriteAllText((Join-Path $work 'result.ini'),$resultText,[Text.Encoding]::Unicode)
)RYJS_BACKUP";
constexpr wchar_t kFullBackupDefaults[] = LR"RYJS_DEFAULTS({"interfaceLanguage": "auto", "uiFont": "auto", "winKeyAction": "grid", "shiftWinKeyAction": "start", "startButtonAction": "grid", "program": "", "monitor": "mouse", "appSource": "allApps", "gridLayout": "groups", "gridColumns": "0", "coverScreen": "0", "gridIconSize": "56", "showLabels": "1", "normalizeIcons": "1", "blurMode": "custom", "blurAmount": "60", "backgroundOpacity": "30", "tintColor": "#000000", "accentColor": "#89B4FA", "glowStrength": "60", "glowSize": "14", "chromeOpacity": "90", "powerBar": "1", "dockAlignment": "center", "powerButtons.lock": "1", "powerButtons.signOut": "1", "powerButtons.sleep": "1", "powerButtons.hibernate": "1", "powerButtons.restart": "1", "powerButtons.shutDown": "1", "powerNoConfirm": "0", "wiggle": "1", "holdTime": "420", "animations": "1", "settingsGlass": "1", "nativeSettingsTitleBar": "0", "settingsTrayIcon": "1", "gpuPresenter": "1", "performanceLogging": "1", "frameRateLimit": "0", "openDuration": "280", "closeDuration": "180", "animationEasing": "settle", "reducedMotion": "0", "labelFontSize": "13", "horizontalSpacing": "56", "verticalSpacing": "16", "groupWidth": "0", "groupSpacing": "32", "groupFlow": "columns", "groupNavigation": "scroll", "folderDuration": "180", "themeMode": "custom", "textColor": "#CDD6F4", "groupColor": "#1E1E2E", "groupOpacity": "0", "hoverColor": "#89B4FA", "selectionColor": "#CBA6F7", "labelPlateOpacity": "65", "labelRadius": "6", "labelShadow": "45", "backgroundDim": "0"})RYJS_DEFAULTS";
