"""Backup tests write only fresh fixtures below this repository's build/."""
import json
from pathlib import Path
import unittest
import uuid
import zipfile
import layout_backup as backup


class BackupTests(unittest.TestCase):
    def setUp(self):
        self.root = Path(__file__).resolve().parents[1] / 'build/backup-tests' / uuid.uuid4().hex
        self.source = self.root / 'source'
        self.source.mkdir(parents=True)
        self.host = {'gridLayout': 'scroll', 'appSource': 'allApps', 'uiFont': 'auto'}
        self.original_registry = backup.registry_settings
        backup.registry_settings = lambda mod: self.host if mod in backup.LEGACY else {'gridLayout': 'groups', 'appSource': 'allApps', 'uiFont': 'auto'}
        (self.source / 'groups-v1-allApps.txt').write_bytes('schema\t1\tgroups\r\nfolder\t科研软件\r\nin\tapp-甲\r\n'.encode())
        self.archive = self.root / 'backup.ryjs.zip'

    def tearDown(self):
        backup.registry_settings = self.original_registry

    def make_archive(self):
        return backup.export(self.source, 'local@seamless-launchpad-custom', self.archive)

    def test_roundtrip_and_explicit_replace_preserve_previous(self):
        original = (self.source / 'groups-v1-allApps.txt').read_bytes()
        self.make_archive()
        destination = self.root / 'restored'
        backup.restore(self.archive, destination)
        self.assertEqual(original, (destination / 'groups-v1-allApps.txt').read_bytes())
        with self.assertRaises(FileExistsError): backup.restore(self.archive, destination)
        result = backup.restore(self.archive, destination, replace=True)
        self.assertEqual(original, (Path(result['previous_backup']) / 'groups-v1-allApps.txt').read_bytes())
        self.assertEqual(original, (self.source / 'groups-v1-allApps.txt').read_bytes())

    def test_private_settings_and_custom_icon(self):
        icon = self.root / '中文图标.ico'
        icon.write_bytes(b'fixture-icon-bytes')
        (self.source / 'icons.txt').write_bytes(('icon\tapp-甲\t' + str(icon) + '\r\n').encode())
        text = '[meta]\nversion=1\n[host]\ngridLayout=' + backup.encode('scroll') + '\n[values]\ngridLayout=' + backup.encode('groups') + '\n[present]\ngridLayout=1\n'
        (self.source / 'ui-settings.ini').write_bytes(text.encode('utf-16'))
        original = (self.source / 'ui-settings.ini').read_bytes()
        self.make_archive()
        destination = self.root / 'restored'
        backup.restore(self.archive, destination)
        effective = backup.effective_settings(destination, backup.registry_settings(backup.NEW))
        self.assertEqual(effective['gridLayout'], 'groups')
        target = (destination / 'icons.txt').read_text(encoding='utf-8').strip().split('\t')[2]
        self.assertEqual(Path(target).read_bytes(), icon.read_bytes())
        self.assertEqual(original, (self.source / 'ui-settings.ini').read_bytes())

    def test_missing_icon_rejects_incomplete_export(self):
        (self.source / 'icons.txt').write_text('icon\tapp\t' + str(self.root / 'missing.ico'), encoding='utf-8')
        with self.assertRaises(FileNotFoundError): self.make_archive()
        self.assertFalse(self.archive.exists())

    def test_tampering_and_traversal_are_rejected_before_destination(self):
        self.make_archive()
        manifest, files = backup.verify(self.archive)
        for kind in ('tampered', 'traversal', 'future'):
            modified = json.loads(json.dumps(manifest))
            contents = dict(files)
            if kind == 'tampered': contents[next(iter(contents))] = b'changed'
            if kind == 'traversal':
                contents['data/../../escape.txt'] = b'escape'
                modified['files']['data/../../escape.txt'] = {'sha256': backup.digest(b'escape'), 'size': 6}
            if kind == 'future': modified['schema'] = 999
            path = self.root / f'{kind}.zip'
            with zipfile.ZipFile(path, 'w') as z:
                for name, data in contents.items(): z.writestr(name, data)
                z.writestr('manifest.json', json.dumps(modified))
            destination = self.root / kind
            with self.assertRaises(ValueError): backup.restore(path, destination)
            self.assertFalse(destination.exists())

    def test_legacy_storage_cannot_be_restore_target(self):
        self.make_archive()
        with self.assertRaises(ValueError): backup.restore(self.archive, backup.BASE / 'local@seamless-launchpad-custom' / 'data-v1', replace=True)


if __name__ == '__main__':
    unittest.main(verbosity=2)
