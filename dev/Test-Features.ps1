param([switch]$Build)
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
if ($Build) { & (Join-Path $PSScriptRoot 'Build.ps1') -Verify -Smoke }
New-Item -ItemType Directory -Path (Join-Path $root 'build') -Force | Out-Null
$clang = Join-Path $env:ProgramFiles 'Windhawk\Compiler\bin\clang++.exe'
& $clang -std=c++23 -target x86_64-w64-mingw32 -O2 -static -municode -mwindows (Join-Path $PSScriptRoot 'test_launch_probe.cpp') -o (Join-Path $root 'build\test_launch_probe.exe') -lshell32
if ($LASTEXITCODE) { throw 'Windowless launch probe compilation failed' }
$arguments = @('-std=c++23','-target','x86_64-w64-mingw32','-DUNICODE','-D_UNICODE','-DWINVER=0x0A00','-D_WIN32_WINNT=0x0A00','-D_WIN32_IE=0x0A00','-DNTDDI_VERSION=0x0A000008','-D__USE_MINGW_ANSI_STDIO=0','-O2','-static','-include','windows.h','-I',(Join-Path $PSScriptRoot 'preview'),'-municode',(Join-Path $PSScriptRoot 'test_features.cpp'),'-o',(Join-Path $root 'build\test_features.exe'),'-lgdi32','-lshell32','-lshcore','-ldwmapi','-ld2d1','-ldwrite','-ld3d11','-ldxgi','-ldcomp','-lole32','-luuid','-lcomdlg32','-luxtheme','-lpropsys','-lpowrprof','-lwindowscodecs','-limm32','-lcomctl32','-ladvapi32','-lwinhttp','-lshlwapi')
& $clang @arguments
if ($LASTEXITCODE) { throw 'Feature test compilation failed' }
@'
import base64
import ctypes
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
import subprocess
import sys
import threading
from urllib.parse import quote_from_bytes

# Inherit Windows' noninteractive error mode and report exits in the log.
ctypes.windll.kernel32.SetErrorMode(0x0001 | 0x0002)
executable = Path(sys.argv[1])

def run_test(label, *args):
    result = subprocess.run([executable, *args], capture_output=True, timeout=90,
                            creationflags=subprocess.CREATE_NO_WINDOW)
    output = (result.stdout + result.stderr).decode(errors='replace')
    with executable.with_name('feature-tests.log').open('a', encoding='utf-8') as log:
        log.write(output)
    print(output, end='', flush=True)
    if result.returncode:
        raise SystemExit(f'{label} failed (exit 0x{result.returncode & 0xffffffff:08X})')

executable.with_name('feature-tests.log').write_text('', encoding='utf-8')
run_test('Feature regression')
run_test('Keyboard batch regression', '--keyboard-batch')

class Fixture(BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/start':
            self.send_response(302)
            self.send_header('Location', '/site/index.html')
            self.end_headers()
            return
        svg = b'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 64 64"><rect width="64" height="64" fill="red"/></svg>'
        responses = {
            '/site/index.html': b'<link rel="icon" href="error.html"><link rel="icon" href="logo.svg">',
            '/site/error.html': b'<html>Not an icon</html>',
            '/site/logo.svg': svg,
            '/inline-svg': b'<link rel="icon" href="data:image/svg+xml,' + quote_from_bytes(svg).encode() + b'">',
            '/base64-svg': b'<link rel="icon" href="data:image/svg+xml;base64,' +
                           base64.b64encode(b'<!--' + b'x' * 4096 + b'-->' + svg) + b'">',
        }
        body = responses.get(self.path)
        self.send_response(200 if body else 404)
        self.end_headers()
        if body:
            self.wfile.write(body)

with ThreadingHTTPServer(('127.0.0.1', 0), Fixture) as server:
    threading.Thread(target=server.serve_forever, daemon=True).start()
    try:
        for route in ('/start', '/inline-svg', '/base64-svg'):
            url = f'http://127.0.0.1:{server.server_port}{route}'
            run_test(f'Website icon regression ({route})', '--web-icon-probe', url)
    finally:
        server.shutdown()
print('Website icon download and cache reload passed')
'@ | & python - (Join-Path $root 'build\test_features.exe')
if ($LASTEXITCODE) { throw 'Feature tests failed' }
