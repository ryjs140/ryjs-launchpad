"""Versioned, verified layout backup. Uses only Python's standard library.

Export reads legacy/RYJS data. Import writes only the explicitly chosen RYJS
destination, never the old mod, and never changes Windhawk registry settings.
"""
import argparse
import configparser
import ctypes
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import shutil
import sys
import uuid
import winreg
import zipfile

LEGACY = {'local@seamless-launchpad-custom', 'seamless-launchpad-custom',
          'local@seamless-launchpad', 'seamless-launchpad',
          'local@fullscreen-start-plus', 'fullscreen-start-plus'}
NEW = 'local@ryjs-launchpad'
BASE = Path(os.environ['ProgramData']) / 'Windhawk/Engine/ModsWritable/mod-storage'
MAX_BYTES = 256 * 1024 * 1024


def digest(data):
    return hashlib.sha256(data).hexdigest()


def registry_settings(mod):
    for hive in (winreg.HKEY_LOCAL_MACHINE, winreg.HKEY_CURRENT_USER):
        try:
            with winreg.OpenKey(hive, rf'SOFTWARE\Windhawk\Engine\Mods\{mod}\Settings') as key:
                result = {}
                for i in range(winreg.QueryInfoKey(key)[1]):
                    name, value, kind = winreg.EnumValue(key, i)
                    if kind not in (winreg.REG_SZ, winreg.REG_EXPAND_SZ, winreg.REG_DWORD):
                        raise ValueError(f'Unsupported setting type: {name}')
                    result[name] = str(value)
                return result
        except FileNotFoundError:
            pass
    return {}


def decode(value):
    return bytes.fromhex(value).decode('utf-16-be', errors='strict')


def encode(value):
    return value.encode('utf-16-be').hex().upper()


def effective_settings(storage, host):
    values = dict(host)
    file = storage / 'ui-settings.ini'
    if not file.exists():
        return values
    ini = configparser.ConfigParser(interpolation=None)
    ini.optionxform = str
    ini.read_string(file.read_text(encoding='utf-16'))
    if ini.get('meta', 'version', fallback='1') != '1':
        raise ValueError('Unknown private settings version')
    for name, present in ini.items('present') if ini.has_section('present') else []:
        if present != '1':
            continue
        previous = decode(ini.get('host', name))
        override = decode(ini.get('values', name))
        if previous == host.get(name, ''):
            values[name] = override
    return values


def is_data_file(path):
    return not path.name.startswith('performance') and path.suffix != '.tmp'


def read_data(storage):
    result = {}
    for path in sorted(storage.rglob('*')):
        if path.is_symlink() or (path.stat().st_file_attributes & 0x400):
            raise ValueError(f'Links/reparse points are not supported: {path}')
        if path.is_file() and is_data_file(path):
            result['data/' + path.relative_to(storage).as_posix()] = path.read_bytes()
    if not result:
        raise ValueError('No data files to export')
    return result


def export(storage, mod, output):
    storage = storage.resolve(strict=True)
    output = output.resolve()
    if output.is_relative_to(storage):
        raise ValueError('Choose an external archive path outside the mod storage')
    if output.exists():
        raise FileExistsError('Archive already exists; choose a new filename')
    host = registry_settings(mod)
    if not host:
        raise ValueError('Host settings are unavailable; refusing an incomplete backup')
    files = read_data(storage)
    assets = {}
    icon_records = files.get('data/icons.txt', b'').decode('utf-8-sig', errors='strict')
    for line in icon_records.splitlines():
        fields = line.split('\t')
        if len(fields) == 3 and fields[0] == 'icon':
            path = Path(fields[2])
            if not path.is_file():
                raise FileNotFoundError(f'Custom icon missing: {path}')
            data = path.read_bytes()
            entry = 'assets/' + digest(data) + path.suffix.lower()
            files[entry] = data
            assets[str(path)] = entry
    effective = effective_settings(storage, host)
    # A live writer must not silently produce a torn backup across files.
    if read_data(storage) != {k: v for k, v in files.items() if k.startswith('data/')} or registry_settings(mod) != host:
        raise RuntimeError('Source changed during backup; retry after closing the launcher')
    manifest = {'schema': 1, 'source_mod': mod, 'host_settings': host,
                'effective_settings': effective, 'icons': assets,
                'files': {name: {'sha256': digest(data), 'size': len(data)} for name, data in files.items()}}
    output.parent.mkdir(parents=True, exist_ok=True)
    with output.open('xb') as stream, zipfile.ZipFile(stream, 'w', zipfile.ZIP_DEFLATED) as archive:
        for name, data in files.items():
            archive.writestr(name, data)
        archive.writestr('manifest.json', json.dumps(manifest, ensure_ascii=False, indent=2))
    verify(output)
    return {'archive': str(output), 'sha256': digest(output.read_bytes()), 'files': len(files),
            'effective_settings': len(effective), 'custom_icons': len(assets)}


def verify(archive_path):
    with zipfile.ZipFile(archive_path) as archive:
        info = archive.infolist()
        if len({i.filename for i in info}) != len(info) or sum(i.file_size for i in info) > MAX_BYTES:
            raise ValueError('Duplicate entries or oversized archive')
        if any(i.file_size > MAX_BYTES for i in info):
            raise ValueError('Oversized entry')
        manifest = json.loads(archive.read('manifest.json'))
        if manifest.get('schema') != 1:
            raise ValueError('Unsupported backup schema')
        expected = set(manifest['files']) | {'manifest.json'}
        if expected != {i.filename for i in info}:
            raise ValueError('Archive entries differ from manifest')
        data = {}
        for name, meta in manifest['files'].items():
            parts = PurePosixPath(name)
            if '\\' in name or ':' in name or parts.is_absolute() or '..' in parts.parts or parts.parts[0] not in ('data', 'assets', 'source', 'binary', 'metadata'):
                raise ValueError(f'Unsafe backup entry: {name}')
            blob = archive.read(name)
            if len(blob) != meta['size'] or digest(blob) != meta['sha256']:
                raise ValueError(f'Checksum mismatch: {name}')
            data[name] = blob
        return manifest, data


def restore(archive, destination, replace=False):
    manifest, files = verify(archive)
    destination = destination.resolve()
    for old in LEGACY:
        old_path = (BASE / old).resolve()
        if destination == old_path or destination.is_relative_to(old_path):
            raise ValueError('Restoring into legacy mod storage is prohibited')
    if destination.exists() and not replace:
        raise FileExistsError('Destination exists. Keep it and choose an empty RYJS destination; no data was replaced')
    if destination.exists() and not (destination / 'ryjs-storage.ini').is_file():
        raise ValueError('Only a marked RYJS data directory can be replaced')
    kernel = ctypes.WinDLL('kernel32', use_last_error=True)
    kernel.OpenMutexW.argtypes = (ctypes.c_uint, ctypes.c_int, ctypes.c_wchar_p)
    kernel.OpenMutexW.restype = ctypes.c_void_p
    kernel.CloseHandle.argtypes = (ctypes.c_void_p,)
    handle = kernel.OpenMutexW(0x00100000, False, r'Local\RYJSLaunchpad.Integration')
    if handle:
        kernel.CloseHandle(handle)
        raise RuntimeError('Disable RYJS Launchpad before importing data')
    stage = destination.with_name(destination.name + '.restore-' + uuid.uuid4().hex)
    stage.mkdir(parents=True)
    for name, blob in files.items():
        relative = PurePosixPath(name)
        if relative.parts[0] not in ('data', 'assets'):
            continue
        relative = PurePosixPath(*relative.parts[1:]) if relative.parts[0] == 'data' else relative
        target = (stage / str(relative)).resolve()
        if not target.is_relative_to(stage.resolve()):
            raise ValueError('Extraction escaped staging directory')
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(blob)
    # Retarget only custom icon files; application targets/order/names remain
    # byte-identical. Preserve the original archive for exact source recovery.
    icons = stage / 'icons.txt'
    if manifest['icons'] and icons.exists():
        text = icons.read_text(encoding='utf-8-sig')
        lines = []
        for line in text.splitlines():
            fields = line.split('\t')
            if len(fields) == 3 and fields[0] == 'icon' and fields[2] in manifest['icons']:
                fields[2] = str(destination / manifest['icons'][fields[2]])
            lines.append('\t'.join(fields))
        text = '\r\n'.join(lines) + '\r\n'
        icons.write_text(text, encoding='utf-8', newline='')
    host = registry_settings(NEW) or registry_settings('ryjs-launchpad') or json.loads((Path(__file__).parent / 'settings-defaults.json').read_text(encoding='utf-8'))
    effective = manifest['effective_settings']
    ui = stage / 'ui-settings.ini'
    if ui.exists():
        shutil.copyfile(ui, stage / 'import-original-ui-settings.ini')
    sections = {'meta': {'version': '1'},
                'host': {k: encode(host.get(k, '')) for k in effective},
                'values': {k: encode(v) for k, v in effective.items()},
                'present': {k: '1' for k in effective}}
    text = ''.join(f'[{section}]\r\n' + ''.join(f'{k}={v}\r\n' for k, v in values.items()) for section, values in sections.items())
    ui.write_bytes(text.encode('utf-16'))
    (stage / 'ryjs-storage.ini').write_bytes('[storage]\r\nschema=1\r\n'.encode('utf-16'))
    # Both resolved paths stay inside the explicitly chosen destination parent.
    if stage.resolve().parent != destination.parent or (destination.exists() and not replace):
        raise ValueError('Unsafe or concurrently created destination')
    previous = None
    if destination.exists():
        previous = destination.with_name(destination.name + '.before-import-' + uuid.uuid4().hex)
        if previous.resolve().parent != destination.parent or destination.is_symlink():
            raise ValueError('Unsafe restore target')
        destination.rename(previous)
    try:
        stage.rename(destination)
    except Exception:
        if previous and not destination.exists(): previous.rename(destination)
        raise
    return {'destination': str(destination), 'files_verified': len(files), 'schema': 1,
            'previous_backup': str(previous) if previous else None}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest='command', required=True)
    out = sub.add_parser('export')
    out.add_argument('--mod-id', default=NEW, choices=sorted(LEGACY | {NEW, 'ryjs-launchpad'}))
    out.add_argument('--storage', type=Path)
    out.add_argument('--output', type=Path, required=True)
    check = sub.add_parser('verify')
    check.add_argument('archive', type=Path)
    incoming = sub.add_parser('import')
    incoming.add_argument('archive', type=Path)
    incoming.add_argument('--destination', type=Path, default=BASE / NEW / 'data-v1')
    incoming.add_argument('--replace', action='store_true', help='Preserve the current destination as .before-import-* before replacing it')
    args = parser.parse_args()
    if args.command == 'export':
        storage = args.storage or BASE / args.mod_id
        if args.mod_id in (NEW, 'ryjs-launchpad') and not args.storage:
            storage /= 'data-v1'
        result = export(storage, args.mod_id, args.output)
    elif args.command == 'verify':
        manifest, files = verify(args.archive)
        result = {'verified': True, 'files': len(files), 'schema': manifest['schema']}
    else:
        result = restore(args.archive, args.destination, args.replace)
    print(json.dumps(result, ensure_ascii=False, indent=2))


if __name__ == '__main__':
    try:
        main()
    except Exception as error:
        print(f'ERROR: {error}', file=sys.stderr)
        sys.exit(1)
