"""Run the upstream compiler without installing, including alpha version labels."""
import importlib.util
from pathlib import Path
import re
import sys


def main():
    script = Path(sys.argv.pop(1)).resolve()
    spec = importlib.util.spec_from_file_location('upstream_compile_mod', script)
    upstream = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = upstream
    spec.loader.exec_module(upstream)
    original = upstream.get_file_version

    def version(path):
        try:
            return original(path)
        except ValueError:
            label = upstream.get_file_version_info(path, ['FileVersion'])['FileVersion']
            match = re.fullmatch(r'(\d+(?:\.\d+){0,3})-[A-Za-z][A-Za-z0-9.\-]*', label)
            if not match:
                raise
            parts = [int(x) for x in match[1].split('.')]
            print(f'Compatibility adapter: numeric compiler version from {label}')
            return tuple((parts + [0] * 4)[:4])

    upstream.get_file_version = version
    return upstream.main()


if __name__ == '__main__':
    sys.exit(main())
