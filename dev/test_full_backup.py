"""Run the exact embedded Windows PowerShell 5.1 worker against isolated data."""
import configparser
import hashlib
import json
import os
from pathlib import Path
import subprocess
import tempfile
import unittest
import zipfile

import layout_backup

ROOT = Path(__file__).resolve().parents[1]
POWERSHELL = Path(os.environ['SystemRoot']) / 'System32/WindowsPowerShell/v1.0/powershell.exe'


class FullBackupTests(unittest.TestCase):
    def setUp(self):
        (ROOT / 'build').mkdir(exist_ok=True)
        self.temp = tempfile.TemporaryDirectory(prefix='full-backup-', dir=ROOT / 'build')
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.storage = self.root / 'data-v1'
        self.storage.mkdir()
        self.worker = self.root / 'worker.ps1'
        self.worker.write_text((ROOT / 'dev/full_backup_worker.ps1').read_text(encoding='utf-8-sig'), encoding='utf-8-sig')
        self.host = {'appSource': 'pinned', 'gridLayout': 'groups', 'interfaceLanguage': 'zh-CN', 'backgroundDim': '10'}
        self.icon = self.root / '自定义图标.ico'
        self.icon.write_bytes(b'fixture-icon')
        self.files = {
            'groups-v1-pinned.txt': 'schema\t1\tgroups\nfolder\t工作\nin\tmanual:rdp\nfolder\t网页\nin\tmanual:web\n',
            'layout-pinned.txt': 'folder\t工作\nin\tmanual:rdp\nfolder\t网页\nin\tmanual:web\n',
            'group-geometry-layout-pinned.txt': 'schema\t1\tgroup-geometry\ngroup\t0\t工作\t20\t10\t400\t240\tgroup\ngroup\t1\t网页\t460\t10\t300\t180\tfolder\n',
            'entries-v1.txt': 'schema\t1\tentries\nentry\tmanual:rdp\t1\tC:\\Remote\\server.rdp\t服务器\t\t\nentry\tmanual:web\t1\thttps://example.org/\t自定义网站\t\t\n',
            'pins.txt': 'pin\tmanual:rdp\npin\tmanual:web\n',
            'icons.txt': f'icon\tmanual:rdp\t{self.icon}\n',
        }
        for name, text in self.files.items():
            (self.storage / name).write_text(text, encoding='utf-8', newline='')
        (self.storage / 'ryjs-storage.ini').write_text('[storage]\nschema=1\n', encoding='utf-16')
        (self.storage / 'ui-settings.ini').write_text(
            '[meta]\nversion=1\n[host]\nbackgroundDim=' + layout_backup.encode('10') +
            '\n[values]\nbackgroundDim=' + layout_backup.encode('35') + '\n[present]\nbackgroundDim=1\n', encoding='utf-16')
        self.archive = self.root / '完整备份.ryjs.zip'
        self.job_number = 0

    def run_job(self, mode, **overrides):
        self.job_number += 1
        work = self.root / f'job-{self.job_number}'
        work.mkdir()
        job = dict(mode=mode, archive=str(self.archive), storage=str(self.storage), mod_id='local@ryjs-launchpad',
                   version='0.2.1', preview=True,
                   defaults=self.host, host_settings=self.host, expected_hash='')
        job.update(overrides)
        config = work / 'job.json'
        config.write_text(json.dumps(job, ensure_ascii=False), encoding='utf-8')
        completed = subprocess.run([str(POWERSHELL), '-NoLogo', '-NoProfile', '-NonInteractive', '-ExecutionPolicy', 'Bypass',
                                    '-File', str(self.worker), '-JobPath', str(config)], capture_output=True, timeout=45)
        result = work / 'result.ini'
        self.assertTrue(result.exists(), completed.stdout.decode(errors='replace') + completed.stderr.decode(errors='replace'))
        ini = configparser.ConfigParser(interpolation=None)
        ini.read_string(result.read_text(encoding='utf-16'))
        values = dict(ini['result'])
        for key in ('summary', 'previous'):
            if key in values:
                values[key] = layout_backup.decode(values[key])
        return values

    def exported(self):
        result = self.run_job('export')
        self.assertEqual(result['status'], 'ok', result)
        return result

    def rewrite(self, change):
        with zipfile.ZipFile(self.archive) as archive:
            files = {name: archive.read(name) for name in archive.namelist()}
        manifest = json.loads(files.pop('manifest.json'))
        change(manifest, files)
        manifest['files'] = {name: {'size': len(data), 'sha256': hashlib.sha256(data).hexdigest()} for name, data in files.items()}
        with zipfile.ZipFile(self.archive, 'w', zipfile.ZIP_DEFLATED) as archive:
            for name, data in files.items():
                archive.writestr(name, data)
            archive.writestr('manifest.json', json.dumps(manifest, ensure_ascii=False))

    def test_sparse_half_grid_geometry_roundtrip(self):
        leaf = 'group-geometry-layout-pinned.txt'
        data = 'schema\t3\tgroup-geometry\ngroup\t1\t网页\t0\t0\t310\t270\tgroup\t0\t0\t0\t0\t5\t6\n'
        (self.storage / leaf).write_text(data, encoding='utf-8')
        self.exported()
        checked = self.run_job('inspect')
        self.assertEqual(checked['status'], 'ok', checked)
        destination = self.root / 'sparse-destination'
        destination.mkdir()
        result = self.run_job('import', storage=str(destination), expected_hash=checked['hash'])
        self.assertEqual(result['status'], 'ok', result)
        self.assertEqual((destination / leaf).read_text(encoding='utf-8'), data)

    def test_complete_roundtrip_preserves_layout_aliases_geometry_settings_and_icons(self):
        exported = self.exported()
        manifest, files = layout_backup.verify(self.archive)
        self.assertEqual(manifest['effective_settings']['backgroundDim'], '35')
        # Portable Windhawk and plain settings: no installed source, DLL or
        # registry export is packed any more.
        self.assertNotIn('source/installed.wh.cpp', files)
        self.assertNotIn('binary/installed.dll', files)
        self.assertNotIn('metadata/settings.reg', files)
        self.assertIn('metadata/恢复说明.txt', files)
        checked = self.run_job('inspect')
        self.assertEqual(checked['status'], 'ok', checked)
        self.assertEqual(checked['hash'], exported['hash'])
        destination = self.root / 'destination'
        destination.mkdir()
        (destination / 'keep.txt').write_text('previous state')
        result = self.run_job('import', storage=str(destination), expected_hash=checked['hash'], host_settings={**self.host, 'backgroundDim': '70'})
        self.assertEqual(result['status'], 'ok', result)
        self.assertEqual((Path(result['previous']) / 'keep.txt').read_text(), 'previous state')
        for name in self.files:
            if name != 'icons.txt':
                self.assertEqual((destination / name).read_bytes(), (self.storage / name).read_bytes())
        icon_path = (destination / 'icons.txt').read_text(encoding='utf-8').strip().split('\t')[2]
        self.assertEqual(Path(icon_path).read_bytes(), self.icon.read_bytes())
        settings = layout_backup.effective_settings(destination, {**self.host, 'backgroundDim': '70'})
        self.assertEqual(settings['backgroundDim'], '35')
        self.assertFalse((destination / 'source').exists())
        self.assertFalse((destination / 'binary').exists())
        self.assertFalse((destination / 'metadata').exists())

    def test_legacy_schema_one_is_importable(self):
        original = layout_backup.registry_settings
        layout_backup.registry_settings = lambda mod: self.host
        try:
            layout_backup.export(self.storage, 'local@seamless-launchpad-custom', self.archive)
        finally:
            layout_backup.registry_settings = original
        checked = self.run_job('inspect')
        self.assertEqual(checked['status'], 'ok', checked)
        result = self.run_job('import', storage=str(self.root / 'legacy-destination'), expected_hash=checked['hash'])
        self.assertEqual(result['status'], 'ok', result)

    def test_managed_icon_and_website_cache_survive_restoring_to_a_new_directory(self):
        icon = Path(r'custom-icons\{12345678-1234-1234-1234-123456789abc}.png')
        favicon = Path(r'web-icons\0123456789abcdef.ico')
        mapping = f'icon\tmanual:rdp\t{icon}\n'
        (self.storage / 'icons.txt').write_text(mapping, encoding='utf-8')
        for relative, content in ((icon, b'custom image'), (favicon, b'website image')):
            path = self.storage / relative
            path.parent.mkdir()
            path.write_bytes(content)
        self.icon.unlink()
        self.exported()
        _, files = layout_backup.verify(self.archive)
        self.assertIn('data/' + icon.as_posix(), files)
        self.assertIn('data/' + favicon.as_posix(), files)
        checked = self.run_job('inspect')
        self.assertEqual(checked['status'], 'ok', checked)
        destination = self.root / 'portable-destination'
        restored = self.run_job('import', storage=str(destination), expected_hash=checked['hash'])
        self.assertEqual(restored['status'], 'ok', restored)
        self.assertEqual((destination / 'icons.txt').read_text(encoding='utf-8'), mapping)
        self.assertEqual((destination / icon).read_bytes(), b'custom image')
        self.assertEqual((destination / favicon).read_bytes(), b'website image')

    def test_geometry_v4_round_trip(self):
        path = self.storage / 'group-geometry-layout-pinned.txt'
        geometry = (
            'schema\t4\tgroup-geometry\n'
            'group\t0\t工作\t20.25\t10.5\t400\t240\tgroup\t0\t0\t0\t0\t4\t8\t2\t0\t400.125\t240.75\n'
            'group\t1\t网页\t460\t10\t300\t180\tfolder\t3\t2\t8\t0\t0\t0\t3\t1\t0\t0\n'
        ).encode('utf-8')
        path.write_bytes(geometry)
        exported = self.exported()
        path.write_bytes(self.files[path.name].encode('utf-8'))
        restored = self.run_job('import', expected_hash=exported['hash'])
        self.assertEqual(restored['status'], 'ok', restored)
        self.assertEqual(path.read_bytes(), geometry)

    def test_geometry_v5_round_trip(self):
        path = self.storage / 'group-geometry-layout-pinned.txt'
        geometry = (
            'schema\t5\tgroup-geometry\n'
            'group\t0\t工作\t0\t0\t272\t342\tgroup\t0\t0\t0\t0\t2\t3\t4\t1\t0\t0\n'
        ).encode('utf-8')
        path.write_bytes(geometry)
        exported = self.exported()
        path.write_bytes(self.files[path.name].encode('utf-8'))
        restored = self.run_job('import', expected_hash=exported['hash'])
        self.assertEqual(restored['status'], 'ok', restored)
        self.assertEqual(path.read_bytes(), geometry)

    def test_geometry_v6_round_trip(self):
        path = self.storage / 'group-geometry-layout-pinned.txt'
        geometry = (
            'schema\t6\tgroup-geometry\t12\n'
            'group\t0\t工作\t60\t30\t240\t300\tgroup\t0\t0\t1\t1\t4\t10\t5\t1\t0\t0\n'
        ).encode('utf-8')
        path.write_bytes(geometry)
        exported = self.exported()
        path.write_bytes(self.files[path.name].encode('utf-8'))
        restored = self.run_job('import', expected_hash=exported['hash'])
        self.assertEqual(restored['status'], 'ok', restored)
        self.assertEqual(path.read_bytes(), geometry)

    def test_changed_archive_after_confirmation_does_not_replace_data(self):
        exported = self.exported()
        before = (self.storage / 'entries-v1.txt').read_bytes()
        self.rewrite(lambda manifest, files: manifest.update(version='changed'))
        result = self.run_job('import', expected_hash=exported['hash'])
        self.assertEqual(result['status'], 'error')
        self.assertEqual((self.storage / 'entries-v1.txt').read_bytes(), before)
        self.assertFalse(list(self.root.glob('data-v1.before-import-*')))

    def test_invalid_archives_leave_current_data_untouched(self):
        changes = {
            'future-schema': lambda m, f: m.update(schema=2),
            'unknown-app': lambda m, f: m.update(source_mod='unrelated-app'),
            'traversal': lambda m, f: f.update({'data/../escape': b'bad'}),
            'case-conflict': lambda m, f: f.update({'data/PINS.txt': f['data/pins.txt']}),
            'missing-layout': lambda m, f: f.pop('data/groups-v1-pinned.txt'),
            'future-groups': lambda m, f: f.update({'data/groups-v2-pinned.txt': b'future'}),
            'missing-asset': lambda m, f: f.pop(next(k for k in f if k.startswith('assets/'))),
            'duplicate-entry': lambda m, f: f.update({'data/entries-v1.txt': f['data/entries-v1.txt'] + f['data/entries-v1.txt'].splitlines(keepends=True)[1]}),
            'unmatched-geometry': lambda m, f: f.update({'data/group-geometry-layout-pinned.txt': f['data/group-geometry-layout-pinned.txt'].replace('工作'.encode(), '错误'.encode())}),
            'future-private-settings': lambda m, f: f.update({'data/ui-settings.ini': '[meta]\nversion=2\n'.encode('utf-16')}),
        }
        for name, change in changes.items():
            with self.subTest(name=name):
                if self.archive.exists():
                    self.archive.unlink()
                self.exported()
                self.rewrite(change)
                before = {p.name: p.read_bytes() for p in self.storage.iterdir()}
                result = self.run_job('import', expected_hash=hashlib.sha256(self.archive.read_bytes()).hexdigest())
                self.assertEqual(result['status'], 'error', result)
                self.assertEqual({p.name: p.read_bytes() for p in self.storage.iterdir()}, before)
                self.assertFalse(list(self.root.glob('data-v1.before-import-*')))

    def test_checksum_corruption_is_rejected(self):
        self.exported()
        with zipfile.ZipFile(self.archive) as archive:
            files = {n: archive.read(n) for n in archive.namelist()}
        files['data/pins.txt'] = b'pin\tchanged\n'
        with zipfile.ZipFile(self.archive, 'w') as archive:
            for name, data in files.items():
                archive.writestr(name, data)
        result = self.run_job('inspect')
        self.assertEqual(result['status'], 'error')
        self.assertIn('checksum', result['summary'].lower())

    def test_export_inside_storage_is_rejected(self):
        result = self.run_job('export', archive=str(self.storage / 'bad.zip'))
        self.assertEqual(result['status'], 'error')
        self.assertFalse((self.storage / 'bad.zip').exists())

    def test_production_import_cannot_target_legacy_storage(self):
        exported = self.exported()
        result = self.run_job('import', preview=False, expected_hash=exported['hash'])
        self.assertEqual(result['status'], 'error')
        self.assertIn('Lumiflow', result['summary'])


if __name__ == '__main__':
    unittest.main(verbosity=2)
